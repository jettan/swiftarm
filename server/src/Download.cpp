#include "../include/Download.h"

// Return code of the thread.
int return_code;

/**
 * Callback to check if a download is completed.
 */
void Download::isCompleteCallback(int fd, short event, void *arg) {
	if (swift::SeqComplete(getID()) != swift::Size(getID())) {
		std::cout << "Percentage downloaded: " << floorf(((swift::Complete(getID()) * 10000.0) / swift::Size(getID()) * 1.0) + 0.5) / 100 << std::endl;
		evtimer_add(&_evcompl, swift::tint2tv(TINT_SEC));
	}
	else
		event_base_loopexit(swift::Channel::evbase, NULL);
}

/**
 * Initialises id and mutex of Download.
 */
void Download::init() {
	pthread_mutex_init(&_mutex, NULL);
	setStatus(READY);
	_id     = -1;
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
	pthread_mutex_unlock( &_mutex);
	
	return returnValue;
}

/**
 * Getter for the download properties.
 */
Download::downloadProps Download::getProperties() {
	return _properties;
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
	double upload_speed = getStatistics().upload_speed;
	
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
 * Setter for status
 */
void Download::setStatus(int status) {
	_status = status;
}

/**
 * Get the download ID.
 */
int Download::getID() {
	return _id;
}

/**
 * Get the status.
 */
int Download::getStatus() {
	return _status;
}
