#include "Download.h"

/**
 * Stops the download and removes all content from disk.
 */
void Download::stop() {
	
	const double left_over = swift::Size(getID()) - swift::Complete(getID());
	
	if (getStatus() != STOPPED && getStatus() != PAUSED) {
		swift::Close(getID());
	}
	setStatus(STOPPED);
	
	std::string mhash = getFilename() + ".mhash";
	std::string mbinmap = getFilename() + ".mbinmap";
	remove(mhash.c_str());
	remove(mbinmap.c_str());
	
	if (left_over > 0) {
		std::cout << "Removed file" << std::endl;
		remove(getFilename().c_str());
	}
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
	int change = chdir(Settings::getDownloadDirectory().c_str());
	
	swift::Address trackeraddr = swift::Address(getTrackerAddress().c_str());
	swift::Sha1Hash roothash   = swift::Sha1Hash(true, getRootHash().c_str());
	
	if (getID() < 0) {
		// Open the file with swift.
		int id  = swift::Open(getFilename().c_str(), roothash, trackeraddr);
		setID(id);
	}
	pthread_mutex_lock(&_transfer_mutex);
	_transfer = swift::FileTransfer::file(getID());
	if(_transfer == NULL) {
		std::cout << "TRANSFER IS NULL" << std::endl;
	}
	pthread_mutex_unlock(&_transfer_mutex);
}

/**
 *  Pauses downloading and uploading.
 */
void Download::pause() {
	std::cout << "Pausing...." << std::endl;
	if (getStatus() == PAUSED) {
		std::cout << "Already Paused" << std::endl;
		return;
	}
	
	// Let swift save the progress and close the file.
	pthread_mutex_lock(&_transfer_mutex);
	swift::Channel::CloseTransfer(_transfer);
	pthread_mutex_unlock(&_transfer_mutex);
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
	setID(id);
	
	pthread_mutex_lock(&_transfer_mutex);
	_transfer = swift::FileTransfer::file(getID());
	if(_transfer == NULL) {
		std::cout << "TRANSFER IS NULL" << std::endl;
	}
	pthread_mutex_unlock(&_transfer_mutex);
	
}

/**
 * Calculates current download and upload speeds.
 */
void Download::calculateSpeeds() {
	pthread_mutex_lock(&_transfer_mutex);
	int dspeed = _transfer->GetCurrentSpeed(swift::DDIR_DOWNLOAD);
	int uspeed = _transfer->GetCurrentSpeed(swift::DDIR_UPLOAD);
	pthread_mutex_unlock(&_transfer_mutex);
	
	setDownloadSpeed(dspeed/1024);
	setUploadSpeed(uspeed/1024);
}

/**
 * Calculates number of peers.
 */
void Download::calculatePeers() {
	pthread_mutex_lock(&_transfer_mutex);
	int seeders  = _transfer->GetNumSeeders();
	int leechers = _transfer->GetNumLeechers();
	pthread_mutex_unlock(&_transfer_mutex);
	
	setSeeders(seeders);
	setPeers(seeders + leechers);
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
 * Determines whether the download is complete
 */
bool Download::isComplete() {
	return swift::Complete(getID()) == swift::Size(getID());
}

/**
 * Setter for amount of seeders.
 * @param amount: The amount of seeders to be set.
 */
void Download::setSeeders(int amount) {
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
	if (getID() < 0 || getStatistics().download_speed == 0)
		return;
	
	struct time estimated_time;
	double speed            = getStatistics().download_speed;
	double time_in_seconds  = swift::Size(getID()) == 0 ? DBL_MAX : ( swift::Size(getID()) - swift::Complete(getID()) ) / (speed*1024);
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
 * Limits upload speed.
 */
void Download::limitUpSpeed(double speed) {
	pthread_mutex_lock(&_transfer_mutex);
	if (speed == 0) {
		_transfer->SetMaxSpeed(swift::DDIR_UPLOAD, DBL_MAX);
	} else {
		_transfer->SetMaxSpeed(swift::DDIR_UPLOAD, speed);
	}
	pthread_mutex_unlock(&_transfer_mutex);
}

/**
 * Limits download speed.
 */
void Download::limitDownSpeed(double speed) {
	pthread_mutex_lock(&_transfer_mutex);
	if (speed == 0) {
		_transfer->SetMaxSpeed(swift::DDIR_DOWNLOAD, DBL_MAX);
	} else {
		_transfer->SetMaxSpeed(swift::DDIR_DOWNLOAD, speed);
	}
	pthread_mutex_unlock(&_transfer_mutex);
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
	statistics.download_percentage = _stats.download_percentage;
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

