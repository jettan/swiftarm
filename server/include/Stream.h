#ifndef _STREAM_H
#define _STREAM_H

#include <iostream>
#include <string>
#include <ctime>

#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <float.h>
#include <pthread.h>
#include <math.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event-config.h>
#include <event2/thread.h>

#include "swift.h"

class Stream {
	protected:
		std::string _tracker;		/// Trackers seeding this stream.
		pthread_mutex_t _mutex;		/// Mutex to prevent download thread and main thread from accessing same data at the same time.
		struct event _evclose;
		volatile bool streaming;
		
	public:
		
		void start();
		void stop();
		void init();
		void setTracker(std::string);
		
		void beginStreaming();
		bool readStreaming();
		
		const int getStatus();
		std::string getTrackerAddress();
		struct event *getEvent();
		
		void setStatus(int status);
		
		Stream() {
			init();
		}
		
		~Stream() {}
};

#endif
