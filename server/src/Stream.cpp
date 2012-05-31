#include "../include/Stream.h"

void Stream::init() {
	streaming = false;
	pthread_mutex_init( &_mutex, NULL);
}

void Stream::beginStreaming() {
	pthread_mutex_lock( &_mutex );
	streaming = true;
	pthread_mutex_unlock( &_mutex );
}


bool Stream::readStreaming() {
	pthread_mutex_lock( &_mutex );
	bool result = streaming;
	pthread_mutex_unlock( &_mutex );
	return result;
}

void closeCallback(int fd, short event, void *arg) {
	Stream* stream = (Stream*) arg;
	if (stream->readStreaming()) {
		evtimer_add(stream->getEvent(), swift::tint2tv(TINT_SEC));
		std::cout << "Busy Streaming" << std::endl;
	}
	else
		event_base_loopexit(swift::Channel::evbase, NULL);
}

void Stream::setTracker(std::string tracker) {
	_tracker = (std::string) tracker;
}

/**
 * Get the event to start loop.
 */
event *Stream::getEvent() {
	event *evclose = &_evclose;
	return evclose;
}

void Stream::stop() {
	pthread_mutex_lock( &_mutex );
	streaming = false;
	pthread_mutex_unlock( &_mutex );
}

void Stream::start() {
	
	//Change the directory to Downloads folder.
	int change = chdir("/dtv/usb/sda1/Downloads");
	
	std::cout << "Tracker = " << _tracker <<std::endl;
	
	swift::Address trackeraddr = swift::Address(_tracker.c_str());
	
	evtimer_assign(getEvent(), swift::Channel::evbase, closeCallback, this);
	evtimer_add(getEvent(), swift::tint2tv(TINT_SEC));
	
	//Set the tracker
	std::cout << "Setting the tracker to " << _tracker  << std::endl;
	swift::SetTracker(trackeraddr);
	
	beginStreaming();
	std::cout << "Dispatching the event base." <<std::endl;
	event_base_dispatch(swift::Channel::evbase);
}

