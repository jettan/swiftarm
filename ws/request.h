#ifndef _REQUEST_H
#define _REQUEST_H

#include <string.h>

class CSocketListener;

class CRequest
{
public:
	CRequest(CSocketListener *pSocketListener, sockethandle handle);
	~CRequest();
	void Initialize(void);
	bool ParseRequest(char *pRawRequest);
	int Read(char* pBuf, size_t sz);
	int Write(char* pBuf, size_t sz);
	ssize_t WriteFile(int iFile, size_t sz);

	CSocketListener *GetSocketListener(void) { return m_pSocketListener; }
	sockethandle GetHandle(void) { return m_Handle; }
	char *GetCommand(void) { return m_pCommand; }
	char *GetPath(void) { return m_pPath; }
	char *GetHTTPVersion(void) { return m_pHTTPVersion; }
	char *GetHeaders(void) { return m_pHeaders; }
	char *GetLastError(void) { return m_pBuf; }

private:
	CSocketListener *m_pSocketListener;
	sockethandle m_Handle;
	char m_pCommand[16];
	char m_pPath[512];
	char m_pHTTPVersion[16];
	char m_pHeaders[8192];
	char m_pBuf[1024];
};

#endif // _REQUEST_H
