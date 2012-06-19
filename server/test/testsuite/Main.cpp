#include <iostream>

#include "curl.h"
#include "easy.h"
#include "gtest.h"
#include "swift.h"

#include "DownloadManager.h"
#include "HttpServer.h"
#include "SearchEngine.h"
#include "Utils.h"

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
	
	
	// Getting the IP address.
	struct ifaddrs * ifAddrStruct = NULL;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;
	std::string ip = DEFAULT_IP;
	
	getifaddrs(&ifAddrStruct);
	
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		
		if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
			
			// is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			if(strcmp(ifa->ifa_name, "eth0") == 0 || strcmp(ifa->ifa_name, "eth1") == 0 || strcmp(ifa->ifa_name, "wlan0") == 0) {
				printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
				ip = std::string(addressBuffer);
			}
		}
	}
	if (ifAddrStruct!=NULL) {
		freeifaddrs(ifAddrStruct);
	}
	
	// Copied from Main.cpp in ./ws
	
	// Enable pthread use in libevent.
	evthread_use_pthreads();
	
	swift::Channel::evbase = event_base_new();
	
	evutil_socket_t sock = INVALID_SOCKET;
	swift::Address bindaddress;
	
	bindaddress = swift::Address(ip.c_str(), 0);
	bindaddress.set_port(25000);
	sock = swift::Listen(swift::Address(bindaddress));
	
	std::cout << "Listening on port " << swift::BoundAddress(sock).port() << "." << std::endl;
	
	// HTTP gateway address for swift to stream.
	swift::Address httpaddr = swift::Address(ip.c_str());
	
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	std::cout << "HTTP gateway installation returned: " << res << std::endl;
	
	std::cout << "Initialised swift" << std::endl;
	
	// Set Download directory
	Settings::init("/dtv/usb/sda1/Downloads");
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

