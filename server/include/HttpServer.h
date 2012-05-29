#ifndef _HTTPSERVER_H
#define _HTTPSERVER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <math.h>
#include <float.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event-config.h>
#include <event2/thread.h>

#include "Download.h"
#include "DownloadManager.h"
#include "swift.h"

namespace HttpServer {
		
		static void sendXMLResponse(struct evhttp_request *req, struct evbuffer *buf);
		static void sendResponse(struct evhttp_request *req, struct evbuffer *buf,  const char *message);
		static void handleRequest(struct evhttp_request *req, void *arg);
		int init();
	
}

#endif
