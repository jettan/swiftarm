#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

#include <iostream>
#include <ctime>
#include <pthread>

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

#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOUR (SECONDS_PER_MINUTE * SECONDS_PER_MINUTE)
#define SECONDS_PER_DAY (SECONDS_PER_HOUR * 24)

class Download {
	
	enum Status {
		READY,
		PAUSED,
		DOWNLOADING,
		UPLOADING,
	};
	
	protected:
		struct _event_base *base;
		struct _event evcompl;
		struct _event evclose;
		
		pthread_t _thread;										/// Thread to start download.
		pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;		/// Mutex to prevent download thread and main thread from accessing same data at the same time.
			
		int _id = -1;					/// Download id.
		double _size					/// Download size.
		Status _status;					/// Current status of the download.
		
		struct downloadProps {
			char *filename;				/// Name of the download.
			char *tracker;				/// Trackers seeding this download.
			char *root_hash;			/// Root hash needed to start swift download.
		};
		
		downloadProps _properties;		/// Properties to pass to the thread. Used to initiate swift download.
		
		/// Struct for holding time data.
		struct time {
			int days;
			int hours;
			int minutes;
			int seconds;
		};
		
		struct downloadStats {
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
		
		int getID();
		double getSize();
		Status getStatus();
		
		struct downloadStats getStatistics();
		struct downloadProps getProperties();
		
		void setDownloadSpeed(double speed);
		void setUploadSpeed(double speed);
		void calculateRatio(double ratio);
		void setProgress(double percentage);
		void setUploadAmount(double amount);
		
		void setSeeders(int amount);
		void setPeers(int amount);
		
		void calculateEstimatedTime();
		
		void setStatus(Status status);
		
		/**
		 * Constructor.
		 */
		Download(char *tracker, char *root_hash, char *filename) {
			_properties.tracker     = tracker;
			_properties.root_hash   = root_hash;
			_properties.status      = Status.READY;
		}
		
		/**
		 * Destructor.
		 */
		~Download() {}
}

#endif
