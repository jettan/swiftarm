#ifndef _DOWNLOADMNGR_H
#define _DOWNLOADMNGR_H

#include "Download.h"
#include "Upload.h"
#include <String>
#include <list>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <float.h>
#include <pthread.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event-config.h>
#include <event2/thread.h>
#include <math.h>

#include "swift.h"
#include <String>

class DownloadManager {
public:
	
	static volatile bool stopStreaming();
	
	static volatile bool startStreaming();
	
	static volatile bool readStreaming();
	
	static void IsCompleteCallback(int fd, short event, void *arg);
	
	static void CloseCallback(int fd, short event, void *arg);
	
	static void startDownload(void *str);
	
	static void stopDownload(String link);
	
	static void* stream (void *str);
	
	static void pauseDownload(String link);
	
	static void resumeDownload(String link);
	
	static void startUpload(String name);
	
	static void stopUpload(String name);
	
	
private:
	
	static list<Download> downloads;
	static list<Upload> uploads;
	
	static double downloaded;
	static double uploaded;
	
	struct event_base *base;
	
	typedef struct str {
		char* tracker;
		char* hash;
		char* filename;
	} DownloadArgs;
	
	volatile bool streaming = false;
	sigset_t oSignalSet;
	pthread_t thread;
	int rc;
	DownloadArgs download_args;
	struct event evcompl;
	struct event evclose;
	int download;
	
	pthread_mutex_t stream_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	
}

#endif // _DOWNLOADMNGR_H
