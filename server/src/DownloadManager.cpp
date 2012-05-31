#include "../include/DownloadManager.h"

using namespace DownloadManager;

void DownloadManager::init() {
	stream = new Stream();
}

void *DownloadManager::startThread(void* arg) {
	std::cout << "Entering stream thread" << std::endl;
	
	Stream *stream = (Stream*) arg;
	stream->start();
	
	std::cout << "Exiting stream thread" << std::endl;
	
	pthread_exit(NULL);
}

void DownloadManager::stopStream() {
	stream->stop();
}

void DownloadManager::startStream(std::string tracker) {
	
	if (!stream->readStreaming()) {
		stream->setTracker(tracker);
		
		std::cout << "Spawning new thread..." << std::endl;
		int return_code = pthread_create(&streaming_thread, NULL, startThread, (void *) stream);
		
		if (return_code) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << return_code << "." << std::endl;
		}
	} else {
		std::cout << "Already Streaming!" <<std::endl;
	}
		
}

