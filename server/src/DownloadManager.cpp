#include "../include/DownloadManager.h"

using namespace DownloadManager;

void add(Download download){
	
	downloads.push_back(download);
	
	if(downloads.size() == 1){
		downloads.front()->start();
	}
}

void removeFromList(const int download_id){
	
	for(int i = 0; i < downloads.size(); i++){
		if(downloads.at(i)->getID() == download_id){
			if(downloads.at(i)->getStatus() == DOWNLOADING){
				downloads.at(i)->stop();
			}
		downloads.erase(i);
		}
	}
}

void DownloadManager::stopStream() {
	Stream::getInstance()->stop();
}

void DownloadManager::startStream(std::string tracker) {
	
	if (!Stream::getInstance()->readStreaming()) {
		Stream::getInstance()->setTracker(tracker);
		
		std::cout << "Spawning new thread..." << std::endl;
		int return_code = pthread_create(&streaming_thread, NULL, startThread, NULL);
		
		if (return_code) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << return_code << "." << std::endl;
		}
	} else {
		std::cout << "Already Streaming!" <<std::endl;
	}
	
}

