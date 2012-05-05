#ifndef _SOCKETLISTENER_H
#define _SOCKETLISTENER_H

struct sockethandle {
	int iSock;
};

class CRequest;

class CSocketListener {
	
	public:
		CSocketListener();
		~CSocketListener();
		bool Create(int iLineCount);
		CRequest* Accept(void);
		int Read(sockethandle handle, char* pBuf, size_t sz);
		int Write(sockethandle handle, char* pBuf, size_t sz);
		ssize_t WriteFile(sockethandle handle, int iFile, size_t sz);
		bool PersistentWait(CRequest *pRequest);
		void CloseRequest(CRequest* pRequest);
		void Shutdown(void);
	
	private:
		void fatalerror(const char *pMsg);
		void dumpRequests(char *pMsg);
		
		int m_iServerSock;
		int m_iMaxRequests;
		CRequest** m_vRequests;
		
		int m_iSegmentId;
		int m_iSharedSize;
		
		union semun {
			int val;
			struct semid_ds *buf;
			unsigned short int *array;
			struct seminfo *__buf;
		};
		
		int m_iSemId;
};

#endif // _SOCKETLISTENER_H
