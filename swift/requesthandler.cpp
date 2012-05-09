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
#include <iostream>
#include <float.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

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
int download;

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

void IsCompleteCallback(int fd, short event, void *arg) {
	if (swift::SeqComplete(download) != swift::Size(download))
		evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	else
		event_base_loopexit(swift::Channel::evbase, NULL);
}


/**
 * Download a file using libswift.
 * @param tracker: The tracker (x.x.x.x:port).
 * @param hash: The root merkle hash (40 characters).
 * @param filename: The name to be given to the downloaded file.
 */
void* Download(void *str) {
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
		download_args.tracker  = "127.0.0.1:20000";
		download_args.hash     = "ed29d19bc8ea69dfb5910e7e20247ee7e002f321";
		download_args.filename = "stream.mp4";
		
		// Spawn new thread to download the file requested.
		rc = pthread_create(&thread, NULL, Download, (void *) &download_args);
		
		if (rc) {
			std::cerr << "ERROR: failed to create download thread. Code: " << rc << "." << std::endl;
		}
		
		sprintf(message, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 18\n\n"
			"Download started.\n");
		pRequest->Write(message, strlen(message));
		
	} else {
		std::cerr << "Bad HTTP GET request." << std::endl;
	}
	return;
}
