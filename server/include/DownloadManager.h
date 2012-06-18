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

#define DEFAULT_PORT 25000
#define UNLIMITED_SPEED 25000

namespace DownloadManager {
	
	struct Amount {
		double amount;
		std::string unit;
	};
	
	static std::vector<Download> downloads;	/// Vector containing all downloads.
	
	static std::string download_directory;
	
	static struct event evcompl;
	static Download *active_download;
	static pthread_t streaming_thread;
	static pthread_t thread;
	static pthread_mutex_t mutex;					/// Mutex to make downloads vector thread safe.
	static pthread_mutex_t active_download_mutex;	/// Mutex to make active_download thread safe.
	
	static ticpp::Document *doc;
	
	static double ratio;		/// Upload amount divided by download amount.
	static double downloaded;	/// Total amount of bytes downloaded this session.
	static double uploaded;		/// Total amount of bytes uploaded this session.
	static int d_pid = -1;		/// Download thread pid.
	
	static double max_upspeed;		/// Max upload speed.
	static double max_downspeed;	/// Max download speed.
	
	void calculateRatio();
	void calculateDownloadAmount();
	void calculateUploadAmount();
	void setDownloadDirectory(std::string dir);
	void setActiveDownload(Download *download);
	Download getActiveDownload();
	std::vector<Download> getDownloads();
	std::string getDownloadDirectory();
	
	void init(std::string download_dir);
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
