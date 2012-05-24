#ifndef _DOWNLOADMANAGER_H
#define _DOWNLOADMANAGER_H

#include "Download.h"

#include <vector>
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

#include "swift.h"

namespace DownloadManager {
	static std::vector<Download> downloads;	/// Vector containing all downloads.
	static pthread_t streaming_thread;
	static pthread_mutex_t stream_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	static volatile bool streaming;
	static struct event evclose;
	
	static double downloaded;			/// Total amount of bytes downloaded this session.
	static double uploaded;			/// Total amount of bytes uploaded this session.
	
	void startStreaming(char* tracker);
	void stopStreaming();
	void *stream(void *str);
	
	void add(Download download);
	void removeFromList(const int download_id);
	void removeFromDisk(const int download_id);
	void clearList();
}

#endif
