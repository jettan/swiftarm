#include <string>
#include <stdlib.h>
#include <stdio.h>
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
#include <assert.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <vector>
#include "ws.h"
#include "requesthandler.h"
#include "socketlistener.h"
#include "request.h"
#include <iostream>

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
	} else if (strcmp(pRequest->GetPath(), "/hello") == 0) {
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 11\n\n"
			"Hello World");
		pRequest->Write(pBuf, strlen(pBuf));
		return;
	} else if (strcmp(pRequest->GetPath(), "/stream") == 0) {
		// Call libswift to open stream...
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Content-Type: text/plain\n"
			"Content-Length: 68\n\n"
			"http://130.161.158.52:15000/ed29d19bc8ea69dfb5910e7e20247ee7e002f321");
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
