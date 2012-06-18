#ifndef _DOWNLOADMANAGER_H
#define _DOWNLOADMANAGER_H

#include <vector>
#include <string>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <float.h>

#include "Stream.h"
#include "Download.h"
#include "swift.h"
#include "ticpp.h"
#include "Exceptions.h"
#include "Utils.h"

#define UNLIMITED_SPEED 0

namespace DownloadManager {
	
	/// Struct used to hold data amounts with their corresponding units.
	struct Amount {
		double amount;
		std::string unit;
	};
	
	static std::vector<Download> downloads;			/// Vector containing all downloads.
	static struct event evcompl;					/// Event needed to start libevent loop.
	static Download *active_download;				/// The currently active download.
	static pthread_t streaming_thread;				/// Thread to start streams.
	static pthread_t thread;						/// Thread to start downloads.
	static pthread_mutex_t mutex;					/// Mutex to make downloads vector thread safe.
	static pthread_mutex_t active_download_mutex;	/// Mutex to make active_download thread safe.
	
	static ticpp::Document *doc;
	
	static double ratio;			/// Upload amount divided by download amount.
	static double downloaded;		/// Total amount of bytes downloaded this session.
	static double uploaded;			/// Total amount of bytes uploaded this session.
	static int d_pid = -1;			/// Download thread pid.
	
	static double max_upspeed;		/// Max upload speed.
	static double max_downspeed;	/// Max download speed.
	
	void calculateRatio();
	void calculateDownloadAmount();
	void calculateUploadAmount();
	void setActiveDownload(Download *download);
	Download getActiveDownload();
	std::vector<Download> getDownloads();
	
	void init();
	void startStream(std::string tracker);
	void stopStream();
	void *startStreamThread(void *arg);
	void updateDownloadStatistics();
	std::string buildXML();
	void *dispatch(void* arg);
	
	int resumeDownload(std::string download_hash);
	int startDownload(const std::string download_hash);
	int getIndexFromHash(const std::string download_hash);
	
	struct Amount getDownloadAmount();
	struct Amount getUploadAmount();
	double getRatio();
	
	double getMaxUpSpeed();
	double getMaxDownSpeed();
	void setMaxUpSpeed(double speed);
	void setMaxDownSpeed(double speed);
	
	void upload(std::string filename);
	void startUploads();
	void downloadFirstInList();
	void add(Download *download);
	void pauseDownload(const std::string download_hash);
	void switchDownload(std::string hash);
	void removeFromList(const std::string download_hash);
	void removeFromDisk(const std::string download_hash);
	void clearList();
}

#endif
