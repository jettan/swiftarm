#include <iostream>

#include "curl.h"
#include "easy.h"
#include "gtest.h"
#include "swift.h"

#include "DownloadManager.h"
#include "HttpServer.h"
#include "SearchEngine.h"
#include "Settings.h"

#include <pthread.h>

pthread_t server_thread;

/**
 * Define the InstallHTTPGateway method in httpgw.cpp.
 */
bool InstallHTTPGateway(struct event_base *evbase, swift::Address addr, uint32_t chunk_size, double *maxspeed);

void *serverCallback(void *args) {
	std::cout << "Entered HTTP server thread" << std::endl;
	HttpServer::init();
}

int main(int argc, char **argv){
	
	// Copied from Main.cpp in ./ws
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
	
	std::string http_address = Settings::getIP() + ":17758";
	
	// HTTP gateway address for swift to stream.
	swift::Address httpaddr = swift::Address(http_address.c_str());
	
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	std::cout << "HTTP gateway installation returned: " << res << std::endl;
	
	std::cout << "Initialised swift" << std::endl;
	
	// Set Download directory
	DownloadManager::init();
	
	//swift::Channel::debug_file = stdout;
	// Make httpserver loop
	int rc = pthread_create(&server_thread, NULL, serverCallback, NULL);
	std::cout << "pthread_create returned: " << rc << std::endl;
	sleep(1);
	// curl init
	curl_global_init(CURL_GLOBAL_ALL);
	
	// gtest init
	::testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}

