#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

#include <iostream>
#include <ctime>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "swift.h"

#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOUR (SECONDS_PER_MINUTE * SECONDS_PER_MINUTE)
#define SECONDS_PER_DAY (SECONDS_PER_HOUR * 24)

enum Status {
	READY,
	PAUSED,
	DOWNLOADING,
	UPLOADING,
	STOPPED,
	SWITCHING,
};

class Download {
	protected:
		
		pthread_mutex_t _mutex;			/// Mutex to prevent download thread and main thread from accessing same data at the same time.
		
		double _size;					/// Download size.
		volatile int _status;			/// Current status of the download.
		
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
		
		/// Struct for holding download statistics.
		struct downloadStats {
			int  id;					/// Download id needed to check the stats.
			double download_speed;		/// Current download speed.
			double upload_speed;		/// Current upload speed.
			double ratio;				/// Download speed divided by upload speed
			double download_percentage; /// Download progress in percentage.
			double upload_amount;		/// Uploaded amount in bytes.
			double download_amount;		/// Downloaded amount in bytes.
			
			int seeders;				/// Number of seeders uploading this file.
			int peers;					/// Number of peers connected to us for this file.
			
			struct time estimated;		/// Estimated time left for download to finish.
		};
		
		downloadStats _stats;			/// Struct holding the statistics of the download.
		
	public:
		void retry();
		void start();
		void stop();
		void pause();
		void resume();
		
		const int getID();
		const int getStatus();
		std::string getTrackerAddress();
		std::string getFilename();
		std::string getRootHash();
		const double getSize();
		
		struct downloadStats getStatistics();
		
		void setDownloadSpeed(double speed);
		void setUploadSpeed(double speed);
		void calculateRatio();
		void setProgress(double percentage);
		void setUploadAmount(double amount);
		void setDownloadAmount(double amount);
		
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
			int rc       = pthread_mutex_init(&_mutex, NULL);
			setStatus(READY);
		}
		
		/**
		 * Destructor.
		 */
		~Download() {
		}
};

#endif
