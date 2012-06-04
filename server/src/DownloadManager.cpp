#include "DownloadManager.h"

void DownloadManager::setDownloadDirectory(std::string dir) {
	downloadDirectory = dir;
}

std::string DownloadManager::getDownloadDirectory(){
	return downloadDirectory;
}

Download* DownloadManager::getActiveDownload(){
	return activeDownload;
}

Download* DownloadManager::getDownloadWithID(const int download_id){
	
	int index = getIndexFromID(download_id);
	return &downloads.at(download_id);
}

// Download first element in the download list
void DownloadManager::downloadFirstInList(){
	
	if(downloads.size() > 0) {
		startDownload(downloads.front().getID());
		activeDownload = &downloads.front();
	}
}

/**
 * Callback to check if a download is completed.
 */
void downloadCallback(int fd, short event, void* arg) {
	std::cout << "SeqComplete " << swift::SeqComplete(DownloadManager::activeDownload->getID()) << std::endl;
	
	std::cout << "Size " << swift::Size(DownloadManager::activeDownload->getID()) << std::endl;
	
	evtimer_add(&DownloadManager::evcompl, swift::tint2tv(TINT_SEC));
}

void* DownloadManager::dispatch(void* arg) {
	
	// Assign callbacks to the event base.
	std::cout << "Entered thread." << std::endl;
	evtimer_assign(&evcompl, swift::Channel::evbase, downloadCallback, NULL);
	evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	
	// Dispatch the event base to enter the swift loop.
	
	std::cout << "Now dispatching event base." << std::endl;
	event_base_dispatch(swift::Channel::evbase);
	
	pthread_exit(NULL);
}

/**
 * Start a download with a specific download ID.
 */
void DownloadManager::startDownload(const int download_id) {
	if (activeDownload) {
		activeDownload->pause();
	}
	
	int index = getIndexFromID(download_id);
	if (index >= 0) {
		activeDownload = &downloads.at(index);
		activeDownload->start();
		
		if (activeDownload->getID() < 0 ) {
			std::cerr << "Could not download " << activeDownload->getFilename() << "!" << std::endl;
			return;
		}
			
		if(d_pid) {
			return;
		} else {
			d_pid = pthread_create(&thread, NULL, dispatch, NULL);
		}
	}
//else if(activeDownload) {
	//	activeDownload->resume();
//	}
}

/**
 * Add a download to the list
 */
void DownloadManager::add(Download *download) {
	downloads.push_back(*download);
	
	if(downloads.size() == 1) {
		downloadFirstInList();
	}
}

/**
 * Find the index of a download in the list based on the download ID
 */
int DownloadManager::getIndexFromID(const int download_id) {
	
	for(int i = 0; i < downloads.size(); i++) {
		if(downloads.at(i).getID() == download_id) {
			return i;
		}
	}
	return -1;
}

/**
 * Remove a download from the list based on the download ID
 */
void DownloadManager::removeFromList(const int download_id) {
	
	int index = getIndexFromID(download_id);
	if(index >= 0) {
		if(downloads.at(index).getStatus() == DOWNLOADING) {
			downloads.at(index).stop();
			downloadFirstInList();
		}
		
		downloads.erase(downloads.begin() + index);
	
	}
}

/** 
 * Remove a download from the hard disk based on download ID
 */
void DownloadManager::removeFromDisk(const int download_id) {
	
	int index = getIndexFromID(download_id);
	
	if(index >= 0) {
		
		std::string filename = downloads.at(index).getFilename();
		removeFromList(download_id);
		
		filename = downloadDirectory + "/" + filename;
		
		if(filename.c_str() == 0) {
			// File removed successfully
			std::cout << "File at directory: " << filename << " has been removed" <<std::endl;
			
		} else {
			// File not found
			std::cout << "Could not find file at directory: " << filename <<std::endl;
		}
	}
}

/** 
 * Remove all downloads from the list
 */
void DownloadManager::clearList() {
	
	if(activeDownload != NULL){
		activeDownload->stop();
	}
	downloads.clear();
}

void DownloadManager::stopStream() {
	Stream::getInstance()->stop();
}

void* DownloadManager::startStreamThread(void* arg) {
	Stream::getInstance()->start();
	pthread_exit(NULL);
}

void DownloadManager::startStream(std::string tracker) {
	
	if (!Stream::getInstance()->readStreaming()) {
		Stream::getInstance()->setTracker(tracker);
		
		std::cout << "Spawning new thread..." << std::endl;
		int return_code = pthread_create(&streaming_thread, NULL, startStreamThread, NULL);
		
		if (return_code) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << return_code << "." << std::endl;
		}
	} else {
		std::cout << "Already Streaming!" <<std::endl;
	}
	
}

