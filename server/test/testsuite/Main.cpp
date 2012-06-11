#include <iostream>

#include "curl.h"
#include "easy.h"
#include "gtest.h"
#include "DownloadManager.h"
#include "swift.h"
#include "HttpServer.h"
#include "SearchEngine.h"
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
	
	// Enable pthread use in libevent.
	evthread_use_pthreads();
	
	swift::Channel::evbase = event_base_new();
	
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
			return -1;
		}
	}
	std::cout << "Listening on port " << swift::BoundAddress(sock).port() << "." << std::endl;
	
	// HTTP gateway address for swift to stream.
	//swift::Address httpaddr = swift::Address("130.161.158.52:15000");
	swift::Address httpaddr = swift::Address("127.0.0.1:15000");
	
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	std::cout << "HTTP gateway installation returned: " << res << std::endl;
	
	std::cout << "Initialised swift" << std::endl;
	
	// Set Download directory
	DownloadManager::setDownloadDirectory("/dtv/usb/sda1/Downloads");
	DownloadManager::init("/dtv/usb/sda1/Downloads");
	
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

