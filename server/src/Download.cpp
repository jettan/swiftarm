#include "../include/Download.h"

/**
 * Callback to check if a download is completed.
 */
void isCompleteCallback(int fd, short event, void* arg) {
	Download *download = (Download*) arg;
	if ((swift::SeqComplete(download->getID()) != swift::Size(download->getID()))) {
		
		std::cout << "Percentage downloaded: " << floorf(((swift::Complete(download->getID()) * 10000.0) / 
		swift::Size(download->getID()) * 1.0) + 0.5) / 100 << std::endl;
		evtimer_add(download->getEvent(), swift::tint2tv(TINT_SEC));
	}
	else {
		//TODO: Don't close download until it is removed
		download->setStatus(UPLOADING);
		event_base_loopexit(swift::Channel::evbase, NULL);
	}
}

/**
 * Clear the download and starts over downloading it.
 */
void Download::retry() {
	
}

/**
 * Starts downloading and uploading.
 */
void Download::start() {
	// Change the directory to Downloads folder.
	int change = chdir("/dtv/usb/sda1/Downloads");
	std::cout << "Changed directory." << std::endl;
	
	swift::Address trackeraddr = swift::Address(getTrackerAddress().c_str());
	std::cout << "Set the tracker address." << std::endl;
	
	swift::Sha1Hash roothash  = swift::Sha1Hash(true, getRootHash().c_str());
	
	// Set the tracker.
	std::cout << "Setting the tracker..." << std::endl;
	swift::SetTracker(getTrackerAddress().c_str());
	
	std::cout << "Filename = " << getFilename() << std::endl;
	
	// Download the file.
	int id = swift::Open(getFilename().c_str(), roothash);
	std::cout << "ID = " << id << std::endl;
	
	setID(id);
	
	std::cout << "ID = " << getID() << std::endl;
	
	
	if (getID() < 0 ) {
		std::cerr << "Could not download " << getFilename() << "!" << std::endl;
	} else {
		// Assign callbacks to the event base.
		std::cout << "Now dispatching event base." << std::endl;
		evtimer_assign(getEvent(), swift::Channel::evbase, isCompleteCallback, this);
		evtimer_add(getEvent(), swift::tint2tv(TINT_SEC));
		
		setStatus(DOWNLOADING);
		// Dispatch the event base to enter the swift loop.
		event_base_dispatch(swift::Channel::evbase);
		
		std::cout << "Download of " << getFilename() << " completed." << std::endl;
		
		// Close the file.
		swift::Close(getID());
	}
}

/**
 *  Pauses downloading and uploading.
 */
void Download::pause(){
	
}

/**
 * Resumes a paused download.
 */
void Download::resume() {
	
}

/**
 * Setter for download speed.
 */
void Download::setDownloadSpeed(double speed) {
	if(getID() < 0)
		return;
		
	pthread_mutex_lock( &_mutex );
	_stats.download_speed = speed;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for upload speed.
 */
void Download::setUploadSpeed(double speed) {
	if(getID() < 0)
		return;
		
	pthread_mutex_lock( &_mutex );
	_stats.upload_speed = speed;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for ratio.
 */
void Download::calculateRatio() {
	if(getID() < 0)
		return;
		
	double download_speed = getStatistics().download_speed;
	double upload_speed   = getStatistics().upload_speed;
	
	pthread_mutex_lock( &_mutex );
	if(download_speed != 0)
		_stats.ratio = upload_speed/download_speed;
	else
		_stats.ratio = 0;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for download progress.
 */
void Download::setProgress(double percentage) {
	if(getID() < 0)
		return;
		
	pthread_mutex_lock( &_mutex );
	_stats.download_percentage = percentage;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for upload amount.
 */
void Download::setUploadAmount(double amount) {
	if(getID() < 0)
		return;
		
	pthread_mutex_lock( &_mutex );
	_stats.upload_amount = amount;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for amount of seeders.
 */
void Download::setSeeders(int amount) {
	if(getID() < 0)
		return;
		
	pthread_mutex_lock( &_mutex );
	_stats.seeders = amount;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for amount of peers.
 */
void Download::setPeers(int amount) {
	if(getID() < 0)
		return;
		
	pthread_mutex_lock( &_mutex );
	_stats.peers = amount;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for estimated download time.
 */
void Download::calculateEstimatedTime() {
	if(getID() < 0)
		return;
	
	struct time estimated_time;
	double speed            = getStatistics().download_speed;
	double time_in_seconds  = ( _size - swift::Complete(getID()) ) / speed;
	double time_left        = time_in_seconds;
	
	int days     = (int) floor(time_left / SECONDS_PER_DAY);
	time_left   -= days * SECONDS_PER_DAY;
	
	int hours    = (int) floor(time_left / SECONDS_PER_HOUR);
	time_left   -= hours * SECONDS_PER_HOUR;
	
	int minutes  = (int) floor(time_left / SECONDS_PER_MINUTE);
	time_left   -= minutes * SECONDS_PER_MINUTE;
	
	estimated_time.days    = days;
	estimated_time.hours   = hours;
	estimated_time.minutes = minutes;
	estimated_time.seconds = time_left;
	
	pthread_mutex_lock( &_mutex );
	_stats.estimated.days      = estimated_time.days;
	_stats.estimated.hours     = estimated_time.hours;
	_stats.estimated.minutes   = estimated_time.minutes;
	_stats.estimated.seconds   = estimated_time.seconds;
	pthread_mutex_unlock( &_mutex);
}

/**
 * Setter for download ID.
 */
void Download::setID(int id) {
	pthread_mutex_lock( &_mutex );
	_stats.id = id;
	pthread_mutex_unlock( &_mutex );
}


/**
 * Setter for status
 */
void Download::setStatus(int status) {
	if(_status == status)
		return;
		
	pthread_mutex_lock( &_mutex );
	_status = status;
	pthread_mutex_unlock( &_mutex );
}

/**
 * Get the event to start loop.
 */
event *Download::getEvent() {
	event *evcompl = &_evcompl;
	return evcompl;
}
 
/**
 * Get the download ID.
 */
const int Download::getID() {
	pthread_mutex_lock( &_mutex );
	const int id = _stats.id;
	pthread_mutex_unlock( &_mutex );
	return id;
}
/**
 * Get the status.
 */
const int Download::getStatus() {
	pthread_mutex_lock( &_mutex );
	const int status = _status;
	pthread_mutex_unlock( &_mutex );
	return status;
}

/**
 * Get the download statistics.
 */
Download::downloadStats Download::getStatistics() {
	struct downloadStats statistics;
	
	if(getID() < 0)
		return statistics;
		
	pthread_mutex_lock( &_mutex );
	statistics.download_speed      = _stats.download_speed;
	statistics.upload_speed        = _stats.upload_speed;
	statistics.ratio               = _stats.ratio;
	statistics.download_percentage = _stats.download_percentage;
	statistics.upload_amount       = _stats.upload_amount;
	statistics.seeders             = _stats.seeders;
	statistics.peers               = _stats.peers;
	statistics.estimated.days      = _stats.estimated.days;
	statistics.estimated.hours     = _stats.estimated.hours;
	statistics.estimated.seconds   = _stats.estimated.seconds;
	statistics.estimated.minutes   = _stats.estimated.minutes;
	pthread_mutex_unlock( &_mutex );
	
	return statistics;
}

/**
 * Get the tracker address.
 */
std::string Download::getTrackerAddress() {
	return _tracker;
}

/**
 * Get the filename.
 */
std::string Download::getFilename() {
	pthread_mutex_lock( &_mutex );
	std::string name = _filename;
	pthread_mutex_unlock( &_mutex );
	
	return name;
}

/**
 * Get the root hash.
 */
std::string Download::getRootHash() {
	return _root_hash;
}

