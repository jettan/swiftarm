#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

#include <iostream>
#include <ctime>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <float.h>

#include "swift.h"
#include "Settings.h"

#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOUR (SECONDS_PER_MINUTE * SECONDS_PER_MINUTE)
#define SECONDS_PER_DAY (SECONDS_PER_HOUR * 24)

/// All states a Download can be in.
enum Status {
	READY,
	PAUSED,
	DOWNLOADING,
	UPLOADING,
	STOPPED,
	SWITCHING,
	STATUS_SIZE,
};

	
class Download {
	protected:
		/// Mutex to prevent download thread and main thread from accessing same data at the same time.
		pthread_mutex_t _mutex;
		
		/// Mutex to prevent download thread and main thread from accessing _transfer at the same time.
		pthread_mutex_t _transfer_mutex;
		
		/// Current status of the download.
		volatile int _status;
		
		/// Name of the download.
		std::string _filename;
		
		/// Trackers seeding this download.
		std::string _tracker;
		
		/// Root hash needed to start swift download.
		std::string _root_hash;
		
		/// Struct for holding time data.
		struct time {
			int days;
			int hours;
			int minutes;
			int seconds;
		};
		
		/// Struct for holding download statistics.
		struct downloadStats {
			/// Download id needed to check the stats.
			int  id;
		
			/// Current download speed in kb/s.
			double download_speed;
			
			/// Current upload speed in kb/s.
			double upload_speed;
			
			/// Download progress in percentage.
			double download_percentage;
			
			/// Number of seeders uploading this file.
			int seeders;
			
			/// Number of peers connected to us for this file.
			int peers;
			
			/// Estimated time left for download to finish.
			struct time estimated;
			};
		
		/// Struct holding the statistics of the download.
		downloadStats _stats;
		
		/// Swift FileTransfer needed to access certain data about swift downloads.
		swift::FileTransfer *_transfer;
		
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
		
		struct downloadStats getStatistics();
		
		void calculateSpeeds();
		void calculatePeers();
		void setDownloadSpeed(double speed);
		void setUploadSpeed(double speed);
		void setProgress(double percentage);
		bool isComplete();
		
		void setSeeders(int amount);
		void setPeers(int amount);
		
		void calculateEstimatedTime();
		
		void setID(int id);
		void setStatus(int status);
		void limitDownSpeed(double speed);
		void limitUpSpeed(double speed);
		
		/**
		 * Constructor.
		 */
		Download(std::string tracker, std::string root_hash, std::string filename) {
			_stats.id    = -1;
			_tracker     = tracker;
			_root_hash   = root_hash;
			_filename    = filename;
			
			pthread_mutex_init(&_mutex, NULL);
			pthread_mutex_init(&_transfer_mutex, NULL);
			
			setDownloadSpeed(0.0);
			setUploadSpeed(0.0);
			setSeeders(0);
			setPeers(0);
			setProgress(0.0);
			setStatus(READY);
		}
		
		/**
		 * Destructor.
		 */
		~Download() {
		}
};

#endif
