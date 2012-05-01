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
#include <unistd.h>
#include <assert.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "ws.h"
#include "socketlistener.h"
#include "request.h"


CSocketListener::CSocketListener() : m_iServerSock(-1), m_iMaxRequests(0)
#if USE_SSL == true
	, m_pCtx(NULL), m_pSSL(NULL)
#endif
{
	m_iSemId = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	union semun argument;
	unsigned short values[1];
	values[0] = 1;
	argument.array = values;
	semctl(m_iSemId, 0, SETALL, argument);

	/* Allocate a shared memory segment. */
	//m_iSharedSize = 0x6400;
	m_iSharedSize = getpagesize();
	m_iSegmentId = shmget(IPC_PRIVATE, m_iSharedSize, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	/* Attach the shared memory segment. */
	m_vRequests = (CRequest**)shmat(m_iSegmentId, 0, 0);
}

CSocketListener::~CSocketListener()
{
	Shutdown();
}

#if USE_SSL == true
bool CSocketListener::InitSSL(void)
{
	if (!USE_SSL)
		return true;

	SSL_library_init();

	SSL_METHOD *pMethod = SSLv23_server_method();
	if (!pMethod)
	{
		fprintf(stderr, "pMethod = NULL\n");
		return false;
	}

	m_pCtx = SSL_CTX_new(pMethod);
	if (!m_pCtx)
	{
		fprintf(stderr, "pCtx = NULL\n");
		return false;
	}

	if (SSL_CTX_use_certificate_file(m_pCtx, "localhost.crt", SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}

	if (SSL_CTX_use_PrivateKey_file(m_pCtx, "localhost.key", SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}

	if (!SSL_CTX_check_private_key(m_pCtx))
	{
		fprintf(stderr, "Private key does not match the certificate public key\n");
		return false;
	}
	return true;
}
#endif

void CSocketListener::dumpRequests(char *pMsg)
{
	fprintf(stderr, "%s\n", pMsg);
	for (int i = 0; i < 5; i++)
		fprintf(stderr, "m_vRequests[%d] = %d\n", i, m_vRequests[i]);
}

bool CSocketListener::Create(int iLineCount)
{
#if USE_SSL == true
	if (!InitSSL())
		return false;
#endif

	m_iMaxRequests = iLineCount;

	for (int i = 0; i < m_iMaxRequests; i++)
		m_vRequests[i] = 0;

	//dumpRequests("After init:");
	
	m_iServerSock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_iServerSock < 0)
	{
		fprintf(stderr, "Error: socket() failed\n");
		return false;
	}

	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(LISTENER_PORT);

	if (bind(m_iServerSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "Error: socket bind() failed, errno = %d\n", errno);
		return false;
	}

	if (listen(m_iServerSock, 256) < 0)
	{
		fprintf(stderr, "Error: socket listen() failed\n");
		return false;
	}

	fprintf(stderr, "Listening for connections...\n");
	return true;
}

CRequest* CSocketListener::Accept(void)
{
	int iNewSock;
	char pRawRequest[4096];
	memset(pRawRequest, 0, sizeof(pRawRequest));
	struct sockaddr_in cli_addr;
	int n, iCliLen = sizeof(cli_addr);
	char pBuf[128];

	iNewSock = accept(m_iServerSock, (struct sockaddr*)&cli_addr, (socklen_t*)&iCliLen);
	if (iNewSock < 0)
	{
		if (g_signalhandled)
			return NULL;

		sprintf(pBuf, "Error on accept, errno is %d\n", errno);
		fatalerror(pBuf);
	}

	sockethandle handle;
	handle.iSock = iNewSock;

#if USE_SSL == true
	if (USE_SSL)
	{
		m_pSSL = SSL_new(m_pCtx);
		if (!m_pSSL)
		{
			shutdown(iNewSock, SHUT_RDWR);
			close(iNewSock);
			fprintf(stderr, "Error: SSL_set_fd failed:\n");
			ERR_print_errors_fp(stderr);
			return NULL;
		}

		int iRet = SSL_set_fd(m_pSSL, iNewSock);
		if (iRet == -1)
		{
			shutdown(iNewSock, SHUT_RDWR);
			close(iNewSock);
			fprintf(stderr, "Error: SSL_set_fd failed:\n");
			ERR_print_errors_fp(stderr);
			return NULL;
		}

		iRet = SSL_accept(m_pSSL);
		if (iRet != 1)
		{
			int iError = SSL_get_error(m_pSSL, iRet);
			if (iError == SSL_ERROR_SYSCALL && iRet == -1 && ERR_get_error() == 0 && errno == 104)
			{
				shutdown(iNewSock, SHUT_RDWR);
				close(iNewSock);
				return NULL;
			}
			else if (iRet == -1 && iError == SSL_ERROR_WANT_READ)
			{
				// try again
				char pBuf[256];
				ssize_t sz;
				fd_set rfds;
				while (true)
				{
					FD_ZERO(&rfds);
					FD_SET(iNewSock, &rfds);
					int retval = select(iNewSock + 1, &rfds, NULL, NULL, NULL);
					if (retval == -1)
						perror("select()");
					else if (retval)
					{
						memset(pBuf, 0, sizeof(pBuf));
						if (FD_ISSET(iNewSock, &rfds))
						{
							iRet = SSL_accept(m_pSSL);
							iError = SSL_get_error(m_pSSL, iRet);
							if (iRet == -1 && iError == SSL_ERROR_WANT_READ)
							{
								// stay in loop
							}
							else
								break;
						}
						else
							fprintf(stderr, "This can't be!\n");
					}
					else
					{
						fprintf(stderr, "This can't be either!\n");
						break;
					}
				}

			}
			else
			{			
				int iError2 = ERR_get_error();
				fprintf(stderr, "Error: SSL_accept returned: %d, SSL_get_error returned: %d, ERR_get_error returned: %d, errno: %d, strerror: %s\n", iRet, iError, iError2, errno, strerror(errno));
				ERR_print_errors_fp(stderr);
				shutdown(iNewSock, SHUT_RDWR);
				close(iNewSock);
				return NULL;
			}
		}

		handle.pSSL = m_pSSL;
	}
#endif

	n = Read(handle, pRawRequest, sizeof(pRawRequest) - 1);
	if (n < 0)
	{
		if (g_signalhandled)
			return NULL;
		fatalerror("ERROR reading from socket\n");
	}

	if (n == 0)
	{
		shutdown(iNewSock, SHUT_RDWR);
		close(iNewSock);
		return NULL;
	}

	CRequest *pRequest = new CRequest(this, handle);
	if (!pRequest->ParseRequest(pRawRequest))
	{
		shutdown(iNewSock, SHUT_RDWR);
		close(iNewSock);
		delete pRequest;
		return NULL;
	}

	struct sembuf operations[1];
	/* Use the first (and only) semaphore. */
	operations[0].sem_num = 0;
	/* Decrement by 1. */
	operations[0].sem_op = -1;
	/* Permit undo’ing. */
	operations[0].sem_flg = SEM_UNDO;
	semop(m_iSemId, operations, 1);

	int idx;
	for (idx = 0; idx < m_iMaxRequests; idx++)
		if (m_vRequests[idx] == 0)
			break;

	assert(idx < 64);

	m_vRequests[idx] = pRequest;

	operations[0].sem_op = 1;
	semop(m_iSemId, operations, 1);
	return pRequest;
}

int CSocketListener::Read(sockethandle handle, char* pBuf, size_t sz)
{
#if USE_SSL == true
	if (USE_SSL)
		return SSL_read(handle.pSSL, pBuf, sz);
	else
#endif
		return recv(handle.iSock, pBuf, sz, 0);
}

int CSocketListener::Write(sockethandle handle, char* pBuf, size_t sz)
{
#if USE_SSL == true
	if (USE_SSL)
		return SSL_write(handle.pSSL, pBuf, sz);
	else
#endif
		return send(handle.iSock, pBuf, sz, 0);
}

ssize_t CSocketListener::WriteFile(sockethandle handle, int iFile, size_t sz)
{
#if USE_SSL == true
	if (USE_SSL)
	{
		char pBuf[1024];
		ssize_t bytesread;
		while (true)
		{
			memset(pBuf, 0, sizeof(pBuf));
			bytesread = read(iFile, pBuf, sizeof(pBuf) - 1);
			if (bytesread == -1)
				fprintf(stderr, "Error, read returned -1\n");

			SSL_write(handle.pSSL, pBuf, bytesread);
			if (bytesread < (sizeof(pBuf) - 1))
				break;
		}

		return sz;
	}
	else
#endif
	{
		off_t offset = 0;
		return sendfile(handle.iSock, iFile, &offset, sz);
	}
}

bool CSocketListener::PersistentWait(CRequest *pRequest)
{
	fd_set rfds;
	//struct timeval tv;
	FD_ZERO(&rfds);
	FD_SET(pRequest->GetHandle().iSock, &rfds);
	//tv.tv_sec = 3;
	//tv.tv_usec = 0;

	//int retval = select(pRequest->GetHandle().iSock + 1, &rfds, NULL, NULL, &tv);
	int retval = select(pRequest->GetHandle().iSock + 1, &rfds, NULL, NULL, NULL);
	if (retval == -1)
		perror("select()");
	else if (retval)
	{
		if (FD_ISSET(pRequest->GetHandle().iSock, &rfds))
		{
			char pRawRequest[4096];
			memset(pRawRequest, 0, sizeof(pRawRequest));
			int n = Read(pRequest->GetHandle(), pRawRequest, sizeof(pRawRequest) - 1);
			if (n < 0)
			{
				if (g_signalhandled)
					fprintf(stderr, "g_signalhandled\n");
				else
					fprintf(stderr, "ERROR reading from socket\n");
				return false;
			}

			if (n == 0)
			{
				shutdown(pRequest->GetHandle().iSock, SHUT_RDWR);
				close(pRequest->GetHandle().iSock);
				return false;
			}

			//CRequest *pRequest = new CRequest(this, pRequest->GetHandle());
			pRequest->Initialize();
			if (!pRequest->ParseRequest(pRawRequest))
			{
				fprintf(stderr, "ParseRequest failed\n");
				return false;
			}
		}
		else
		{
			fprintf(stderr, "This can't be!\n");
			return false;
		}
	}
	else
	{
		fprintf(stderr, "Neither can this be!\n");
		return false;
	}

	return true;
}

void CSocketListener::CloseRequest(CRequest *pRequest)
{
	//pthread_mutex_lock(m_pMutex);
	struct sembuf operations[1];
	/* Use the first (and only) semaphore. */
	operations[0].sem_num = 0;
	/* Decrement by 1. */
	operations[0].sem_op = -1;
	/* Permit undo’ing. */
	operations[0].sem_flg = SEM_UNDO;
	semop(m_iSemId, operations, 1);
	
	for (int i = 0; i < m_iMaxRequests; i++)
	{
		if (m_vRequests[i] == pRequest)
		{
			int j;
			for (j = i + 1; j < m_iMaxRequests; j++)
			{
				m_vRequests[j - 1] = m_vRequests[j];
				if (m_vRequests[j] == 0)
					break;
			}

			m_vRequests[j] = 0;
			
			shutdown(pRequest->GetHandle().iSock, SHUT_RDWR);
			close(pRequest->GetHandle().iSock);
#if USE_SSL == true
			if (USE_SSL)
				SSL_free(pRequest->GetHandle().pSSL);
#endif

			delete pRequest;
			break;
		}
	}

	operations[0].sem_op = 1;
	semop(m_iSemId, operations, 1);
}

void CSocketListener::Shutdown(void)
{
	if (m_iSemId == 0)
		return;

	struct sembuf operations[1];
	/* Use the first (and only) semaphore. */
	operations[0].sem_num = 0;
	/* Decrement by 1. */
	operations[0].sem_op = -1;
	/* Permit undo’ing. */
	operations[0].sem_flg = SEM_UNDO;
	semop(m_iSemId, operations, 1);

	// close all request sockets
	for (int i = 0; i < m_iMaxRequests; i++)
	{
		CRequest *pRequest = m_vRequests[i];
		if (pRequest == NULL)
			break;

		shutdown(pRequest->GetHandle().iSock, SHUT_RDWR);
		close(pRequest->GetHandle().iSock);
#if USE_SSL == true
		if (USE_SSL)
			SSL_free(pRequest->GetHandle().pSSL);
#endif
		delete pRequest;
	}

	for (int i = 0; i < m_iMaxRequests; i++)
		m_vRequests[i] = 0;

	operations[0].sem_op = 1;
	semop(m_iSemId, operations, 1);

	// close the server socket
	if (m_iServerSock != -1)
	{
		shutdown(m_iServerSock, SHUT_RDWR);
		close(m_iServerSock);
		m_iServerSock = -1;
	}

#if USE_SSL == true
	if (USE_SSL)
	{
		SSL_CTX_free(m_pCtx);
		m_pCtx = NULL;
	}
#endif

	/* Detach the shared memory segment. */
	shmdt(m_vRequests);

	/* Deallocate the shared memory segment. */
	shmctl(m_iSegmentId, IPC_RMID, 0);

	union semun ignored_argument;
	semctl(m_iSemId, 1, IPC_RMID, ignored_argument);
	m_iSemId = 0;
}

void CSocketListener::fatalerror(char *pMsg)
{
	fprintf(stderr, "fatal error occurred: %s\n", pMsg);
	exit(1);
}
