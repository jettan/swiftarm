#include <iostream>

#include "ticpp.h"
#include "swift.h"
#include "DownloadManager.h"
#include "HttpServer.h"
#include "SearchEngine.h"
#include "Utils.h"

#include <stdio.h>
#include <string.h>

/**
 * Define the InstallHTTPGateway method in httpgw.cpp.
 */
bool InstallHTTPGateway(struct event_base *evbase, swift::Address addr, uint32_t chunk_size, double *maxspeed);

bool InstallStatsGateway(struct event_base *evbase, swift::Address addr);

/**
 * Application main loop.
 */
int main() {
	
	// Start python binder
	SearchEngine::init();
	
	Settings::init(DEFAULT_DOWNLOAD_DIR);
	
	// Enable pthread use in libevent.
	evthread_use_pthreads();
	
	swift::Channel::evbase = event_base_new();
	
	evutil_socket_t sock = INVALID_SOCKET;
	swift::Address bindaddress;
	
	bindaddress = swift::Address(Settings::getIP().c_str(), 0);
	bindaddress.set_port(DEFAULT_PORT);
	sock = swift::Listen(swift::Address(bindaddress));
	
	std::cout << "Listening on port " << swift::BoundAddress(sock).port() << "." << std::endl;
	
	std::string http_address  = Settings::getIP() + ":15000";
	
	// HTTP gateway address for swift to stream.
	swift::Address httpaddr = swift::Address(http_address.c_str());
	
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	std::cout << "HTTP gateway installation returned: " << res << std::endl;
	
	//std::cout << "Stats gateway installation returned: " << res << std::endl;
	
	std::cout << "Initialised swift" << std::endl;
	
	// Set Download directory
	DownloadManager::init();
	
	
	// Make httpserver loop
	HttpServer::init();
}

