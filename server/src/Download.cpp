#include "Download.h"

/**
 * Stops the download and removes all content from disk.
 */
void Download::stop() {
	setStatus(STOPPED);
	swift::Close(getID());
	
	std::string mhash = getFilename() + ".mhash";
	std::string mbinmap = getFilename() + ".mbinmap";
	remove(mhash.c_str());
	remove(mbinmap.c_str());
}

/**
 * Deletes the downloaded content and try again from the beginning.
 */
void Download::retry() {
	stop();
	remove(getFilename().c_str());
	usleep(200000);
	start();
}

/**
 * Starts downloading and uploading.
 */
void Download::start() {
	if (getStatus() != READY) {
		std::cout << "Download not ready!" << std::endl;
		return;
	}
	
	// Set status to DOWNLOADING
	setStatus(DOWNLOADING);
	
	// Change the directory to Downloads folder.
	// Temporarily hard coded.
	int change = chdir("/dtv/usb/sda1/Downloads");
	
	swift::Address trackeraddr = swift::Address(getTrackerAddress().c_str());
	swift::Sha1Hash roothash   = swift::Sha1Hash(true, getRootHash().c_str());
	
	std::cout << "Filename = " << getFilename() << std::endl;
	
	// Open the file with swift.
	int id  = swift::Open(getFilename().c_str(), roothash, trackeraddr);
	setID(id);
	
	std::cout << "ID = " << getID() << std::endl;
}

/**
 *  Pauses downloading and uploading.
 */
void Download::pause() {
	if (getStatus() == PAUSED) {
		std::cout << "Already Paused" << std::endl;
		return;
	}
	// Let swift save the progress and close the file.
	swift::Checkpoint(getID());
	swift::Close(getID());
	setStatus(PAUSED);
}

/**
 * Resumes a paused download.
 */
void Download::resume() {
	if (getStatus() != PAUSED)
		return;
		
	setStatus(DOWNLOADING);
	
	swift::Address trackeraddr = swift::Address(getTrackerAddress().c_str());
	swift::Sha1Hash roothash   = swift::Sha1Hash(true, getRootHash().c_str());
	
	int id = swift::Open(getFilename().c_str(), roothash, trackeraddr, false);
	
	// Not sure if this has to be called...
	setID(id);
}

/**
 * Setter for download speed.
 * @param speed: The speed in Kb/sec.
 */
void Download::setDownloadSpeed(double speed) {
	if (getID() < 0 || speed < 0)
		return;
	
	pthread_mutex_lock(&_mutex);
	_stats.download_speed = speed;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Setter for upload speed.
 * @param speed: The speed in Kb/sec.
 */
void Download::setUploadSpeed(double speed) {
	if (getID() < 0 || speed < 0)
		return;
	
	pthread_mutex_lock(&_mutex);
	_stats.upload_speed = speed;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Calculates upload/download ratio.
 */
void Download::calculateRatio() {
	if (getID() < 0)
		return;
	
	double download_amount = getStatistics().download_amount;
	double upload_amount   = getStatistics().upload_amount;
	
	pthread_mutex_lock(&_mutex);
	if (download_amount > 0) {
		_stats.ratio = upload_amount/download_amount;
	} else {
		_stats.ratio = 0;
	}
	
	pthread_mutex_unlock(&_mutex);
}

/**
 * Setter for download progress.
 * @param percentage: The percentage to be set between 0 and 100.
 */
void Download::setProgress(double percentage) {
	if (getID() < 0 || percentage < 0 || percentage > 100)
		return;
		
	pthread_mutex_lock(&_mutex);
	_stats.download_percentage = percentage;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Setter for upload amount.
 * @param amount: Amount to be set in Kb.
 */
void Download::setUploadAmount(double amount) {
	if (getID() < 0 || amount < 0)
		return;
		
	pthread_mutex_lock(&_mutex);
	_stats.upload_amount = amount;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Setter for download amount.
 * @param amount: Amount to be set in Kb.
 */
void Download::setDownloadAmount(double amount) {
	if (getID() < 0 || amount < 0)
		return;
		
	pthread_mutex_lock(&_mutex);
	_stats.download_amount = amount;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Setter for amount of seeders.
 * @param amount: The amount of seeders to be set.
 */
void Download::setSeeders(int amount) {
	// TODO: or amount > max connections - the rest of peers we got already.
	if (getID() < 0 || amount < 0)
		return;
		
	pthread_mutex_lock(&_mutex);
	_stats.seeders = amount;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Setter for amount of peers.
 * @param amount: The amount of peers to be set.
 */
void Download::setPeers(int amount) {
	if (getID() < 0 || amount < 0)
		return;
		
	pthread_mutex_lock(&_mutex);
	_stats.peers = amount;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Calculates estimated download time.
 */
void Download::calculateEstimatedTime() {
	if (getID() < 0)
		return;
	
	struct time estimated_time;
	double speed            = getStatistics().download_speed;
	double time_in_seconds  = ( swift::Size(getID()) - swift::Complete(getID()) ) / speed;
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
	
	pthread_mutex_lock(&_mutex);
	_stats.estimated.days      = estimated_time.days;
	_stats.estimated.hours     = estimated_time.hours;
	_stats.estimated.minutes   = estimated_time.minutes;
	_stats.estimated.seconds   = estimated_time.seconds;
	pthread_mutex_unlock(&_mutex);
}

/**
 * Setter for download ID.
 * @param id: The download id given by swift::Open.
 */
void Download::setID(int id) {
	if(id > -1) {
		pthread_mutex_lock(&_mutex);
		_stats.id = id;
		pthread_mutex_unlock(&_mutex);
	}
	else {
		_stats.id = -1;
	}
}


/**
 * Setter for status
 * @param status: The new status to be set.
 */
void Download::setStatus(int status) {
	if (getStatus() == status)
		return;
		
	if(status > -1 && status < STATUS_SIZE) {
		pthread_mutex_lock(&_mutex);
		_status = status;
		pthread_mutex_unlock(&_mutex);
	}
}

/**
 * Getter for the download ID.
 */
const int Download::getID() {
	pthread_mutex_lock(&_mutex);
	const int id = _stats.id;
	pthread_mutex_unlock(&_mutex);
	return id;
}

/**
 * Getter for the status.
 */
const int Download::getStatus() {
	pthread_mutex_lock(&_mutex);
	const int status = _status;
	pthread_mutex_unlock(&_mutex);
	return status;
}

/**
 * Getter for the download statistics.
 */
Download::downloadStats Download::getStatistics() {
	struct downloadStats statistics;
	
	if(getID() < 0)
		return statistics;
		
	pthread_mutex_lock(&_mutex);
	statistics.download_speed      = _stats.download_speed;
	statistics.upload_speed        = _stats.upload_speed;
	statistics.ratio               = _stats.ratio;
	statistics.download_percentage = _stats.download_percentage;
	statistics.upload_amount       = _stats.upload_amount;
	statistics.download_amount     = _stats.download_amount;
	statistics.seeders             = _stats.seeders;
	statistics.peers               = _stats.peers;
	statistics.estimated.days      = _stats.estimated.days;
	statistics.estimated.hours     = _stats.estimated.hours;
	statistics.estimated.seconds   = _stats.estimated.seconds;
	statistics.estimated.minutes   = _stats.estimated.minutes;
	pthread_mutex_unlock(&_mutex);
	
	return statistics;
}

/**
 * Getter for the tracker address.
 */
std::string Download::getTrackerAddress() {
	return _tracker;
}

/**
 * Getter for the filename.
 */
std::string Download::getFilename() {
	return _filename;
}

/**
 * Getter for the root hash.
 */
std::string Download::getRootHash() {
	return _root_hash;
}

