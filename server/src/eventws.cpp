#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <float.h>
#include <pthread.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event-config.h>
#include <event2/thread.h>
#include <math.h>

#include "swift.h"

namespace HttpServer {
	static struct event_base *base;
}

namespace DownloadManager {
	struct event evcompl;
	struct event evclose;
	
	typedef struct str {
		char* tracker;
		char* hash;
		char* filename;
	} DownloadArgs;
	
	DownloadArgs download_args;
	
	pthread_t thread;
	pthread_mutex_t stream_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	volatile bool streaming = false;
	int download;
}

/**
 * Define the InstallHTTPGateway method in httpgw.cpp.
 */
bool InstallHTTPGateway(struct event_base *evbase, swift::Address addr, uint32_t chunk_size, double *maxspeed);

namespace DownloadManager {
	/**
	 * Indicates that streaming should stop.
	 */
	volatile bool stopStreaming() {
		pthread_mutex_lock( &stream_mutex );
		streaming = false;
		bool result = streaming;
		pthread_mutex_unlock( &stream_mutex );
		return result;
	}
	
	/**
	 * Indicates that streaming should start.
	 */
	volatile bool startStreaming() {
		pthread_mutex_lock( &stream_mutex );
		streaming = true;
		bool result = streaming;
		pthread_mutex_unlock( &stream_mutex );
		return result;
	}
	
	/**
	 * Returns the current value of streaming.
	 */
	volatile bool readStreaming() {
		pthread_mutex_lock( &stream_mutex );
		bool result = streaming;
		pthread_mutex_unlock( &stream_mutex );
		return result;
	}
	
	/**
	 * Callback to check if a download is completed.
	 */
	void IsCompleteCallback(int fd, short event, void *arg) {
		if (swift::SeqComplete(download) != swift::Size(download)) {
			std::cout << "Percentage downloaded: " << floorf(((swift::Complete(download) * 10000.0) / swift::Size(download) * 1.0) + 0.5) / 100 << std::endl;
			evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
		}
		else
			event_base_loopexit(swift::Channel::evbase, NULL);
	}
	
	/*
	 * Callback needed to close streams.
	 */
	void CloseCallback(int fd, short event, void *arg) {
		std::cout << "Callback value of streaming: " << readStreaming() << std::endl;
		if (readStreaming()) {
			evtimer_add(&evclose, swift::tint2tv(TINT_SEC));
			std::cout << "Busy streaming..." << std::endl;
		
		} else {
			std::cerr << "Calling loopexit of HTTPgw." << std::endl;
			event_base_loopexit(swift::Channel::evbase, NULL);
			std::cout << "loopexit called." << std::endl;
		}
	}
	
	/**
	 * Download a file using libswift.
	 * @param str: Struct containing all necessary data for downloads.
	 */
	void* Download(void *str) {
		std::cout << "Entered download thread." << std::endl;
		
		// Change the directory to Downloads folder.
		chdir("/dtv/usb/sda1/Downloads");
		
		DownloadArgs *da = (DownloadArgs*) str;
		download         = 0;
		char* tracker    = da->tracker;
		char* hash       = da->hash;
		char* filename   = da->filename;
		
		swift::Address trackeraddr = swift::Address(tracker);
		swift::Sha1Hash root_hash = swift::Sha1Hash(true, hash);
		
		// Set the tracker.
		std::cout << "Setting the tracker..." << std::endl;
		swift::SetTracker(trackeraddr);
		
		// Download the file.
		download = swift::Open(filename, root_hash);
		
		if (download < 0 ) {
			std::cerr << "Could not download " << filename << "!" << std::endl;
		} else {
			// Assign callbacks to the event base.
			std::cout << "Now dispatching event base." << std::endl;
			evtimer_assign(&evcompl, swift::Channel::evbase, IsCompleteCallback, NULL);
			evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
			
			// Dispatch the event base to enter the swift loop.
			event_base_dispatch(swift::Channel::evbase);
			
			std::cout << "Download of " << filename << " completed." << std::endl;
			// Close the file.
			swift::Close(download);
		}
		
		std::cout << "Exiting download thread." << std::endl;
		// Exit the download thread when the download is finished.
		pthread_exit(NULL);
	}
	
	/**
	 * Stream a file using libswift.
	 * @param str: Struct containing all necessary data for streams.
	 */
	void* Stream(void *str) {
		std::cout << "Entered the stream thread." << std::endl;
		
		// Change the directory to Downloads folder.
		chdir("/dtv/usb/sda1/Downloads");
		
		DownloadArgs *da = (DownloadArgs*) str;
		char* tracker    = da->tracker;
		
		swift::Address trackeraddr = swift::Address(tracker);
		
		// Set the tracker.
		std::cout << "Setting the tracker..." << std::endl;
		swift::SetTracker(trackeraddr);
		
		evtimer_assign(&evclose, swift::Channel::evbase, CloseCallback, NULL);
		evtimer_add(&evclose, swift::tint2tv(TINT_SEC));
		
		std::cout << "Dispatching the event base." << std::endl;
		event_base_dispatch(swift::Channel::evbase);
		
		std::cout << "Exiting stream thread." << std::endl;
		// Exit the stream thread when the download is finished.
		pthread_exit(NULL);
	}
}

namespace HttpServer {
	/**
	 * Send the HTTP response.
	 */
	static void send_response(struct evhttp_request *req, struct evbuffer *buf,  const char *message) {
		
		// Add HTTP headers.
		evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain");
		
		// Add the plaintext message with this.
		evbuffer_add_printf(buf, message);
		
		// Send the final message.
		evhttp_send_reply(req, 200, "OK", buf);
		std::cout << "Sent the message." << std::endl;
	}
	
	/**
	 * Send the HTTP xml response.
	 */
	static void send_xml_response(struct evhttp_request *req, struct evbuffer *buf) {
		char speedstr[1024];
		sprintf(speedstr,"<DOWNLOADS><DOWNLOAD><NAME>bla!</NAME><DSPEED>%f</DSPEED><USPEED>%f</USPEED><PROGRESS>%f</PROGRESS></DOWNLOAD></DOWNLOADS>", 30, 20, 90.9);
		
		char contlenstr[1024];
		sprintf(contlenstr,"%i",strlen(speedstr));
		
		// Add HTTP headers.
		struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
		evhttp_add_header(headers, "Content-Type", "text/xml" );
		//evhttp_add_header(headers, "Content-Length", contlenstr );
		
		// Add the xml message with this.
		int ret = evbuffer_add(buf, speedstr, strlen(speedstr));
		if (ret < 0) {
			printf("ERROR!");
			return;
		}
		
		// Send the final message.
		evhttp_send_reply(req, 200, "OK", buf);
		std::cout << "Sent the message." << std::endl;
	}
	
	
	/**
	 * The HTTP GET request handler.
	 */
	static void request_handler(struct evhttp_request *req, void *arg) {
		const char *uri = evhttp_request_get_uri(req);
		const char *path;
		struct evbuffer *evb;
		struct evhttp_uri *decoded;
		
		std::cout << "Got a GET request for " << uri << std::endl;
		
		// Decode the URI.
		decoded = evhttp_uri_parse(uri);
		
		// This is the string we want to compare.
		path = evhttp_uri_get_path(decoded);
		
		// This holds the content we want to send.
		evb = evbuffer_new();
		
		if(strcmp(path, "/download") == 0) {
			DownloadManager::download_args.tracker  = "130.161.158.52:20000";
			DownloadManager::download_args.hash     = "012b5549e2622ea8bf3d694b4f55c959539ac848";
			DownloadManager::download_args.filename = "stream.mp4";
			
			// Spawn new thread to download the file requested.
			int rc = pthread_create(&DownloadManager::thread, NULL, DownloadManager::Download, (void *) &DownloadManager::download_args);
			
			if (rc) {
				std::cerr << "ERROR: failed to create download thread. Code: " << rc << "." << std::endl;
			}
			
			send_response(req, evb, "file:///dtv/usb/sda1/Downloads/stream.mp4");
			
		} else if (strcmp(path, "/close") == 0) {
			if (DownloadManager::readStreaming()) {
				std::cout << "Close request received." << std::endl;
				std::cout << "Close to: " << DownloadManager::stopStreaming() << std::endl;
				
				send_response(req, evb, "Streaming stopped");
				
			} else {
				std::cout << "No stream closed." << std::endl;
			}
			
		} else if (strcmp(path, "/stream") == 0) {
			if (!DownloadManager::readStreaming()) {
				std::cout << "Start with: " << DownloadManager::startStreaming() << std::endl;
				
				DownloadManager::download_args.tracker = "127.0.0.1:20000";
				int rc = pthread_create(&DownloadManager::thread, NULL, DownloadManager::Stream, (void *) &DownloadManager::download_args);
				
				if (rc) {
					std::cerr << "ERROR: failed to create stream thread. Code: " << rc << "." << std::endl;
				}	
			}
			send_response(req, evb, "http://130.161.159.107:15000/012b5549e2622ea8bf3d694b4f55c959539ac848");
			
		} else if (strcmp(path, "/getDownloads") == 0) {
			send_xml_response(req, evb);
		
		} else if (strcmp(path, "/alive") == 0) {
			send_response(req, evb, "I'm alive!");
			
		} else {
			std::cout << "Bad request: " << path << std::endl;
		}
		
		// Some garbage collecting.
		if (decoded)
			evhttp_uri_free(decoded);
		if (evb)
			evbuffer_free(evb);
	}
	
	void initializeHttpServer() {
		
		struct evhttp *http;
		struct evhttp_bound_socket *handle;
		
		// The port we want to bind.
		unsigned short port = 1337;
		// The event base.
		base = event_base_new();
		std::cout << "Created new event base." << std::endl;
		
		// Create a new evhttp object to handle requests.
		http = evhttp_new(base);
		std::cout << "Created new evhttp." << std::endl;
		
		// Assign the request handler to the web server.
		evhttp_set_gencb(http, request_handler, NULL);
		
		// Now we tell the evhttp what port to listen on.
		handle = evhttp_bind_socket_with_handle(http, "127.0.0.1", port);
		if (!handle) {
			std::cerr << "Couldn't bind to port " << (int)port << ". Exiting." << std::endl;
			return;
		} else {
			std::cout << "Successfully bound to port " << (int)port << "." << std::endl;
		}
		
		// Go into the main loop.
		event_base_dispatch(base);
	}
}

void initSwift() {
	swift::Channel::evbase = event_base_new();
	
	// Make a socket to listen to for swift.
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
			return;
		}
	}
	std::cout << "Listening on port " << swift::BoundAddress(sock).port() << "." << std::endl;
	
	// HTTP gateway address for swift to stream.
	swift::Address httpaddr    = swift::Address("127.0.0.1:15000");
	
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	
	std::cerr << "HTTP gateway creation returned " << res << "." << std::endl;
}

int main() {
	
	// Enable pthread use in libevent.
	evthread_use_pthreads();
	
	// Initialize swift http gateway.
	initSwift();
	
	// Set the initial value of streaming to false.
	DownloadManager::streaming = false;
	
	// Initialize the HTTP server.
	HttpServer::initializeHttpServer();
	
	return 0;
}

