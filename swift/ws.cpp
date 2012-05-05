#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "ws.h"
#include "socketlistener.h"
#include "request.h"
#include "requesthandler.h"
#include <iostream>
#include "compat.h"
#include "swift.h"

void signalhandler(int signum);
CSocketListener g_socketlistener;
bool g_signalhandled = false;
char g_signalname[32];

int main(int argc, char* argv[]) {
	swift::Channel::evbase = event_base_new();
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
