#ifndef _REQUESTHANDLER_H
#define _REQUESTHANDLER_H

#include <string>

class CRequest;

struct directoryentry
{
	char pPath[1024];
	char pName[256];
};

class CRequestHandler
{
public:
	static bool Handle(CRequest *pRequest);
	static void HandleGET(CRequest *pRequest);
	static void HandlePOST(CRequest *pRequest);
	static void SendDirContents(CRequest *pRequest, char* pDirName, bool bForTreeView);

private:
	static bool Impersonate(CRequest *pRequest);
};

#endif // _REQUESTHANDLER_H
