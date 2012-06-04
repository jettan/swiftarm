#ifndef _DOWNLOADMANAGER_H
#define _DOWNLOADMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <float.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event-config.h>
#include <event2/thread.h>

#include "Stream.h"
#include "Download.h"
#include "swift.h"

namespace DownloadManager {
	
	
	static std::vector<Download> downloads;	/// Vector containing all downloads.
	
	static std::string downloadDirectory;
	
	static struct event evcompl;
	static Download *activeDownload;
	static pthread_t streaming_thread;
	static pthread_t thread;
	
	static double downloaded;			/// Total amount of bytes downloaded this session.
	static double uploaded;				/// Total amount of bytes uploaded this session.
	static int d_pid;					/// Download thread pid.
	
	void setDownloadDirectory(std::string dir);
	std::string getDownloadDirectory();
	Download* getActiveDownload();
	Download* getDownloadWithID(const int download_id);
	
	void startStream(std::string tracker);
	void stopStream();
	void *startStreamThread(void *arg);
	void *dispatch(void* arg);
	
	void downloadFirstInList();
	void startDownload(const int download_id);
	void add(Download download);
	int getIndexFromID(const int download_id);
	void removeFromList(const int download_id);
	void removeFromDisk(const int download_id);
	void clearList();
}

#endif
