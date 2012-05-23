#ifndef _DOWNLOADMNGR_H
#define _DOWNLOADMNGR_H

#include "Download.h"

#include <vector>
#include <iostream>

#include <cstdio>
#include <cstdlib>

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
#include <math.h>
#include "swift.h"

namespace DownloadManager {
private:
	static vector<Download> downloads;	/// Vector containing all downloads.
	
	static double downloaded;			/// Total amount of bytes downloaded this session.
	static double uploaded;				/// Total amount of bytes uploaded this session.
	
public:
	static void startStreaming();
	static void stopStreaming();
	static void stream();
	
	static void add(Download download);
	static void removeFromList(const int download_id);
	static void removeFromDisk(const int download_id);
	static void clearList();
}

#endif // _DOWNLOADMNGR_H
