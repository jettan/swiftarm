#include "../include/DownloadManager.h"

using namespace DownloadManager;

void beginStreaming() {
	pthread_mutex_lock( &stream_mutex );
	streaming = true;
	bool result = streaming;
	pthread_mutex_unlock( &stream_mutex );
}

void stopStreaming() {
	pthread_mutex_lock( &stream_mutex );
	streaming = false;
	pthread_mutex_unlock( &stream_mutex );
}

volatile bool readStreaming() {
	pthread_mutex_lock( &stream_mutex );
	bool result = streaming;
	pthread_mutex_unlock( &stream_mutex );
	return result;
}

void CloseCallback(int fd, short event, void *arg) {
	std::cout << "Callback value of streaming: " << readStreaming() << std::endl;
	if (readStreaming()) {
		evtimer_add(&evclose, swift::tint2tv(TINT_SEC));
		std::cout << "Busy streaming..." << std::endl;
		
	} else {
		std::cerr << "Calling loopexit of HTTPgw." << std::endl;
		event_base_loopexit(swift::Channel::evbase, NULL);
		std::cout << "loopexit called." << std::endl;
	}
}

void *DownloadManager::stream(void *str) {
	std::cout << "Entered the stream thread." << std::endl;
	
	// Change the directory to Downloads folder.
	int change = chdir("/home/jettan/Downloads");
	
	char *tracker = (char *) str;
	
	std::cout << "Tracker = " << tracker << std::endl;
	
	swift::Address trackeraddr = swift::Address(tracker);
	
	// Set the tracker.
	std::cout << "Setting the tracker..." << std::endl;
	swift::SetTracker(trackeraddr);
	
	evtimer_assign(&evclose, swift::Channel::evbase, CloseCallback, NULL);
	evtimer_add(&evclose, swift::tint2tv(TINT_SEC));
	
	std::cout << "Dispatching the event base." << std::endl;
	event_base_dispatch(swift::Channel::evbase);
	
	std::cout << "Exiting stream thread." << std::endl;
	// Exit the stream thread when the download is finished.
	pthread_exit(NULL);
}

void DownloadManager::startStreaming(char *tracker_address) {
	
	if (!readStreaming()) {
		beginStreaming();
		
		std::cout << "Spawning new thread..." << std::endl;
		int rc = pthread_create(&streaming_thread, NULL, stream, (void *) tracker_address);
		
		if (rc) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << rc << "." << std::endl;
		}
	}
}


