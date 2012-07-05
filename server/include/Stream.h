#ifndef _STREAM_H
#define _STREAM_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include "Settings.h"
#include "swift.h"

/**
 * Singleton class for downloading and binding streams.
 * Makes use of swift methods.
 */
class Stream {
	protected:
		/// Trackers seeding this stream.
		std::string _tracker;		
		
		/// Root hash of the stream.
		std::string _hash;			
		
		/// Mutex to prevent download thread and main thread from accessing same data at the same time.
		pthread_mutex_t _mutex;	
		
		/// Event used by libevent to stop a stream.
		struct event _evclose;
		
		/// Boolean to determine whether a stream is opened or not.
		volatile bool _streaming;

	private:
		/// Singleton instance of Stream class.
		static Stream *_instance;
		
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
		void setTracker(std::string tracker);
		void setRoothash(std::string hash);
		
		void beginStreaming();
		bool readStreaming();
		
		const int getStatus();
		std::string getTrackerAddress();
		std::string getRootHash();
		struct event *getEvent();
		
		void setStatus(int status);
		
};

#endif
