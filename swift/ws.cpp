#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <float.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <iostream>

#include "compat.h"
#include "socketlistener.h"
#include "request.h"
#include "requesthandler.h"
#include "ws.h"
#include "swift.h"


void signalhandler(int signum);
CSocketListener g_socketlistener;
bool g_signalhandled = false;
volatile bool streaming = false;
char g_signalname[32];

/**
 * Define the InstallHTTPGateway method in httpgw.cpp.
 */
bool InstallHTTPGateway(struct event_base *evbase, swift::Address addr, uint32_t chunk_size, double *maxspeed);

int main(int argc, char* argv[]) {
	
	char pBuf[512];
	// register to catch the signals
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &signalhandler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGCHLD, &sa, NULL);
	sigaction(SIGKILL, &sa, NULL);
	sigaction(SIGABRT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
	
	if (!g_socketlistener.Create(64)) {
		std::cerr << "Socket error." << std::endl;
		g_socketlistener.Shutdown();
		return 1;
	}
	
	swift::Channel::evbase = event_base_new();
	
	// Make a socket to listen to.
	evutil_socket_t sock = INVALID_SOCKET;
	swift::Address bindaddress;
	for (int i = 0; i < 10; i++) {
		bindaddress = swift::Address((uint32_t) INADDR_ANY, 0);
		sock = swift::Listen(swift::Address(bindaddress));
		
		if (sock > 0) {
			break;
		}
		
		if (sock == 9) {
			std::cerr << "Could not listen to any socket for swift." << std::endl;
			return 1;
		}
	}
	std::cout << "Listening on port " << swift::BoundAddress(sock).port() << "." << std::endl;
	
	swift::Address httpaddr    = swift::Address("130.161.159.107:15000");
	
//	swift::Address httpaddr    = swift::Address("130.161.158.52:15000");
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	
	std::cerr << "HTTP gateway creation returned " << res << "." << std::endl;
	
	while (true) {
		CRequest *pRequest = g_socketlistener.Accept(); // blocks until a connection is made
		
		if (g_signalhandled) {
			if (strcmp(g_signalname, "SIGCHLD") == 0) {
				int pid;
				int status;
				while (true) {
					pid = waitpid(WAIT_ANY, &status, WNOHANG);
					if (pid < 0) {
						break;
					}
					if (pid == 0) {
						break;
					}
					
					if (status != 0) {
						std::cerr << "Received SIGCHILD for pid = " << pid << ", status = " << status << std::endl;
					}
				}
				g_signalhandled = false;
			} else {
				break;
			}
		}
		
		if (pRequest) {
			if (fork() == 0) {
				while (true) {
					CRequestHandler::Handle(pRequest);
					bool bRet = g_socketlistener.PersistentWait(pRequest);
					if (!bRet) {
						break;
					}
				}
				
				g_socketlistener.CloseRequest(pRequest);
				_exit(0);
			}
			
			close(pRequest->GetHandle().iSock);
			delete pRequest;
		}
	}
	
	std::cout << "Shutting down listener." << std::endl;
	// Shutdown the listener port of swift.
	swift::Shutdown(sock);
	
	g_socketlistener.Shutdown();
	return 0;
}

void signalhandler(int signum) {
	g_signalhandled = true;
	if (signum == SIGINT) {
		strcpy(g_signalname, "SIGINT");
	} else if (signum == SIGTERM) {
		strcpy(g_signalname, "SIGTERM");
	} else if (signum == SIGCHLD) {
		strcpy(g_signalname, "SIGCHLD");
	} else if (signum == SIGKILL) {
		strcpy(g_signalname, "SIGKILL");
	} else if (signum == SIGABRT) {
		strcpy(g_signalname, "SIGABRT");
	} else if (signum == SIGPIPE) {
		strcpy(g_signalname, "SIGPIPE");
	}
}
