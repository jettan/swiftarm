#ifndef _STREAM_H
#define _STREAM_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include "Utils.h"
#include "swift.h"

class Stream {
	protected:
		std::string _tracker;		/// Trackers seeding this stream.
		pthread_mutex_t _mutex;		/// Mutex to prevent download thread and main thread from accessing same data at the same time.
		struct event _evclose;		/// Event used by libevent to stop a stream.
		volatile bool _streaming;	/// Boolean to determine whether a stream is opened or not.

	private:
		static Stream *_instance;	/// Singleton instance of Stream class.
		
		/**
		 * Constructor, made private to implement Singleton pattern.
		 */
		Stream() {
			init();
		}
		
	public:
		static Stream *getInstance();
		
		/**
		 * Destructor.
		 */
		~Stream() {}
		
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
		
};

#endif
