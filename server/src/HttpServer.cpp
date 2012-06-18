#include "HttpServer.h"

namespace HttpServer {
	struct event_base *base;
}


void HttpServer::setIP(std::string ip) {
	
	struct sockaddr_in sa;
	
	if (inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1) {
		ip_address = ip;
	} else {
		InvalidIPException *e = new InvalidIPException();
		throw *e;
	}
}

std::string HttpServer::getIP() {
	
	return ip_address;
}

/**
 * Sends the HTTP XML response.
 * @param msg: The xml message to be sent.
 * @param req: The received HTTP request.
 * @param buf: Buffer used to send the reply.
 */
static void HttpServer::sendXMLResponse(std::string msg, struct evhttp_request *req, struct evbuffer *buf) {
	// Add HTTP headers.
	struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
	evhttp_add_header(headers, "Content-Type", "text/xml" );
	
	// Add the XML message.
	int ret = evbuffer_add(buf, msg.c_str(), msg.length());
	if (ret < 0) {
		printf("ERROR!");
		return;
	}
	
	// Send the final message.
	evhttp_send_reply(req, 200, "OK", buf);
	std::cout << "Sent the message." << std::endl;
}

/**
 * Sends the HTTP response.
 * @param req: The received HTTP request.
 * @param buf: Buffer used to send the reply.
 * @param message: The message to be sent.
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

/**
 * Used to tokenize strings.
 */
std::vector<std::string> &split(const std::string &str, char delim, std::vector<std::string> &elems) {
	std::stringstream stream(str);
	std::string item;
	
	while(std::getline(stream, item, delim)) {
		elems.push_back(item);
	}
	
	return elems;
}

/**
 * Used to tokenize strings.
 */
std::vector<std::string> split(const std::string &str, char delim) {
	std::vector<std::string> elems;
	return split(str, delim, elems);
}

/**
 * The HTTP GET request handler.
 * @param req: The request struct from libevent.
 * @param arg: Ignored argument.
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
	std::string path_str = path;
	
	// This holds the content we want to send.
	evb = evbuffer_new();
	
	// Message will look like: "/add:hash".
	if (path_str.size() == 45 && path_str.substr(0, 4).compare("/add") == 0  && path_str.at(4) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				struct SearchEngine::result res = SearchEngine::getResultWithHash(hash);
				DownloadManager::add(new Download(res.tracker, res.hash, res.filename));
				sendResponse(req, evb, "Download Added");
			} catch(std::exception e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, e.what());
			}
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/download:roothash"
	} else if (path_str.size() == 50 && path_str.substr(0, 9).compare("/download") == 0  && path_str.at(9) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				DownloadManager::switchDownload(hash);
				sendResponse(req, evb, "Download Started");
			} catch(FileNotFoundException e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, "-1");
			} 
		} else {
				sendResponse(req, evb, "-1");
			}
	// Message will look like: "/stop:roothash"
	} else if (path_str.size() == 46 && path_str.substr(0, 5).compare("/stop") == 0  && path_str.at(5) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				DownloadManager::removeFromList(hash);
				sendResponse(req, evb, "Download removed from list");
			} catch(FileNotFoundException e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, "-1");
			}
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/stop:roothash"
	} else if (path_str.size() == 48 && path_str.substr(0, 7).compare("/remove") == 0 && path_str.at(7) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				DownloadManager::removeFromDisk(hash);
				sendResponse(req, evb, "Download removed from list");
			} catch(FileNotFoundException e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, "-1");
			}
		} else {
			sendResponse(req, evb, "-1");
		}
	} else if (path_str.size() == 47 && path_str.substr(0, 6).compare("/pause") == 0  && path_str.at(6) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				DownloadManager::pauseDownload(hash);
				sendResponse(req, evb, "Download Paused");
			} catch(FileNotFoundException e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, "-1");
			}
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/resume:roothash"
	} else if (path_str.size() == 48 && path_str.substr(0, 7).compare("/resume") == 0  && path_str.at(7) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				DownloadManager::resumeDownload(hash);
				sendResponse(req, evb, "Download Resumed");
			} catch(FileNotFoundException e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, "-1");
			}
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/remove:roothash"
	} else if (path_str.size() == 48 && path_str.substr(0, 7).compare("/remove") == 0  && path_str.at(7) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				DownloadManager::removeFromDisk(hash);
				sendResponse(req, evb, "Download removed from disk");
			} catch(FileNotFoundException e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, "-1");
			}
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/search:searchterm"
	} else if (path_str.size() > 8 && path_str.substr(0, 7).compare("/search") == 0 && path_str.at(7) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string search_term = result.at(1);
			std::string result = SearchEngine::search(search_term);
			sendXMLResponse(result, req, evb);
		} else {
			sendResponse(req, evb, "-1");
		}
	// Returns current download statistics to JavaScript
	} else if (strcmp(path, "/stats") == 0) {
		std::string msg = DownloadManager::buildXML();
		sendXMLResponse(msg, req, evb);
		
	// Message will look like: "/stream:filename"
	} else if (path_str.size() >= 7 && path_str.substr(0,7).compare("/stream") == 0 && path_str.at(7) == ':') {
		std::vector<std::string> result = split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			
			try {
				struct SearchEngine::result res = SearchEngine::getResultWithHash(hash);
				DownloadManager::startStream(res.tracker);
				std::string address = getIP() + ":15000/" + res.hash;
				std::cout << address << std::endl;
				sendResponse(req, evb, address.c_str());
			} catch(FileNotFoundException e) {
				std::cout << "Exception Caught In HttpServer" << std::endl;
				std::cout << e.what() << std::endl;
				sendResponse(req, evb, e.what());
			}
		} else {
			sendResponse(req, evb, "-1");
		}
	// Stops streaming.
	} else if (strcmp(path, "/stopStream") == 0) {
		DownloadManager::stopStream();
		sendResponse(req, evb, "Not streaming anymore.");
		
	// Parse settings, message will look like: "/settings:downmax:upmax:downdir".
	} else if (path_str.size() >= 27 && path_str.substr(0,9).compare("/settings") == 0 && path_str.at(9) == ':') {
		std::vector<std::string> result = split(path, ':');
		
		if (result.size() == 4) {
			double max_down          = strtod(result.at(1).c_str(), NULL);
			double max_up            = strtod(result.at(2).c_str(), NULL);
			std::string download_dir = result.at(3);
			
			DownloadManager::setDownloadDirectory(download_dir);
			DownloadManager::setMaxUpSpeed(max_up);
			DownloadManager::setMaxDownSpeed(max_down);
		}
		
		sendResponse(req, evb, "Received settings.");
		
	// Message will look like: "/clear"
	} else if (strcmp(path, "/clear") == 0) {
		try {
			DownloadManager::clearList();
			sendResponse(req, evb, "Download list cleared");
		} catch(FileNotFoundException e) {
			std::cout << "Exception Caught In HttpServer" << std::endl;
			std::cout << e.what() << std::endl;
			sendResponse(req, evb, "-1");
		}
		
	// Check to see whether the server is still alive.
	} else if (strcmp(path, "/alive") == 0) {
		sendResponse(req, evb, "Alive");
		
	// Reply at bad requests:
	} else {
		std::cout << "Bad request: " << path << std::endl;
		sendResponse(req, evb, "Bad Request");
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
 * Initialises the web server.
 */
int HttpServer::init() {
	
	
	// Test to see if we can get the ip address of the local machine
	struct ifaddrs * ifAddrStruct = NULL;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;
	setIP(DEFAULT_IP);
	
	getifaddrs(&ifAddrStruct);
	
	std::cout << "We can get an IP address now!" << std::endl;
	
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		
		if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
			
			// is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			if(strcmp(ifa->ifa_name, "eth0") == 0 || strcmp(ifa->ifa_name, "eth1") == 0 || strcmp(ifa->ifa_name, "wlan0") == 0) {
				printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
				std::string ip(addressBuffer);
				setIP(ip);
			}
		}
	}
	if (ifAddrStruct!=NULL) {
		freeifaddrs(ifAddrStruct);
	}
	
	std::cout << "Set the IP address to: " << getIP() << std::endl;
	
	// end ip test
	
	
	
	
	
	
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
	handle = evhttp_bind_socket_with_handle(http, getIP().c_str(), port);
	//handle = evhttp_bind_socket_with_handle(http, "130.161.159.107", port);
	//handle = evhttp_bind_socket_with_handle(http, "127.0.0.1", port);
	//handle = evhttp_bind_socket_with_handle(http, "130.161.158.52", port);
	
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

