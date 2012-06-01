#include "../include/DownloadManager.h"

using namespace DownloadManager;

void *DownloadManager::startThread(void* arg) {
	std::cout << "Entering stream thread" << std::endl;
	
	Stream::getInstance()->start();
	
	std::cout << "Exiting stream thread" << std::endl;
	
	pthread_exit(NULL);
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

