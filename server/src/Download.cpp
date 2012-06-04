#include "Download.h"

/**
 * Stop the download.
 */
void Download::stop() {
	setStatus(STOPPED);
}

/**
 * Clear the download and starts over downloading it.
 */
void Download::retry() {
	stop();
	usleep(200000);
	start();
}

/**
 * Starts downloading and uploading.
 */
void Download::start() {
	if(getStatus() != READY) {
		std::cout << "Download not ready!" << std::endl;
		return;
	}
	
	// Set status to DOWNLOADING
	setStatus(DOWNLOADING);
	
	// Change the directory to Downloads folder.
	//int change = chdir("/dtv/usb/sda1/Downloads");
	int change = chdir("/dtv/usb/sda1/Downloads");
	std::cout << "Changed directory." << std::endl;
	
	
	std::cout << "Tracker Address: " << getTrackerAddress()  << std::endl;
	swift::Address trackeraddr = swift::Address(getTrackerAddress().c_str());
	std::cout << "Set the tracker address." << std::endl;
	std::cout << "Hash: " << getRootHash().c_str()  << std::endl;
	
	swift::Sha1Hash roothash  = swift::Sha1Hash(true, getRootHash().c_str());
	
	// Set the tracker.
	std::cout << "Setting the tracker..." << std::endl;
	swift::SetTracker(trackeraddr);
	
	std::cout << "Filename = " << getFilename() << std::endl;
	
	// Download the file.
	int id  = swift::Open(getFilename().c_str(), roothash);
	
	setID(id);
	
	std::cout << "ID = " << getID() << std::endl;
}

/**
 *  Pauses downloading and uploading.
 */
void Download::pause(){
	swift::Close(getID());
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

