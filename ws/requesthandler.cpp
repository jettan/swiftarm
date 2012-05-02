#include <string.h>
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

bool CRequestHandler::Handle(CRequest *pRequest)
{
	if (strncmp(pRequest->GetCommand(), "GET", 3) == 0) {
		std::cout << "Received GET command." << std::endl;
		HandleGET(pRequest);
	}
	return true;
}

void CRequestHandler::HandleGET(CRequest *pRequest)
{
	if (!USE_SSL)
	{
		int val = 1;
		if (setsockopt(pRequest->GetHandle().iSock, SOL_TCP, TCP_CORK, &val, 4) == -1)
		{
			fprintf(stderr, "setsockopt failed, errno = %d\n", errno);
			return;
		}
	}

	char pBuf[8192];
	memset(pBuf, 0, sizeof(pBuf));

	char pFileName[128];
	memset(pFileName, 0, sizeof(pFileName));
	if (strcmp(pRequest->GetPath(), "/test.txt") == 0) {
		sprintf(pFileName, "test.txt");
	}
	else
		std::cout << "Nothing to do here..." << std::endl;
		//sprintf(pFileName, "htdocs%s", pRequest->GetPath());

	//fprintf(stderr, "%s\n", pFileName);
	
	
	int iFile = open(pFileName, O_RDONLY);
	if (iFile < 0)
	{
		if (errno == EACCES)
			sprintf(pBuf, "HTTP/1.1 403 Forbidden\n\n");
		else if (errno == ENOENT)
			sprintf(pBuf, "HTTP/1.1 404 File not found\n\n");
		else
		{
			fprintf(stderr, "Error: open failed, unknown errno: %d\n", errno);
			sprintf(pBuf, "HTTP/1.1 404 File not found\n\n");
		}
		pRequest->Write(pBuf, strlen(pBuf));
		return;
	}

	struct stat buf;
	memset(&buf, 0, sizeof(buf));
	if (fstat(iFile, &buf) != 0)
	{
		fprintf(stderr, "error, fstat failed for: %s\n", pFileName);
		close(iFile);
		return;
	}

	// Check file type
	char *pFileType = pFileName + (strlen(pFileName) - 5);
	if (strcmp(pFileType + 1, ".txt") == 0)
	{
		std::cout << "Sending the header..." << std::endl;
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			//"Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0\n"
			//"Pragma: no-cache\n"
			"Accept-Ranges: none\n"
			"Keep-Alive: timeout=15, max=100\n"
			"Connection: Keep-Alive\n"
			"Content-Type: text/plain\n"
			"Content-Length: %d\n\n", buf.st_size);
	}
	else if (strcmp(pFileType + 1, ".gif") == 0)
	{
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Accept-Ranges: none\n"
			"Keep-Alive: timeout=15, max=100\n"
			"Connection: Keep-Alive\n"
			"Content-Type: image/gif\n"
			"Content-Length: %d\n\n", buf.st_size);
	}
	else if (strcmp(pFileType + 1, ".jpg") == 0)
	{
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Accept-Ranges: none\n"
			"Keep-Alive: timeout=15, max=100\n"
			"Connection: Keep-Alive\n"
			"Content-Type: image/jpeg\n"
			"Content-Length: %d\n\n", buf.st_size);
	}
	else if (strcmp(pFileType + 1, ".css") == 0)
	{
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Accept-Ranges: none\n"
			"Keep-Alive: timeout=15, max=100\n"
			"Connection: Keep-Alive\n"
			"Content-Type: text/css\n"
			"Content-Length: %d\n\n", buf.st_size);
	}
	else if (strcmp(pFileType + 2, ".js") == 0)
	{
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			"Accept-Ranges: none\n"
			"Keep-Alive: timeout=15, max=100\n"
			"Connection: Keep-Alive\n"
			"Content-Type: application/x-javascript\n"
			"Content-Length: %d\n\n", buf.st_size);
	}
	
	pRequest->Write(pBuf, strlen(pBuf));
	
	std::cout << "Sending the message..." << std::endl;
	ssize_t bytestowrite = buf.st_size;
	off_t offset = 0;
	ssize_t byteswritten = pRequest->WriteFile(iFile, bytestowrite);
	std::cout << "Sent the message." << std::endl;
	
	if (byteswritten != bytestowrite)
	{
		fprintf(stderr, "error, bytestowrite = %d, byteswritten = %d\n", bytestowrite, byteswritten);
		close(iFile);
		return;
	}
	
	close(iFile);
	
	return;
}
