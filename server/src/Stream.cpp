#include "Stream.h"

Stream* Stream::_instance;

/**
 * Initialises stream state and mutex.
 */
void Stream::init() {
	_streaming = false;
	pthread_mutex_init(&_mutex, NULL);
}

/**
 * Sets stream state to "begin streaming".
 */
void Stream::beginStreaming() {
	pthread_mutex_lock( &_mutex );
	if (!_streaming)
		_streaming = true;
	pthread_mutex_unlock( &_mutex );
}

/**
 * Returns stream state.
 */
bool Stream::readStreaming() {
	pthread_mutex_lock( &_mutex );
	bool result = _streaming;
	pthread_mutex_unlock( &_mutex );
	return result;
}

/**
 * Libevent loop for streaming files, also used to close stream.
 * @param fd: The file descriptor used by swift.
 * @param event: The event it gets from libevent.
 * @param arg: Argument taken by callback.
 */
void closeCallback(int fd, short event, void *arg) {
	Stream* stream = (Stream*) arg;
	if (stream->readStreaming()) {
		evtimer_add(stream->getEvent(), swift::tint2tv(TINT_SEC));
		std::cout << "Busy Streaming" << std::endl;
	}
}

/**
 * Returns the tracker address.
 */
std::string Stream::getTrackerAddress() {
	return _tracker;
}

/**
 * Returns the root hash.
 */
std::string Stream::getRootHash() {
	return _hash;
}

/**
 * Sets the tracker address of the stream.
 * @param tracker: The tracker address to be set.
 */
void Stream::setTracker(std::string tracker) {
	_tracker = (std::string) tracker;
}

/**
 * Sets the root hash of the stream.
 * @param hash: The root hash to be set.
 */
void Stream::setRoothash(std::string hash) {
	_hash = (std::string) hash;
}

/**
 * Returns the event to start the libevent loop.
 */
event *Stream::getEvent() {
	return &_evclose;
}

/**
 * Returns Singleton instance of own class.
 */
Stream *Stream::getInstance() {
	if (!_instance)
		_instance = new Stream;
		
	return _instance;
}

/**
 * Stops the stream.
 */
void Stream::stop() {
	if (readStreaming()) {
		pthread_mutex_lock( &_mutex );
		_streaming = false;
		pthread_mutex_unlock( &_mutex );
		evtimer_del(getEvent());
		
		std::string filename = getRootHash();
		std::string mbinmap  = filename + ".mbinmap";
		std::string mhash    = filename + ".mhash";
		
		remove(mbinmap.c_str());
		remove(mhash.c_str());
		remove(filename.c_str());
	}
}

/**
 * Starts the stream.
 */
void Stream::start() {
	// Change the directory to Downloads folder.
	int change = chdir(Settings::getDownloadDirectory().c_str());
	
	std::cout << "Tracker = " << _tracker <<std::endl;
	
	swift::Address trackeraddr = swift::Address(_tracker.c_str());
	
	evtimer_assign(getEvent(), swift::Channel::evbase, closeCallback, this);
	evtimer_add(getEvent(), swift::tint2tv(TINT_SEC));
	
	//Set the tracker
	std::cout << "Setting the tracker to " << _tracker << std::endl;
	swift::SetTracker(trackeraddr);
	
	beginStreaming();
	std::cout << "Dispatching the event base." << std::endl;
	
}

