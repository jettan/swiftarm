#include "include/DownloadManager.h"


/**
 * Indicates that streaming should stop.
 */
static volatile bool stopStreaming() {
	pthread_mutex_lock( &stream_mutex );
	streaming = false;
	bool result = streaming;
	pthread_mutex_unlock( &stream_mutex );
	return result;
}

/**
 * Indicates that streaming should start.
 */
static volatile bool startStreaming() {
	pthread_mutex_lock( &stream_mutex );
	streaming = true;
	bool result = streaming;
	pthread_mutex_unlock( &stream_mutex );
	return result;
}

/**
 * Returns the current value of streaming.
 */
static volatile bool readStreaming() {
	pthread_mutex_lock( &stream_mutex );
	bool result = streaming;
	pthread_mutex_unlock( &stream_mutex );
	return result;
}

/**
 * Callback to check if a download is completed.
 */
static void IsCompleteCallback(int fd, short event, void *arg) {
	if (swift::SeqComplete(download) != swift::Size(download)) {
		std::cout << "Percentage downloaded: " << floorf(((swift::Complete(download) * 10000.0) / swift::Size(download) * 1.0) + 0.5) / 100 << std::endl;
		evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	}
	else
		event_base_loopexit(swift::Channel::evbase, NULL);
}

/*
 * Callback needed to close streams.
 */
static void CloseCallback(int fd, short event, void *arg) {
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


static void startDownload(void *str){
	
	std::cout << "Entered download thread." << std::endl;
	
	// Change the directory to Downloads folder.
	chdir("/tmp");
	
	DownloadArgs *da = (DownloadArgs*) str;
	download         = 0;
	char* tracker    = da->tracker;
	char* hash       = da->hash;
	char* filename   = da->filename;
	
	swift::Address trackeraddr = swift::Address(tracker);
	swift::Sha1Hash root_hash = swift::Sha1Hash(true, hash);
	
	// Set the tracker.
	std::cout << "Setting the tracker..." << std::endl;
	swift::SetTracker(trackeraddr);
	
	// Download the file.
	download = swift::Open(filename, root_hash);
	
	if (download < 0 ) {
		std::cerr << "Could not download " << filename << "!" << std::endl;
	} else {
		// Assign callbacks to the event base.
		std::cout << "Now dispatching event base." << std::endl;
		evtimer_assign(&evcompl, swift::Channel::evbase, IsCompleteCallback, NULL);
		evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
		
		// Dispatch the event base to enter the swift loop.
		event_base_dispatch(swift::Channel::evbase);
		
		std::cout << "Download of " << filename << " completed." << std::endl;
		// Close the file.
		swift::Close(download);
	}
	
	std::cout << "Exiting download thread." << std::endl;
	// Exit the download thread when the download is finished.
	pthread_exit(NULL);
}

static void stopDownload(String link){
	
}

/**
 * Stream a file using libswift.
 * @param str: Struct containing all necessary data for streams.
 */
static void* stream(void *str) {
	std::cout << "Entered the stream thread." << std::endl;
	
	// Change the directory to Downloads folder.
	chdir("/tmp");
	
	DownloadArgs *da = (DownloadArgs*) str;
	char* tracker    = da->tracker;
	
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

static void pauseDownload(String link){
	
}

static void resumeDownload(String link){
	
}

static void startUpload(String name){
	
}

static void stopUpload(String name){
	
}
