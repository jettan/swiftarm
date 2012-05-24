#include "../include/Download.h"

/**
 * Callback to check if a download is completed.
 */
void isCompleteCallback(int fd, short event, void* arg) {
	Download *self = (Download*) arg;
	std::cout << "ID: " << self->getID() << std::endl;
	
	if (swift::SeqComplete(self->getID()) != swift::Size(self->getID())) {
		std::cout << "Percentage downloaded: " << floorf(((swift::Complete(self->getID()) * 10000.0) / swift::Size(self->getID()) * 1.0) + 0.5) / 100 << std::endl;
		evtimer_add(self->getEvent(), swift::tint2tv(TINT_SEC));
	}
	else
		event_base_loopexit(swift::Channel::evbase, NULL);
}

/**
 * Download a file using libswift.
 * @param str: Struct containing all necessary data for downloads.
 */
void* Download::startThread(void *arg) {
	Download *download = (Download *) arg;
	
	std::cout << "Entered download thread." << std::endl;
	
	// Change the directory to Downloads folder.
	int change = chdir("/dtv/usb/sda1/Downloads");
	std::cout << "Changed directory." << std::endl;
	
	swift::Address trackeraddr = swift::Address(download->getProperties().tracker);
	std::cout << "Set the tracker address." << std::endl;
	
	swift::Sha1Hash roothash  = swift::Sha1Hash(true, download->getProperties().root_hash);
	
	// Set the tracker.
	std::cout << "Setting the tracker..." << std::endl;
	swift::SetTracker(download->getProperties().tracker);
	
	std::cout << "Filename = " << download->getProperties().filename << std::endl;
	
	// Download the file.
	int id = swift::Open(download->getProperties().filename, roothash);
	std::cout << "ID = " << id << std::endl;
	
	download->setID(id);
	
	std::cout << "ID = " << download->getID() << std::endl;

	
	if (download->getID() < 0 ) {
		std::cerr << "Could not download " << download->getProperties().filename << "!" << std::endl;
	} else {
		// Assign callbacks to the event base.
		std::cout << "Now dispatching event base." << std::endl;
		evtimer_assign(download->getEvent(), swift::Channel::evbase, isCompleteCallback, this);
		evtimer_add(download->getEvent(), swift::tint2tv(TINT_SEC));
		
		// Dispatch the event base to enter the swift loop.
		event_base_dispatch(swift::Channel::evbase);
		
		std::cout << "Download of " << download->getProperties().filename << " completed." << std::endl;
		
		// Close the file.
		swift::Close(download->getID());
	}
	/**/
	std::cout << "Exiting download thread." << std::endl;
	// Exit the download thread when the download is finished.
	pthread_exit(NULL);
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
	// Spawn new thread to download the file requested.
	int result_code = pthread_create(&_thread, NULL, Download::callStartThread, this);
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
 * Getter for the download statistics.
 */
Download::downloadStats Download::getStatistics() {
	struct downloadStats returnValue;
	
	if(getID() < 0)
		return returnValue;
		
	pthread_mutex_lock( &_mutex );
	returnValue.download_speed      = _stats.download_speed;
	returnValue.upload_speed        = _stats.upload_speed;
	returnValue.ratio               = _stats.ratio;
	returnValue.download_percentage = _stats.download_percentage;
	returnValue.upload_amount       = _stats.upload_amount;
	returnValue.seeders             = _stats.seeders;
	returnValue.peers               = _stats.peers;
	returnValue.estimated.days      = _stats.estimated.days;
	returnValue.estimated.hours     = _stats.estimated.hours;
	returnValue.estimated.seconds   = _stats.estimated.seconds;
	returnValue.estimated.minutes   = _stats.estimated.minutes;
	pthread_mutex_unlock( &_mutex );
	
	return returnValue;
}

Download::downloadProps Download::getProperties() {
	struct downloadProps returnValue;
	std::cout << "Entering critical section." << std::endl;
	pthread_mutex_lock( &_mutex );
	std::cout << "Entered critical section." << std::endl;
	returnValue.tracker   = _properties.tracker;
	returnValue.root_hash = _properties.root_hash;
	returnValue.filename  = _properties.filename;
	pthread_mutex_unlock( &_mutex );
	
	std::cout << "Exited critical section." << std::endl;
	return returnValue;
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
 * Get the event to start loop.
 */
event *Download::getEvent() {
	return &_evcompl;
}
 
/**
 * Get the download ID.
 */
int Download::getID() {
	pthread_mutex_lock( &_mutex );
	int id = _stats.id;
	pthread_mutex_unlock( &_mutex );
	return id;
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
	_status = status;
}

/**
 * Get the status.
 */
int Download::getStatus() {
	return _status;
}

