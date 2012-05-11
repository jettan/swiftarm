#include <string>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <vector>
#include <float.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

#include "swift.h"
#include "ws.h"
#include "requesthandler.h"
#include "socketlistener.h"
#include "request.h"

typedef struct str {
	char* tracker;
	char* hash;
	char* filename;
} DownloadArgs;

sigset_t oSignalSet;
pthread_t thread;
int rc;

DownloadArgs download_args;
struct event evcompl;
struct event evclose;
int download;

bool streaming;

CRequestHandler::CRequestHandler() {
	rc = 0;
	
	sigemptyset(&oSignalSet);
	sigaddset(&oSignalSet, SIGINT);
	sigaddset(&oSignalSet, SIGABRT);
	sigaddset(&oSignalSet, SIGQUIT);
	sigaddset(&oSignalSet, SIGKILL);
	sigaddset(&oSignalSet, SIGTERM);
	pthread_sigmask(SIG_BLOCK, &oSignalSet, NULL);
}



/**
 * Callback to check if a download is completed.
 */
void IsCompleteCallback(int fd, short event, void *arg) {
	if (swift::SeqComplete(download) != swift::Size(download)) {
		std::cout << "Download busy..." << std::endl;
		evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	}
	else
		event_base_loopexit(swift::Channel::evbase, NULL);
}

void CloseCallback(int fd, short event, void *arg) {
	if (streaming) {
		evtimer_add(&evclose, swift::tint2tv(TINT_SEC));
		std::cout << "Busy streaming..." << std::endl;
	} else {
		std::cerr << "Calling loopexit of HTTPgw." << std::endl;
		event_base_loopexit(swift::Channel::evbase, NULL);
		std::cout << "loopexit called." << std::endl;
	}
}

/**
 * Download a file using libswift.
 * @param str: Struct containing all necessary data for downloads.
 */
void* Download(void *str) {
	std::cout << "Entered download thread." << std::endl;
	
	// Change the directory to Downloads folder.
	chdir("/tmp");
	
	DownloadArgs *da = (DownloadArgs*) str;
	download         = 0;
	char* tracker    = da->tracker;
	char* hash       = da->hash;
	char* filename   = da->filename;
	
	swift::Address trackeraddr = swift::Address(tracker);
	swift::Sha1Hash root_hash = swift::Sha1Hash(true, hash);
	
	// Set the tracker.
	std::cout << "Setting the tracker..." << std::endl;
	swift::SetTracker(trackeraddr);
	
	// Download the file.
	download = swift::Open(filename, root_hash);
	
	if (download < 0 ) {
		std::cerr << "Could not download " << filename << "!" << std::endl;
	} else {
		// Assign callbacks to the event base.
		std::cout << "Now dispatching event base." << std::endl;
		evtimer_assign(&evcompl, swift::Channel::evbase, IsCompleteCallback, NULL);
		evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
		
		// Dispatch the event base to enter the swift loop.
		event_base_dispatch(swift::Channel::evbase);
		
		std::cout << "Download of " << filename << " completed." << std::endl;
		// Close the file.
		swift::Close(download);
		
	}
	
	std::cout << "Exiting download thread." << std::endl;
	// Exit the download thread when the download is finished.
	pthread_exit(NULL);
}

/**
 * Stream a file using libswift.
 * @param str: Struct containing all necessary data for streams.
 */
void* Stream(void *str) {
	std::cout << "Entered the stream thread." << std::endl;
	
	// Change the directory to Downloads folder.
	chdir("/tmp");
	
	DownloadArgs *da = (DownloadArgs*) str;
	char* tracker    = da->tracker;
	
	swift::Address trackeraddr = swift::Address(tracker);
	
	// Set the tracker.
	std::cout << "Setting the tracker..." << std::endl;
	swift::SetTracker(trackeraddr);
	
	evtimer_assign(&evclose, swift::Channel::evbase, CloseCallback, NULL);
	evtimer_add(&evclose, swift::tint2tv(TINT_SEC));
	
	std::cout << "Dispatching the event base." << std::endl;
	event_base_dispatch(swift::Channel::evbase);
	
	std::cout << "Exiting stream thread." << std::endl;
	// Exit the stream thread when the download is finished.
	pthread_exit(NULL);
}

/**
 * The HTTP request command handler.
 * @param *pRequest: The request received.
 */
bool CRequestHandler::Handle(CRequest *pRequest) {
	if (strncmp(pRequest->GetCommand(), "GET", 3) == 0) {
		std::cout << "Received GET command." << std::endl;
		HandleGET(pRequest);
	}
	return true;
}

/**
 * The handler for all GET commands.
 * Commands can be "download" and "stop_download".
 */
void CRequestHandler::HandleGET(CRequest *pRequest) {
	int val = 1;
	if (setsockopt(pRequest->GetHandle().iSock, SOL_TCP, TCP_CORK, & val, 4) == -1) {
		std::cerr << "setsockopt failed, errno = " << errno << std::endl;
		return;
	}
	
	char message[8192];
	memset(message, 0, sizeof(message));
	
	if (strcmp(pRequest->GetPath(), "/download") == 0) {
		// Fill in the neccessary arguments to download a file.
		download_args.tracker  = "130.161.158.52:20000";
		download_args.hash     = "ed29d19bc8ea69dfb5910e7e20247ee7e002f321";
		download_args.filename = "stream.m2ts";
		
		// Spawn new thread to download the file requested.
		rc = pthread_create(&thread, NULL, Download, (void *) &download_args);
		
		if (rc) {
			std::cerr << "ERROR: failed to create download thread. Code: " << rc << "." << std::endl;
		}
		
		sprintf(message, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 41\n\n"
			"file:///tmp/stream.m2ts");
		pRequest->Write(message, strlen(message));
		
	} else if (strcmp(pRequest->GetPath(), "/close") == 0) {
		std::cout << "Close request received." << std::endl;
		streaming = false;
		
		sprintf(message, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 23\n\n"
			"Not streaming anymore.\n");
		pRequest->Write(message, strlen(message));
		
	} else if (strcmp(pRequest->GetPath(), "/stream") == 0) {
		streaming = true;
		download_args.tracker = "130.161.158.52:20000";
		rc = pthread_create(&thread, NULL, Stream, (void *) &download_args);
		
		if (rc) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << rc << "." << std::endl;
		}
		
		sprintf(message, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 69\n\n"
			"http://130.161.159.107:15000/ed29d19bc8ea69dfb5910e7e20247ee7e002f321");
		pRequest->Write(message, strlen(message));
		
	} else if (strcmp(pRequest->GetPath(), "/alive") == 0) {
		sprintf(message, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 11\n\n"
			"I'm alive!\n");
		pRequest->Write(message, strlen(message));
		
	} else {
		std::cerr << "Bad HTTP GET request." << std::endl;
	}
	return;
}
