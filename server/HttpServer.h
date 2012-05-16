#ifndef _HTTPSERVER_H
#define _HTTPSERVER_H

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
#include <String>

class HttpServer {
	
	/**
	 * Send the HTTP response.
	 */
	static void send_response(struct evhttp_request *req, struct evbuffer *buf,  const char *message);
	
	
	/**
	 * The HTTP GET request handler.
	 */
	static void handle_request(struct evhttp_request *req, void *arg);
	
	int init(int argc, char **argv);
}
