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

bool CRequestHandler::Handle(CRequest *pRequest)
{
	if (strncmp(pRequest->GetCommand(), "GET", 3) == 0)
		HandleGET(pRequest);
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
	//sprintf(pBuf, "HTTP/1.1 200 OK\nDate: Sun, 13 Nov 2005 21:15:12 GMT\nServer: Apache/2.0.54 (Unix) mod_ssl/2.0.54 OpenSSL/0.9.7a PHP/5.0.5\nContent-Location: index.html.en\nVary: negotiate,accept-language,accept-charset\nTCN: choice\nLast-Modified: Sun, 21 Nov 2004 14:35:21 GMT\nETag: \"127e2d-5b0-a64a7c40;127e44-961-a64a7c40\"\nAccept-Ranges: bytes\nContent-Length: 1510\nKeep-Alive: timeout=15, max=100\nConnection: Keep-Alive\nContent-Type: text/html\nContent-Language: en\n\n");
	//sprintf(pBuf, "HTTP/1.1 200 OK\nDate: Sun, 13 Nov 2005 21:22:28 GMT\nServer: Apache/2.0.54 (Unix) mod_ssl/2.0.54 OpenSSL/0.9.7a PHP/5.0.5\nX-Powered-By: PHP/5.0.5\nSet-Cookie: PHPSESSID=0e90326d25dd3f59509b02cbd6c6848b; path=/\nExpires: Thu, 19 Nov 1981 08:52:00 GMT\nCache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0\nPragma: no-cache\nContent-Length: 1515\nKeep-Alive: timeout=15, max=100\nConnection: Keep-Alive\nContent-Type: text/html\nAllow: GET, POST, HEAD, PUT\n\n");

	char pFileName[256];
	memset(pFileName, 0, sizeof(pFileName));
	if (strcmp(pRequest->GetPath(), "/") == 0)
		sprintf(pFileName, "htdocs/index.html");
	else
		sprintf(pFileName, "htdocs%s", pRequest->GetPath());

	fprintf(stderr, "%s\n", pFileName);

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
	if (strcmp(pFileType, ".html") == 0)
	{
		sprintf(pBuf, "HTTP/1.1 200 OK\n"
			//"Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0\n"
			//"Pragma: no-cache\n"
			"Accept-Ranges: none\n"
			"Keep-Alive: timeout=15, max=100\n"
			"Connection: Keep-Alive\n"
			"Content-Type: text/html\n"
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

	ssize_t bytestowrite = buf.st_size;
	off_t offset = 0;
	ssize_t byteswritten = pRequest->WriteFile(iFile, bytestowrite);
	if (byteswritten != bytestowrite)
	{
		fprintf(stderr, "error, bytestowrite = %d, byteswritten = %d\n", bytestowrite, byteswritten);
		close(iFile);
		return;
	}

	close(iFile);
}
