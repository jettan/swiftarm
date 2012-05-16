#include "HttpServer.h"


/**
 * Define the InstallHTTPGateway method in httpgw.cpp.
 */
bool InstallHTTPGateway(struct event_base *evbase, swift::Address addr, uint32_t chunk_size, double *maxspeed);

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
 * The HTTP GET request handler.
 */
static void handle_request(struct evhttp_request *req, void *arg) {
	
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
		download_args.tracker  = "130.161.158.52:20000";
		download_args.hash     = "ed29d19bc8ea69dfb5910e7e20247ee7e002f321";
		download_args.filename = "stream.mp4";
		
		// Spawn new thread to download the file requested.
		rc = pthread_create(&thread, NULL, Download, (void *) &download_args);
		
		if (rc) {
			std::cerr << "ERROR: failed to create download thread. Code: " << rc << "." << std::endl;
		}
		
		send_response(req, evb, "file:///tmp/stream.mp4");
		
	} else if (strcmp(path, "/close") == 0) {
		if (readStreaming()) {
			std::cout << "Close request received." << std::endl;
			std::cout << "Close to: " << stopStreaming() << std::endl;
			
			send_response(req, evb, "Streaming stopped");
			
		} else {
			std::cout << "No stream closed." << std::endl;
		}
		
	} else if (strcmp(path, "/stream") == 0) {
		if (!readStreaming()) {
			std::cout << "Start with: " << startStreaming() << std::endl;
			
			download_args.tracker = "130.161.158.52:20000";
			rc = pthread_create(&thread, NULL, Stream, (void *) &download_args);
			
			if (rc) {
				std::cerr << "ERROR: failed to create stream thread. Code: " << rc << "." << std::endl;
			}	
		}
		send_response(req, evb, "http://130.161.159.107:15000/ed29d19bc8ea69dfb5910e7e20247ee7e002f321");
		
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

int init(int argc, char **argv) {
	
	// Enable pthread use in libevent.
	evthread_use_pthreads();
	
	struct evhttp *http;
	struct evhttp_bound_socket *handle;
	
	// The port we want to bind.
	unsigned short port = 1337;
	
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
			return 1;
		}
	}
	std::cout << "Listening on port " << swift::BoundAddress(sock).port() << "." << std::endl;
	
	
	// HTTP gateway address for swift to stream.
	swift::Address httpaddr    = swift::Address("130.161.159.107:15000");
	
	// swift::Address httpaddr    = swift::Address("130.161.158.52:15000");
	double maxspeed[2] = {DBL_MAX, DBL_MAX};
	
	// Install the HTTP gateway to stream.
	bool res = InstallHTTPGateway(swift::Channel::evbase, httpaddr, SWIFT_DEFAULT_CHUNK_SIZE, maxspeed);
	
	std::cerr << "HTTP gateway creation returned " << res << "." << std::endl;
	
	// Set the initial value of streaming to false.
	streaming = false;
	rc        = 0;
	
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		return (1);
	}
	
	// The event base.
	base = event_base_new();
	std::cout << "Created new event base." << std::endl;
	
	// Create a new evhttp object to handle requests.
	http = evhttp_new(base);
	std::cout << "Created new evhttp." << std::endl;
	
	// Assign the request handler to the web server.
	evhttp_set_gencb(http, request_handler, NULL);
	
	// Now we tell the evhttp what port to listen on.
	// handle = evhttp_bind_socket_with_handle(http, "127.0.0.1", port);
	handle = evhttp_bind_socket_with_handle(http, "130.161.159.107", port);
	if (!handle) {
		std::cerr << "Couldn't bind to port " << (int)port << ". Exiting." << std::endl;
		return 1;
	} else {
		std::cout << "Successfully bound to port " << (int)port << "." << std::endl;
	}
	
	// Go into the main loop.
	event_base_dispatch(base);
	
	return 0;
}

