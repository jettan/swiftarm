#include "DownloadManager.h"

/**
 * Initialises the download manager.
 */
void DownloadManager::init(std::string download_dir) {
	active_download = NULL;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&active_download_mutex, NULL);
	setDownloadDirectory(download_dir);
	startUploads();
}

/**
 * Sets the directory where swift will download files to.
 * @param dir: The directory where swift will download files to.
 */
void DownloadManager::setDownloadDirectory(std::string dir) {
	download_directory = dir;
	chdir(download_directory.c_str());
}

/**
 * Returns a copy of the vector downloads.
 */
std::vector<Download> DownloadManager::getDownloads() {
	pthread_mutex_lock(&mutex);
	std::vector<Download> return_value = downloads;
	pthread_mutex_unlock(&mutex);
	
	return return_value;
}

/**
 * Returns the download directory as string.
 */
std::string DownloadManager::getDownloadDirectory() {
	return download_directory;
}

/**
 * Sets the active download.
 * @param download: The download to be the active download.
 */
void DownloadManager::setActiveDownload(Download *download) {
	pthread_mutex_lock(&active_download_mutex);
	active_download = download;
	pthread_mutex_unlock(&active_download_mutex);
}

/**
 * Calculates the upload/download ratio.
 */
void DownloadManager::calculateRatio() {
	ratio = uploaded / downloaded;
}

/**
 * Calculates the download amount.
 */
void DownloadManager::calculateDownloadAmount() {
	downloaded = swift::Channel::global_raw_bytes_down / 1024;
}

/**
 * Calculates the upload amount.
 */
void DownloadManager::calculateUploadAmount() {
	uploaded = swift::Channel::global_raw_bytes_up / 1024;
}

/**
 *
 */
Download DownloadManager::getActiveDownload() {
	
	return *active_download;
}

/**
 * Updates the download statistics.
 */
void DownloadManager::updateDownloadStatistics() {
	
	pthread_mutex_lock(&mutex);
	pthread_mutex_lock(&active_download_mutex);
	
	for (int i = 0; i < downloads.size(); i++) {
		if (downloads.at(i).getStatus() == UPLOADING || downloads.at(i).getStatus() == DOWNLOADING) {
			
			int id = downloads.at(i).getID();
			double progress = floorf(((swift::Complete(id) * 10000.0) / swift::Size(id) * 1.0) + 0.5) / 100;
			downloads.at(i).setProgress(progress);
			downloads.at(i).calculateSpeeds();
			downloads.at(i).calculateEstimatedTime();
			downloads.at(i).calculatePeers();
		}
	}
	calculateDownloadAmount();
	calculateUploadAmount();
	calculateRatio();
	
	pthread_mutex_unlock(&active_download_mutex);
	pthread_mutex_unlock(&mutex);
}

/**
 * Builds the XML response for the HTTP server, providing download statistics.
 */
std::string DownloadManager::buildXML() {
	updateDownloadStatistics();
	
	doc = new ticpp::Document("doc");
	
	ticpp::Declaration dec( "1.0", "", "");
	doc->LinkEndChild(&dec);
	
	ticpp::Element downloads_tag("DOWNLOADS");
	
	doc->LinkEndChild(&downloads_tag);
	
	for(int i = 0; i < getDownloads().size(); i++) {
		
		ticpp::Element download_tag("DOWNLOAD");
		downloads_tag.LinkEndChild(&download_tag);
		
		ticpp::Element status_tag("STATUS");
		download_tag.LinkEndChild(&status_tag);
		ticpp::Text status_value(getDownloads().at(i).getStatus());
		status_tag.LinkEndChild(&status_value);
		
		ticpp::Element name_tag("NAME");
		download_tag.LinkEndChild(&name_tag);
		ticpp::Text name_value(getDownloads().at(i).getFilename());
		name_tag.LinkEndChild(&name_value);
		
		ticpp::Element dspeed_tag("DSPEED");
		download_tag.LinkEndChild(&dspeed_tag);
		ticpp::Text dspeed_value(getDownloads().at(i).getStatistics().download_speed);
		dspeed_tag.LinkEndChild(&dspeed_value);
		
		ticpp::Element uspeed_tag("USPEED");
		download_tag.LinkEndChild(&uspeed_tag);
		ticpp::Text uspeed_value(getDownloads().at(i).getStatistics().upload_speed);
		uspeed_tag.LinkEndChild(&uspeed_value);
		
		ticpp::Element progress_tag("PROGRESS");
		download_tag.LinkEndChild(&progress_tag);
		ticpp::Text progress_value(getDownloads().at(i).getStatistics().download_percentage);
		progress_tag.LinkEndChild(&progress_value);
		
		ticpp::Element ratio_tag("RATIO");
		download_tag.LinkEndChild(&ratio_tag);
		ticpp::Text ratio_value(getRatio());
		ratio_tag.LinkEndChild(&ratio_value);
		
		ticpp::Element upload_amount_tag("UPLOADAMOUNT");
		download_tag.LinkEndChild(&upload_amount_tag);
		
		std::ostringstream upload_amount;
		upload_amount << getUploadAmount().amount << " " << getUploadAmount().unit;
		
		ticpp::Text upload_amount_value(upload_amount.str());
		upload_amount_tag.LinkEndChild(&upload_amount_value);
		
		ticpp::Element download_amount_tag("DOWNLOADAMOUNT");
		download_tag.LinkEndChild(&download_amount_tag);
		
		std::ostringstream download_amount;
		download_amount << getDownloadAmount().amount << " " << getDownloadAmount().unit;
		
		ticpp::Text download_amount_value(download_amount.str());
		download_amount_tag.LinkEndChild(&download_amount_value);
		
		ticpp::Element seeders_tag("SEEDERS");
		download_tag.LinkEndChild(&seeders_tag);
		ticpp::Text seeders_value(getDownloads().at(i).getStatistics().seeders);
		seeders_tag.LinkEndChild(&seeders_value);
		
		ticpp::Element peers_tag("PEERS");
		download_tag.LinkEndChild(&peers_tag);
		ticpp::Text peers_value(getDownloads().at(i).getStatistics().peers);
		peers_tag.LinkEndChild(&peers_value);
		
		ticpp::Element timedays_tag("TIMEDAYS");
		download_tag.LinkEndChild(&timedays_tag);
		ticpp::Text timedays_value(getDownloads().at(i).getStatistics().estimated.days);
		timedays_tag.LinkEndChild(&timedays_value);
		
		ticpp::Element timehours_tag("TIMEHOURS");
		download_tag.LinkEndChild(&timehours_tag);
		ticpp::Text timehours_value(getDownloads().at(i).getStatistics().estimated.hours);
		timehours_tag.LinkEndChild(&timehours_value);
		
		ticpp::Element timeminutes_tag("TIMEMINUTES");
		download_tag.LinkEndChild(&timeminutes_tag);
		ticpp::Text timeminutes_value(getDownloads().at(i).getStatistics().estimated.minutes);
		timeminutes_tag.LinkEndChild(&timeminutes_value);
		
		ticpp::Element timeseconds_tag("TIMESECONDS");
		download_tag.LinkEndChild(&timeseconds_tag);
		ticpp::Text timeseconds_value(getDownloads().at(i).getStatistics().estimated.seconds);
		timeseconds_tag.LinkEndChild(&timeseconds_value);
		
		ticpp::Element hash_tag("HASH");
		download_tag.LinkEndChild(&hash_tag);
		ticpp::Text hash_value(getDownloads().at(i).getRootHash());
		hash_tag.LinkEndChild(&hash_value);
	}
	
	TiXmlPrinter printer;
	
	doc->Accept(&printer);
	std::string xml_stats = printer.Str();
	
	return xml_stats;
}

/**
 * Callback to keep all downloads and uploads running.
 * @param fd: The file descriptor used by swift.
 * @param event: The event it gets from libevent.
 * @param arg: Unused argument from libevent.
 */
void downloadCallback(int fd, short event, void* arg) {
	if (DownloadManager::getDownloads().size() == 0) {
		std::cout << "You shall not pass!" << std::endl;
		evtimer_add(&DownloadManager::evcompl, swift::tint2tv(TINT_SEC));
	} else {
			
		pthread_mutex_lock(&DownloadManager::active_download_mutex);
		
		// Temporary debug prints.
		std::cout << "Download ID: " << DownloadManager::active_download->getID() << std::endl;
		std::cout << "Download Name: " << DownloadManager::active_download->getFilename() << std::endl;
		std::cout << "Status: " << DownloadManager::active_download->getStatus() << std::endl;
		
		if (DownloadManager::active_download->getStatus() == UPLOADING || DownloadManager::active_download->getStatus() == DOWNLOADING)
			std::cout << "Percentage downloaded: " << floorf(((swift::Complete(DownloadManager::active_download->getID()) * 10000.0) /
			  swift::Size(DownloadManager::active_download->getID()) * 1.0) + 0.5) / 100 << std::endl;
		
		bool is_unlocked = false;
			
		if (swift::SeqComplete(DownloadManager::active_download->getID()) == swift::Size(DownloadManager::active_download->getID()) ||
		  DownloadManager::active_download->getStatus() == PAUSED) {
			
			if (DownloadManager::active_download->getStatus() != UPLOADING && DownloadManager::active_download->getStatus() != PAUSED) {
				DownloadManager::active_download->setStatus(UPLOADING);
			}
			
			int i = 0;
			while (i < DownloadManager::getDownloads().size() &&
			  DownloadManager::getDownloads().at(i).getStatus() != READY &&
			  DownloadManager::getDownloads().at(i).getStatus() != PAUSED) {
				i++;
			}
			
			if (DownloadManager::getDownloads().size() > 0 && i < DownloadManager::getDownloads().size()) {
				if (DownloadManager::getDownloads().at(i).getStatus() == READY) {
					pthread_mutex_unlock(&DownloadManager::active_download_mutex);
					is_unlocked = true;
					DownloadManager::startDownload(DownloadManager::getDownloads().at(i).getRootHash());
				}
			}
		}
		
		if (!is_unlocked) {
			pthread_mutex_unlock(&DownloadManager::active_download_mutex);
		}
		
		evtimer_add(&DownloadManager::evcompl, swift::tint2tv(TINT_SEC));
	}
}

/**
 * Dispatches swift::Channel::evbase to get into the main loop to download files.
 * @param arg: Unused argument of pthread_create.
 */
void *DownloadManager::dispatch(void* arg) {
	// Assign callbacks to the event base.
	std::cout << "Entered thread." << std::endl;
	evtimer_assign(&evcompl, swift::Channel::evbase, downloadCallback, NULL);
	evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	
	// Dispatch the event base to enter the swift loop.
	
	std::cout << "Now dispatching event base." << std::endl;
	event_base_dispatch(swift::Channel::evbase);
	std::cout << "Download ID: " << active_download->getID() << std::endl;
	pthread_exit(NULL);
}

/**
 * Pauses a download with the given root hash.
 * @param download_hash: The root hash of the download.
 */
void DownloadManager::pauseDownload(const std::string download_hash) {
	std::cout << "pausing" << std::endl;
	int index = getIndexFromHash(download_hash);
	std::cout << "Index = " << index << std::endl;
	
	if (index >= 0) {
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&active_download_mutex);
		
		downloads.at(index).pause();
		
		pthread_mutex_unlock(&active_download_mutex);
		pthread_mutex_unlock(&mutex);
	}
}

/**
 * Resumes a paused download given a root hash.
 * @param download_hash: The root hash of the download.
 */
int DownloadManager::resumeDownload(std::string download_hash) {
	int index = getIndexFromHash(download_hash);
	
	if (index >= 0) {
		pthread_mutex_lock(&mutex);
		setActiveDownload(&downloads.at(index));
		pthread_mutex_unlock(&mutex);
		
		pthread_mutex_lock(&active_download_mutex);
		
		if (active_download->getID() > -1) {
			active_download->resume();
		} else {
			pthread_mutex_unlock(&active_download_mutex);
			return -1;
		}
		
		pthread_mutex_unlock(&active_download_mutex);
	}
	return 0;
}

/**
 * Starts a download given a root hash.
 * @param download_hash: The root hash of the download.
 */
int DownloadManager::startDownload(const std::string download_hash) {
	int index = getIndexFromHash(download_hash);
	std::cout << "Index = " << index << std::endl;
	
	if (index >= 0) {
		pthread_mutex_lock(&mutex);
		setActiveDownload(&downloads.at(index));
		pthread_mutex_unlock(&mutex);
		
		pthread_mutex_lock(&active_download_mutex);
		active_download->start();
		pthread_mutex_unlock(&active_download_mutex);
		
		if (d_pid != 0) {
			d_pid = pthread_create(&thread, NULL, dispatch, NULL);
			if (d_pid) {
				std::cerr << "Could not create download thread!" << std::endl;
				return -1;
			}
			std::cout << "Download pid = " << d_pid << std::endl;
		}
	}
	return 0;
}

/**
 * Switches active download to another download with the given root hash.
 * @param hash: The root hash of the download to be switched to.
 */
void DownloadManager::switchDownload(std::string hash) {
	pthread_mutex_lock(&active_download_mutex);
	std::string previous_hash = active_download->getRootHash();
	
	if (previous_hash.compare(hash) != 0) {
		int original_state = active_download->getStatus();
		try {
			
			active_download->setStatus(SWITCHING);
			pthread_mutex_unlock(&active_download_mutex);
			
			startDownload(hash);
			pauseDownload(previous_hash);
		}
		catch(FileNotFoundException e) {
			active_download->setStatus(original_state);
			std::cout << "FileNotFoundException caught in switchDownload, exception was thrown" << std::endl;
			throw e;
		}
	}
	else {
		pthread_mutex_unlock(&active_download_mutex);
	}
}

/**
 * Adds a download to the list.
 * @param download: The download to be added.
 */
void DownloadManager::add(Download *download) {
	
	if (Stream::getInstance()->readStreaming()) {
		std::cout << "Cannot add download when streaming" << std::endl;
		DownloadWhileStreamingException *exception = new DownloadWhileStreamingException();
		throw *exception;
	}
		
	// Only add a new download that is not alredy in the list.
	for (int i = 0; i < getDownloads().size(); i++) {
		if (getDownloads().at(i).getRootHash().compare(download->getRootHash()) == 0) {
		std::cout << "Cannot add same download" << std::endl;
			AlreadyDownloadingException *exception = new AlreadyDownloadingException();
			throw *exception;
		}
	}
	
	int active_index = 0;
	
	if (active_download) {
		pthread_mutex_lock(&active_download_mutex);
		active_index = getIndexFromHash(active_download->getRootHash());
		pthread_mutex_unlock(&active_download_mutex);
	}
	
	//download->limitDownSpeed(max_downspeed);
	//download->limitUpSpeed(max_upspeed);

	pthread_mutex_lock(&mutex);
	downloads.push_back(*download);
	pthread_mutex_unlock(&mutex);
	
	if (active_download) {
		pthread_mutex_lock(&active_download_mutex);
		std::cout << "Active Index is: " << getIndexFromHash(active_download->getRootHash()) << std::endl;
		pthread_mutex_unlock(&active_download_mutex);
		
		pthread_mutex_lock(&mutex);
		setActiveDownload(&downloads.at(active_index));
		pthread_mutex_unlock(&mutex);
		
		pthread_mutex_lock(&active_download_mutex);
		std::cout << "Active Index is: " << getIndexFromHash(active_download->getRootHash()) << std::endl;
		pthread_mutex_unlock(&active_download_mutex);
	}
	std::cout << "Added download.\n";
	
	for (int i = 0; i < getDownloads().size(); i++) {
		std::cout << "Element " << i << " of vector: " << getDownloads().at(i).getFilename() << std::endl;
	}
	
	if (getDownloads().size() == 1) {
		std::cout << "Starting first download.\n";
		downloadFirstInList();
	}
}

/**
 * Downloads first element in the download list.
 */
void DownloadManager::downloadFirstInList() {
	if (getDownloads().size() > 0) {
		startDownload(getDownloads().front().getRootHash());
	}
}

/**
 * Starts all Uploads located in the public folder.
 */
void DownloadManager::startUploads() {
	DIR *dp;
	struct dirent *dirp;
	std::string root_hash = "";
	
	if((dp = opendir(download_directory.c_str())) == NULL) {
		std::cout << "Failed opening Downloads directory" << std::endl;
	}
	
	while ((dirp = readdir(dp)) != NULL) {
		std::string filename(dirp->d_name);
		if (filename.at(0) != '.' && filename.find(".mhash") == std::string::npos && filename.find(".mbinmap") == std::string::npos) {
			int id = swift::Open(dirp->d_name, root_hash.c_str(), swift::Address());
			std::cout << dirp->d_name << std::endl;
			
			if (id < 0) {
				return;
			}
			
			root_hash = swift::RootMerkleHash(id).hex().c_str();
			Download file("130.161.158.60:20000", root_hash, filename);
			file.setID(id);
			add(&file);
		}
	}
	closedir(dp);
}

/**
 * Finds the index of a download in the list given a root hash.
 * @param download_hash: The root hash of a download to be searched.
 */
int DownloadManager::getIndexFromHash(const std::string download_hash) {
	for (int i = 0; i < getDownloads().size(); i++) {
		if (getDownloads().at(i).getRootHash().compare(download_hash) == 0) {
			return i;
		}
	}
	FileNotFoundException *e = new FileNotFoundException();
	throw *e;
}

/**
 * Returns the downloaded amount in kb, MB or GB.
 */
struct DownloadManager::Amount DownloadManager::getDownloadAmount() {
	struct Amount result;
	result.amount = downloaded;
	result.unit   = "kb";
	
	if (result.amount > 1024*1024) {
		result.amount = downloaded / (1024*1024);
		result.unit = "GB";
	} else if (result.amount > 1024) {
		result.amount = downloaded / 1024;
		result.unit = "MB";
	}
	
	return result;
}

/**
 * Returns the uploaded amount in kb, MB or GB.
 */
struct DownloadManager::Amount DownloadManager::getUploadAmount() {
	struct Amount result;
	result.amount = uploaded;
	result.unit   = "kb";
	
	if (result.amount > 1024*1024) {
		result.amount = uploaded / (1024*1024);
		result.unit = "GB";
	} else if (result.amount > 1024) {
		result.amount = uploaded / 1024;
		result.unit = "MB";
	}
	
	return result;
}

/**
 * Returns the upload/download ratio.
 */
double DownloadManager::getRatio() {
	return ratio;
}

/**
 * Returns the maximum download speed.
 */
double DownloadManager::getMaxDownSpeed() {
	return max_downspeed;
}

/**
 * Returns the maximum upload speed.
 */
double DownloadManager::getMaxUpSpeed() {
	return max_upspeed;
}

/**
 * Sets the maximum download speed.
 */
void DownloadManager::setMaxDownSpeed(double speed) {
	max_downspeed = speed;
	
	for (int i = 0; i < getDownloads().size(); i++) {
		pthread_mutex_lock(&mutex);
		downloads.at(i).limitDownSpeed(max_downspeed);
		pthread_mutex_unlock(&mutex);
	}
}

/**
 * Sets the maximum upload speed.
 */
void DownloadManager::setMaxUpSpeed(double speed) {
	max_upspeed = speed;
	
	for (int i = 0; i < getDownloads().size(); i++) {
		pthread_mutex_lock(&mutex);
		downloads.at(i).limitUpSpeed(max_upspeed);
		pthread_mutex_unlock(&mutex);
	}
}

/**
 * Removes a download from the list given a root hash.
 * @param download_hash: The root hash of the download to be removed.
 */
void DownloadManager::removeFromList(const std::string download_hash) {
	int index = getIndexFromHash(download_hash);
	
	if (index >= 0) {
		if (getDownloads().at(index).getStatus() == DOWNLOADING || getDownloads().at(index).getStatus() == UPLOADING) {
			
			pthread_mutex_lock(&mutex);
			pthread_mutex_lock(&active_download_mutex);
			
			downloads.at(index).stop();
			
			active_download = NULL;
			free(active_download);
			
			pthread_mutex_unlock(&active_download_mutex);
			pthread_mutex_unlock(&mutex);
			
		}
		
		pthread_mutex_lock(&mutex);
		downloads.erase(downloads.begin() + index);
		pthread_mutex_unlock(&mutex);
		
		if(active_download == NULL) {
			downloadFirstInList();
		}
	}
}

/**
 * Removes a download from the hard disk given a root hash.
 * @param download_hash: The root hash of the download to be removed.
 */
void DownloadManager::removeFromDisk(const std::string download_hash) {
	
	int index = getIndexFromHash(download_hash);
	
	if (index >= 0) {
		std::string filename = getDownloads().at(index).getFilename();
		removeFromList(download_hash);
		
		filename = download_directory + "/" + filename;
		
		if (remove(filename.c_str()) != 0) {
			// File removed successfully
			std::cout << "File at directory: " << filename << " has been removed" << std::endl;
		} else {
			// File not found
			std::cout << filename << " already deleted." << std::endl;
		}
	}
}

/**
 * Removes all downloads from the list
 */
void DownloadManager::clearList() {
	for (int i = 0; i < getDownloads().size(); i++) {
		pthread_mutex_lock(&mutex);
		downloads.at(i).stop();
		pthread_mutex_unlock(&mutex);
	}
	
	pthread_mutex_lock(&mutex);
	
	pthread_mutex_lock(&active_download_mutex);
	
	downloads.clear();
	
	active_download = NULL;
	free(active_download);
	pthread_mutex_unlock(&active_download_mutex);
	
	pthread_mutex_unlock(&mutex);
	
}

/**
 * Stops streaming.
 */
void DownloadManager::stopStream() {
	Stream::getInstance()->stop();
}

/**
 * Starts streaming.
 * @param arg: Unused argument of pthread_create.
 */
void* DownloadManager::startStreamThread(void* arg) {
	Stream::getInstance()->start();
	
	if (d_pid != 0) {
		event_base_dispatch(swift::Channel::evbase);
	}
	
	std::cout << "Exiting stream thread." << std::endl;
	pthread_exit(NULL);
}

/**
 * Starts the streaming thread.
 * @param tracker: The tracker from which we stream content.
 */
void DownloadManager::startStream(std::string tracker) {
	for (int i = 0; i < downloads.size(); i++) {
		if (downloads.at(i).getStatus() == DOWNLOADING) {
			pauseDownload(downloads.at(i).getRootHash());
			std::cout << downloads.at(i).getFilename() << std::endl;
		}
	}
	
	if (!Stream::getInstance()->readStreaming()) {
		Stream::getInstance()->setTracker(tracker);
		
		std::cout << "Spawning new thread..." << std::endl;
		int return_code = pthread_create(&streaming_thread, NULL, startStreamThread, NULL);
		
		if (return_code) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << return_code << "." << std::endl;
		}
	} else {
		std::cout << "Already Streaming!" << std::endl;
	}
}

