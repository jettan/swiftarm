#include "../include/HttpServer.h"

namespace HttpServer {
	struct event_base *base;
}

/**
 * Send the HTTP XML response.
 */
static void HttpServer::sendXMLResponse(struct evhttp_request *req, struct evbuffer *buf) {
	char speedstr[1024];
	sprintf(speedstr,"<DOWNLOADS><DOWNLOAD><NAME>bla!</NAME><DSPEED>%f</DSPEED><USPEED>%f</USPEED><PROGRESS>%f</PROGRESS></DOWNLOAD></DOWNLOADS>", 30.0, 20.0, 90.9);
	
	// Add HTTP headers.
	struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
	evhttp_add_header(headers, "Content-Type", "text/xml" );
	
	// Add the XML message.
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
 * Send the HTTP response.
 */
static void HttpServer::sendResponse(struct evhttp_request *req, struct evbuffer *buf,  const char *message) {
	
	// Add HTTP headers.
	evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain");
	
	// Add the plaintext message with this.
	evbuffer_add_printf(buf,"%s", message);
	
	// Send the final message.
	evhttp_send_reply(req, 200, "OK", buf);
	std::cout << "Sent the message." << std::endl;
}

void* startThread(void *arg) {
	std::cout << "Entered thread" << std::endl;
	Download *download = (Download*) arg;
	download->start();
	std::cout << "Exiting thread" << std::endl;
	pthread_exit(NULL);
}

/**
 * The HTTP GET request handler.
 */
static void HttpServer::handleRequest(struct evhttp_request *req, void *arg) {
	
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
		//TODO: Parse the http request.
		
		std::string tracker     = "130.161.158.52:20000";
		std::string root_hash   = "012b5549e2622ea8bf3d694b4f55c959539ac848";
		std::string name        = "bla.mp4";
		Download* test          = new Download(tracker, root_hash, name);
		
		//Wait until download object is actually created before starting the thread.
		sleep(1);
		
		//Start thread to start download loop.
		pthread_t thread;
		int result_code = pthread_create(&thread, NULL, startThread, (void*) test);
		
		//TODO: Construct the path where the file will be downloaded.
		char response[] = "file:///dtv/usb/sda1/Downloads/bla.mp4";
		
		sendResponse(req, evb, response);
		
	} else if (strcmp(path, "/getDownloads") == 0) {
		sendXMLResponse(req, evb);
		
	} else if (strcmp(path, "/stream") == 0) {
		//TODO: Parse the stream request.
		//TODO: Set the download properties (tracker, etc).
		//TODO: Call downloadmanager to start streaming.
		
		char tracker[]   = "130.161.158.52:20000";
		DownloadManager::startStreaming(tracker);
		
		//TODO: Construct url from which stream can be read from.
		sendResponse(req, evb, "http://130.161.159.107:15000/012b5549e2622ea8bf3d694b4f55c959539ac848");
		
	} else if (strcmp(path, "/alive") == 0) {
		sendResponse(req, evb, "Alive");
	} else if (strcmp(path, "/close") == 0) {
		DownloadManager::stopStreaming();
		sendResponse(req, evb, "Not streaming anymore.");
	} else {
		std::cout << "Bad request: " << path << std::endl;
	}
	
	// Some garbage collecting.
	if (decoded) {
		evhttp_uri_free(decoded);
		std::cout << "Cleaned decoded garbage" << std::endl;
	}
	if (evb) {
		evbuffer_free(evb);
		std::cout << "Cleaned evb garbage" << std::endl;
	}
}

/**
 * Initialize the web server.
 */
int HttpServer::init() {
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
	evhttp_set_gencb(http, handleRequest, NULL);
	
	// Now we tell the evhttp what port to listen on.
	//handle = evhttp_bind_socket_with_handle(http, "130.161.158.52", port);
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

