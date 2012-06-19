#ifndef _HTTPSERVER_H
#define _HTTPSERVER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event-config.h>
#include <event2/thread.h>

#include "Exceptions.h"
#include "Download.h"
#include "Stream.h"
#include "DownloadManager.h"
#include "SearchEngine.h"
#include "swift.h"
#include "Utils.h"

namespace HttpServer {
	
	static void sendXMLResponse(std::string msg, struct evhttp_request *req, struct evbuffer *buf);
	static void sendResponse(struct evhttp_request *req, struct evbuffer *buf,  const char *message);
	static void handleRequest(struct evhttp_request *req, void *arg);
	
	static std::string addRequest(std::string hash);
	static std::string downloadRequest(std::string hash);
	static std::string uploadRequest(std::string filename);
	static std::string stopRequest(std::string hash);
	static std::string removeRequest(std::string hash);
	static std::string pauseRequest(std::string hash);
	static std::string resumeRequest(std::string hash);
	static std::string streamRequest(std::string hash);
	static std::string settingsRequest(std::vector<std::string> settings);
	static std::string clearRequest();
	
	int init();
	
}

#endif
