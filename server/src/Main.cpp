#include <iostream>

#include "ticpp.h"
#include "swift.h"
#include "DownloadManager.h"
#include "HttpServer.h"
#include "SearchEngine.h"

/**
 * Define the InstallHTTPGateway method in httpgw.cpp.
 */
bool InstallHTTPGateway(struct event_base *evbase, swift::Address addr, uint32_t chunk_size, double *maxspeed);

bool InstallStatsGateway(struct event_base *evbase, swift::Address addr);

/**
 * Application main loop.
 */
int main(){
	// Enable pthread use in libevent.
	evthread_use_pthreads();
	
	swift::Channel::evbase = event_base_new();
	
	
	evutil_socket_t sock = INVALID_SOCKET;
	swift::Address bindaddress;
	for (int i = 0; i < 10; i++) {
		bindaddress = swift::Address((uint32_t) INADDR_ANY, 0);
		bindaddress.set_port(25000);
		sock = swift::Listen(swift::Address(bindaddress));
		
		if (sock > 0) {
			break;
		}
		
		if (sock == 9) {
			std::cerr << "Could not listen to any socket for swift." << std::endl;
			return -1;
		}
	}
	std::cout << "Listening on port " << swift::BoundAddress(sock).port() << "." << std::endl;
	
	// HTTP gateway address for swift to stream.
	//swift::Address httpaddr = swift::Address("130.161.158.52:15000");
	swift::Address httpaddr = swift::Address("130.161.159.107:15000");
	//swift::Address statsaddr = swift::Address("127.0.0.1:6876");
	
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	std::cout << "HTTP gateway installation returned: " << res << std::endl;
	
	//res = InstallStatsGateway(swift::Channel::evbase, statsaddr);
	//std::cout << "Stats gateway installation returned: " << res << std::endl;
	
	std::cout << "Initialised swift" << std::endl;
	
	// Set Download directory
	DownloadManager::init("/dtv/usb/sda1/Downloads");
	
	// swift::Channel::debug_file = stdout;
	// Make httpserver loop
	HttpServer::init();
}

