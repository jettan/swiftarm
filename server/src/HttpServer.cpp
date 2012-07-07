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
	std::cout << "Sent the response." << std::endl;
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
	std::cout << "Sent the response." << std::endl;
}
/**
 * Handler for the /search request.
 */
static std::string searchRequest(std::string search_term) {
	std::string response = "-1";
	
	std::vector<std::string> tokenized = Settings::split(search_term, '_');
	
	std::string final_search_term;
	for(int i = 0; i < tokenized.size(); i++) {
		final_search_term = final_search_term + " " + tokenized.at(i);
	}
	
	SearchEngine::search(final_search_term);
	std::cout << "Search term is: " << final_search_term << std::endl;
	response = "Search request sent.";
	
	return response;
}

/**
 * Handler for the /add request.
 */
static std::string addRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		struct SearchEngine::result res = SearchEngine::getResultWithHash(hash);
		DownloadManager::add(new Download(res.tracker, res.hash, res.filename));
		response = "Download Added";
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
 * Handler for the /download request.
 */
static std::string downloadRequest(std::string hash) {
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
 * Handler for the /upload request.
 */
static std::string uploadRequest(std::string filename) {
	std::string response = "-1";
	
	try {
		filename  = Settings::replaceSubstring(filename, "%20", " ");
		std::cout << filename << std::endl;
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
 * Handler for the /stop request.
 */
static std::string stopRequest(std::string hash) {
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
 * Handler for the /remove request.
 */
static std::string removeRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		DownloadManager::removeFromDisk(hash);
		response = "Download removed from disk";
	} catch(FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /pause request.
 */
static std::string pauseRequest(std::string hash) {
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
 * Handler for the /resume request.
 */
static std::string resumeRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		DownloadManager::resumeDownload(hash);
		response = "Download Resumed";
	} catch (FileNotFoundException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	} catch (CannotResumeException e) {
		std::cout << "Exception Caught In HttpServer" << std::endl;
		std::cout << e.what() << std::endl;
		response = e.what();
	}
	
	return response;
}

/**
 * Handler for the /stream request.
 */
static std::string streamRequest(std::string hash) {
	std::string response = "-1";
	
	try {
		struct SearchEngine::result res = SearchEngine::getResultWithHash(hash);
		DownloadManager::startStream(res.tracker, res.hash);
		std::string address = "http://" + Settings::getIP() + ":17758/" + res.hash;
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
 * Handler for the /settings request.
 */
static std::string settingsRequest(std::vector<std::string> result) {
	
	double max_up            = strtod(result.at(1).c_str(), NULL);
	double max_down          = strtod(result.at(2).c_str(), NULL);
	std::string download_dir = result.at(3);
	download_dir             = Settings::replaceSubstring(download_dir, "%20", " ");
	DownloadManager::limitUpSpeeds(max_up);
	DownloadManager::limitDownSpeeds(max_down);
	Settings::setDownloadDirectory(download_dir);
	Settings::saveSettings(result);
	
	return "Received settings.";

}

/**
 * Handler for the /clear request.
 */
static std::string clearRequest() {
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
 * The HTTP GET request handler.
 * @param req: The request struct from libevent.
 * @param arg: Ignored argument.
 */
static void HttpServer::handleRequest(struct evhttp_request *req, void *arg) {
	const char *uri = evhttp_request_get_uri(req);
	const char *path;
	struct evbuffer *evb;
	struct evhttp_uri *decoded;
	
	std::cout << "---------------------" << std::endl;
	std::cout << "Got a GET request for " << uri << std::endl;
	
	// Decode the URI.
	decoded = evhttp_uri_parse(uri);
	
	// This is the string we want to compare.
	
	path = evhttp_uri_get_path(decoded);
	std::string path_str = path;
	
	// This holds the content we want to send.
	evb = evbuffer_new();
	
	// Vector with the request text
	std::vector<std::string> result = Settings::split(path_str, ':');
	
	if(result.size() == 1) {
		// Stops streaming.
		if (result.at(0).compare("/stopStream") == 0) {
			DownloadManager::stopStream();
			sendResponse(req, evb, "Not streaming anymore.");
			
		// Message will look like: "/clear".
		} else if (result.at(0).compare("/clear") == 0) {
			std::string response = clearRequest();
			sendResponse(req, evb, response.c_str());
			
		// Check to see whether the server is still alive.
		} else if (result.at(0).compare("/alive") == 0) {
			sendResponse(req, evb, "Alive");
			
		// Returns an XML file with the latest search results.
		} else if (result.at(0).compare("/results") == 0) {
			std::string results = SearchEngine::getResults();
			sendXMLResponse(results, req, evb);
			
		// Returns current download statistics to JavaScript.
		} else if (result.at(0).compare("/stats") == 0) {
			std::string msg = DownloadManager::buildXML();
			sendXMLResponse(msg, req, evb);
		
		// Returns current settings to JavaScript frontend
		} else if (result.at(0).compare("/settings") == 0) {
			std::ostringstream stream;
			stream << DownloadManager::getMaxUpSpeed()  << ":" << DownloadManager::getMaxDownSpeed() << ":" << Settings::getDownloadDirectory();
			std::string response = stream.str();
			sendResponse(req, evb, response.c_str());
			
		// Reply at bad requests:
		} else {
			std::cout << "Bad request: " << path << std::endl;
			sendResponse(req, evb, "Bad Request");
		}
	} else if (result.size() == 2) {
		// Message will look like: "/add:hash".
		if (result.at(1).size() == 40 && result.at(0).compare("/add") == 0) {
			std::string hash = result.at(1);
			std::string response = addRequest(hash);
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/download:hash".
		} else if (result.at(1).size() == 40 && result.at(0).compare("/download") == 0) {
			std::string hash = result.at(1);
			std::string response = downloadRequest(hash);
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/upload:filename"
		} else if (result.at(0).compare("/upload") == 0) {
			std::string filename = result.at(1);
			std::string response = uploadRequest(filename);
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/stop:roothash"
		} else if (result.at(1).size() == 40 && result.at(0).compare("/stop") == 0) {
			std::string hash = result.at(1);
			std::string response = stopRequest(hash);
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/remove:roothash"
		} else if (result.at(1).size() == 40 && result.at(0).compare("/remove") == 0) {
			std::string hash = result.at(1);
			std::string response = removeRequest(hash);
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/pause:hash"
		} else if (result.at(1).size() == 40 && result.at(0).compare("/pause") == 0) {
			std::string hash = result.at(1);
			std::string response = pauseRequest(hash);
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/resume:roothash"
		} else if (result.at(1).size() == 40 && result.at(0).compare("/resume") == 0) {
			std::string hash = result.at(1);
			std::string response = resumeRequest(hash);
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/search:searchterm"
		} else if (result.at(0).compare("/search") == 0) {
			std::string response = searchRequest(result.at(1));
			sendResponse(req, evb, response.c_str());
			
		// Message will look like: "/stream:hash"
		} else if (result.at(0).compare("/stream") == 0) {
			std::string hash = result.at(1);
			std::string response = streamRequest(hash);
			sendResponse(req, evb, response.c_str());
			
		// Reply at bad requests:
		} else {
			std::cout << "Bad request: " << path << std::endl;
			sendResponse(req, evb, "Bad Request");
		}
	} else if (result.size() == 4) {
		// Parse settings, message will look like: "/settings:downmax:upmax:downdir".
		if (result.at(0).compare("/settings") == 0) {
			std::string response = settingsRequest(result);
			sendResponse(req, evb, response.c_str());
		}
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
	std::cout << "---------------------" << std::endl;
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

