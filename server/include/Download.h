#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOUR (SECONDS_PER_MINUTE * SECONDS_PER_MINUTE)
#define SECONDS_PER_DAY (SECONDS_PER_HOUR * 24)

#include <iostream>
#include <ctime>
#include <string>

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

#include "../include/swift.h"

enum Status {
	READY,
	PAUSED,
	DOWNLOADING,
	UPLOADING,
};

class Download {
	protected:
		struct event _evcompl;
		
		pthread_t _thread;			/// Thread to start download.
		pthread_mutex_t _mutex;		/// Mutex to prevent download thread and main thread from accessing same data at the same time.
		
		double _size;				/// Download size.
		volatile int _status;		/// Current status of the download.
		
		std::string _filename;			/// Name of the download.
		std::string _tracker;			/// Trackers seeding this download.
		std::string _root_hash;			/// Root hash needed to start swift download.
		
		/// Struct for holding time data.
		struct time {
			int days;
			int hours;
			int minutes;
			int seconds;
		};
		
		struct downloadStats {
			int  id;					/// Download id needed to check the stats.
			double download_speed;		/// Current download speed.
			double upload_speed;		/// Current upload speed.
			double ratio;				/// Download speed divided by upload speed
			double download_percentage; /// Download progress in percentage.
			double upload_amount;		/// Uploaded amount in bytes.
			
			int seeders;				/// Number of seeders uploading this file.
			int peers;					/// Number of peers connected to us for this file.
			
			struct time estimated;		/// Estimated time left for download to finish.
		};
		
		
		volatile downloadStats _stats;  /// Struct holding the statistics of the download.
		
	public:
		void retry();
		void start();
		void pause();
		void resume();
		
		const int getID();
		const int getStatus();
		std::string getTrackerAddress();
		std::string getFilename();
		std::string getRootHash();
		const double getSize();
		struct event *getEvent();
		
		//void isCompleteCallback(int fd, short event, void* arg);
		
		struct downloadStats getStatistics();
		
		void setDownloadSpeed(double speed);
		void setUploadSpeed(double speed);
		void calculateRatio();
		void setProgress(double percentage);
		void setUploadAmount(double amount);
		
		void setSeeders(int amount);
		void setPeers(int amount);
		
		void calculateEstimatedTime();
		
		void setID(int id);
		void setStatus(int status);
		
		/**
		 * Constructor.
		 */
		Download(std::string tracker, std::string root_hash, std::string filename) {
			_stats.id    = -1;
			_tracker     = tracker;
			_root_hash   = root_hash;
			_filename    = filename;
			int rc = pthread_mutex_init(&_mutex, NULL);
			setStatus(READY);
		}
		
		/**
		 * Destructor.
		 */
		~Download() {
		}
};

#endif
