#include "HttpServer.h"

namespace HttpServer {
	struct event_base *base;
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
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			std::string response = addRequest(hash);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	} else if (strcmp(path, "/dispersy") == 0) {
		SearchEngine::searchDispersy();
		sendResponse(req, evb, "Called dispersy.");
	// Message will look like: "/download:hash"
	} else if (path_str.size() == 50 && path_str.substr(0, 9).compare("/download") == 0  && path_str.at(9) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			std::string response = downloadRequest(hash);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/upload:filename"
	} else if (path_str.size() > 7 && path_str.substr(0, 7).compare("/upload") == 0  && path_str.at(7) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string filename = result.at(1);
			std::string response = uploadRequest(filename);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/stop:roothash"
	} else if (path_str.size() == 46 && path_str.substr(0, 5).compare("/stop") == 0  && path_str.at(5) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			std::string response = stopRequest(hash);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/remove:roothash"
	} else if (path_str.size() == 48 && path_str.substr(0, 7).compare("/remove") == 0 && path_str.at(7) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			std::string response = removeRequest(hash);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/pause:hash"
	} else if (path_str.size() == 47 && path_str.substr(0, 6).compare("/pause") == 0  && path_str.at(6) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			std::string response = pauseRequest(hash);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/resume:roothash"
	} else if (path_str.size() == 48 && path_str.substr(0, 7).compare("/resume") == 0  && path_str.at(7) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			std::string response = resumeRequest(hash);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	// Message will look like: "/search:searchterm"
	} else if (path_str.size() > 8 && path_str.substr(0, 7).compare("/search") == 0 && path_str.at(7) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
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
		
	// Message will look like: "/stream:hash"
	} else if (path_str.size() >= 7 && path_str.substr(0,7).compare("/stream") == 0 && path_str.at(7) == ':') {
		std::vector<std::string> result = Settings::split(path_str, ':');
		
		if (result.size() == 2) {
			std::string hash = result.at(1);
			std::string response = streamRequest(hash);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "-1");
		}
	// Stops streaming.
	} else if (strcmp(path, "/stopStream") == 0) {
		DownloadManager::stopStream();
		sendResponse(req, evb, "Not streaming anymore.");
		
	// Parse settings, message will look like: "/settings:downmax:upmax:downdir".
	} else if (path_str.size() >= 27 && path_str.substr(0,9).compare("/settings") == 0 && path_str.at(9) == ':') {
		std::vector<std::string> result = Settings::split(path, ':');
		
		if (result.size() == 4) {
			std::string response = settingsRequest(result);
			sendResponse(req, evb, response.c_str());
		} else {
			sendResponse(req, evb, "Invalid settings format");
		}
		
	// Message will look like: "/clear"
	} else if (strcmp(path, "/clear") == 0) {
		
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
 * Handler for the /add request
 */
static std::string HttpServer::addRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		struct SearchEngine::result res = SearchEngine::getResultWithHash(hash);
		DownloadManager::add(new Download(res.tracker, res.hash, res.filename));
		response = "Download Added";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /download request
 */
static std::string HttpServer::downloadRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		if(DownloadManager::getIndexFromHash(hash) > -1) {
			DownloadManager::switchDownload(hash);
			response = "Download Started";
		}
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	} catch(AlreadyDownloadingException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /upload request
 */
static std::string HttpServer::uploadRequest(std::string filename) {
	std::string response = "-1";
	
	try {
		DownloadManager::upload(filename);
		response = "Upload Started";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /stop request
 */
static std::string HttpServer::stopRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		DownloadManager::removeFromList(hash);
		response = "Download removed from list";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /remove request
 */
static std::string HttpServer::removeRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		DownloadManager::removeFromDisk(hash);
		response = "Download removed from list";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /pause request
 */
static std::string HttpServer::pauseRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		DownloadManager::pauseDownload(hash);
		response = "Download Paused";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /resume request
 */
static std::string HttpServer::resumeRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		DownloadManager::resumeDownload(hash);
		response = "Download Resumed";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /stream request
 */
static std::string HttpServer::streamRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		struct SearchEngine::result res = SearchEngine::getResultWithHash(hash);
		DownloadManager::startStream(res.tracker);
		std::string address = Settings::getIP() + ":15000/" + res.hash;
		std::cout << address << std::endl;
		response = address.c_str();
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /settings request
 */
static std::string HttpServer::settingsRequest(std::vector<std::string> result) {
	
	double max_down          = strtod(result.at(1).c_str(), NULL);
	double max_up            = strtod(result.at(2).c_str(), NULL);
	std::string download_dir = result.at(3);
	
	DownloadManager::limitUpSpeeds(max_up);
	DownloadManager::limitDownSpeeds(max_down);
	Settings::setDownloadDirectory(download_dir);
	Settings::saveSettings(result);
	
	return "Received settings.";

}

/**
 * Handler for the /clear request
 */
static std::string HttpServer::clearRequest() {
	std::string response = "-1";
	
	try {
		DownloadManager::clearList();
		response = "Download list cleared";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Initialises the web server.
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
	handle = evhttp_bind_socket_with_handle(http, Settings::getIP().c_str(), port);
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

