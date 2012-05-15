#ifndef _DOWNLOADMNGR_H
#define _DOWNLOADMNGR_H

#include "Download.h"
#include "Upload.h"
#include <String>
#include <list>

class DownloadManager {
public:
	
	DownloadManager();
	~DownloadManager();
	
	void startDownload(String link);
	
	void stopDownload(String link);
	
	void pauseDownload(String link);
	
	void resumeDownload(String link);
	
	void startUpload(String name);
	
	void stopUpload(String name);
	
	
private:
	list<Download> downloads;
	list<Upload> uploads;
	
	double downloaded;
	double uploaded;
}

#endif // _DOWNLOADMNGR_H
