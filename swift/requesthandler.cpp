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

#define checkResults(string, val) {                                                        \
	if (val) {                                                                             \
		printf("Failed with %d at %s.", val, string);                                      \
		exit(1);                                                                           \
	}                                                                                      \
}                                                                                          \

typedef struct str {
	char* tracker;
	char* hash;
	int filesize;
	char* filename;
} DownloadArgs;

sigset_t oSignalSet;
pthread_t thread;
int rc;

DownloadArgs download_args;
struct event evcompl;
int download;
int size;

CRequestHandler::CRequestHandler() {
	rc = 0;
	
	sigemptyset(&oSignalSet);
	sigaddset(&oSignalSet, SIGINT);
	sigaddset(&oSignalSet, SIGABRT);
	sigaddset(&oSignalSet, SIGQUIT);
	pthread_sigmask(SIG_BLOCK, &oSignalSet, NULL);
	
}

void IsCompleteCallback(int fd, short event, void *arg) {
	if (swift::SeqComplete(download) != size)
		evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	else
		event_base_loopexit(swift::Channel::evbase, NULL);
}


/**
 * Download a file using libswift.
 * @param tracker: The tracker (x.x.x.x:port).
 * @param hash: The root merkle hash (40 characters).
 * @param filesize: The filesize of the file.
 * @param filename: The name to be given to the downloaded file.
 */
void* Download(void *str) {
	DownloadArgs *da = (DownloadArgs*) str;
	download         = 0;
	char* tracker    = da->tracker;
	char* hash       = da->hash;
	size             = da->filesize;
	char* filename   = da->filename;
	
	// Make a socket to listen to.
	evutil_socket_t sock = INVALID_SOCKET;
	sock = swift::Listen(swift::Address("127.0.0.1:32997"));
	
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
		
		std::cout << "Download completed." << std::endl;
		// Close the file.
		swift::Close(download);
	}
	
	std::cout << "Exiting download thread." << std::endl;
	// Close the download thread when the download is finished.
	pthread_exit(NULL);
}

bool CRequestHandler::Handle(CRequest *pRequest) {
	if (strncmp(pRequest->GetCommand(), "GET", 3) == 0) {
		std::cout << "Received GET command." << std::endl;
		HandleGET(pRequest);
	}
	return true;
}

void CRequestHandler::HandleGET(CRequest *pRequest) {
	int val = 1;
	if (setsockopt(pRequest->GetHandle().iSock, SOL_TCP, TCP_CORK, & val, 4) == -1) {
		std::cerr << "setsockopt failed, errno = " << errno << std::endl;
		return;
	}
	
	char pBuf[8192];
	memset(pBuf, 0, sizeof(pBuf));
	
	char pFileName[128];
	memset(pFileName, 0, sizeof(pFileName));
	if (strcmp(pRequest->GetPath(), "/test.txt") == 0) {
		sprintf(pFileName, "test.txt");
		
	} else if (strcmp(pRequest->GetPath(), "/close_download") == 0) {
		std::cout << "Attempt to join download thread with main thread..." << std::endl;
		
		// Join the threads.
		void* status;
		rc = pthread_join(thread, &status);
		checkResults("joining download thread with main thread", rc);
		
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 16\n\n"
			"Closed download.");
		pRequest->Write(pBuf, strlen(pBuf));
		return;
		
	} else if (strcmp(pRequest->GetPath(), "/download") == 0) {
		
		// Fill in the neccessary arguments to download a file.
		download_args.tracker  = "127.0.0.1:20000";
		download_args.hash     = "ed29d19bc8ea69dfb5910e7e20247ee7e002f321";
		download_args.filesize = 61382756;
		download_args.filename = "stream.mp4";
		
		// Spawn new thread to download the file requested.
		int retval = pthread_create(&thread, NULL, &Download, (void*) &download_args);
		
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 17\n\n"
			"Download started.");
		pRequest->Write(pBuf, strlen(pBuf));
		return;
		
	} else {
		std::cout << "Nothing to do here..." << std::endl;
	}
	
	int iFile = open(pFileName, O_RDONLY);
	if (iFile < 0) {
		if (errno == EACCES) {
			sprintf(pBuf, "HTTP/1.1 403 Forbidden\n\n");
		} else if (errno == ENOENT) {
			sprintf(pBuf, "HTTP/1.1 404 File not found\n\n");
		} else {
			std::cerr << "Error: open failed, unknown errno: " << errno << std::endl;
			sprintf(pBuf, "HTTP/1.1 404 File not found\n\n");
		}
		pRequest->Write(pBuf, strlen(pBuf));
		return;
	}
	
	struct stat buf;
	memset(&buf, 0, sizeof(buf));
	
	if (fstat(iFile, &buf) != 0) {
		std::cerr << "error, fstat failed for: " << pFileName << std::endl;
		close(iFile);
		return;
	}
	
	// Check file type
	char *pFileType = pFileName + (strlen(pFileName) - 5);
	if (strcmp(pFileType + 1, ".txt") == 0) {
		std::cout << "Sending the header..." << std::endl;
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Accept-Ranges: none\n"
			"Keep-Alive: timeout=15, max=100\n"
			"Connection: Keep-Alive\n"
			"Content-Type: text/plain\n"
			"Content-Length: %ld\n\n", buf.st_size);
	}
	
	pRequest->Write(pBuf, strlen(pBuf));
	
	std::cout << "Sending the message..." << std::endl;
	ssize_t bytestowrite = buf.st_size;
	off_t offset = 0;
	ssize_t byteswritten = pRequest->WriteFile(iFile, bytestowrite);
	std::cout << "Sent the message." << std::endl;
	
	if (byteswritten != bytestowrite) {
		std::cerr << "error, bytestowrite = " << bytestowrite << ", byteswritten = " << byteswritten << std::endl;
		close(iFile);
		return;
	}
	
	close(iFile);
	
	return;
}
