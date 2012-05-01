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
#include <regex.h>
#include "ws.h"
#include "socketlistener.h"
#include "request.h"

CRequest::CRequest(CSocketListener *pSL, sockethandle handle) : m_pSocketListener(pSL), m_Handle(handle)
{
	Initialize();
}

CRequest::~CRequest()
{
}

void CRequest::Initialize()
{
	memset(m_pCommand, 0, sizeof(m_pCommand));
	memset(m_pPath, 0, sizeof(m_pPath));
	memset(m_pHTTPVersion, 0, sizeof(m_pHTTPVersion));
	memset(m_pHeaders, 0, sizeof(m_pHeaders));
	memset(m_pBuf, 0, sizeof(m_pBuf));
}

// Expected format:
// GET / HTTP/1.1\n
bool CRequest::ParseRequest(char *pRawRequest)
{
	char *pRegex = "(.*) (.*) HTTP/(.*)\\s((.*\\s)*)";
	re_set_syntax(RE_SYNTAX_POSIX_EGREP);
	struct re_pattern_buffer buffer;
	memset (&buffer, 0, sizeof (buffer));

	const char *pErr = re_compile_pattern(pRegex, strlen(pRegex), &buffer);
	if (pErr)
	{
		fprintf(stderr, "Error compiling regular expression while parsing request: %s\n", pErr);
		regfree(&buffer);
		return false;
	}

	struct re_registers regs;
	int iRet = re_search(&buffer, pRawRequest, strlen(pRawRequest), 0, strlen(pRawRequest), &regs);
	if (iRet == -2)
	{
		fprintf(stderr, "Internal error while searching request.\n");
		regfree(&buffer);
		return false;
	}
	else if (iRet == -1)
	{
		// malformatted request
		regfree(&buffer);
		return false;
	}

	strncpy(m_pCommand, pRawRequest + regs.start[1], regs.end[1] - regs.start[1]);
	strncpy(m_pPath, pRawRequest + regs.start[2], regs.end[2] - regs.start[2]);
	strncpy(m_pHTTPVersion, pRawRequest + regs.start[3], regs.end[3] - regs.start[3]);
	strncpy(m_pHeaders, pRawRequest + regs.start[4], regs.end[4] - regs.start[4]);
	regfree(&buffer);
	return true;
}

int CRequest::Read(char* pBuf, size_t sz)
{
	return m_pSocketListener->Read(m_Handle, pBuf, sz);
}

int CRequest::Write(char* pBuf, size_t sz)
{
	return m_pSocketListener->Write(m_Handle, pBuf, sz);
}

ssize_t CRequest::WriteFile(int iFile, size_t sz)
{
	return m_pSocketListener->WriteFile(m_Handle, iFile, sz);
}
