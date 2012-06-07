#include "DownloadManager.h"

void DownloadManager::init() {
pthread_mutex_init(&mutex, NULL);
pthread_mutex_init(&active_download_mutex, NULL);
}

void DownloadManager::setDownloadDirectory(std::string dir) {
	download_directory = dir;
	chdir(download_directory.c_str());
}

std::vector<Download> DownloadManager::getDownloads() {
	pthread_mutex_lock(&mutex);
	std::vector<Download> return_value = downloads;
	pthread_mutex_unlock(&mutex);
	
	return return_value;
}

std::string DownloadManager::getDownloadDirectory() {
	return download_directory;
}

void DownloadManager::setActiveDownload(Download *download) {
	pthread_mutex_lock(&active_download_mutex);
	active_download = download;
	pthread_mutex_unlock(&active_download_mutex);
}

void DownloadManager::updateDownloadStatistics() {
	
	
}

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
		ticpp::Text ratio_value(getDownloads().at(i).getStatistics().ratio);
		ratio_tag.LinkEndChild(&ratio_value);
		
		ticpp::Element upload_amount_tag("UPLOADAMOUNT");
		download_tag.LinkEndChild(&upload_amount_tag);
		ticpp::Text upload_amount_value(getDownloads().at(i).getStatistics().upload_amount);
		upload_amount_tag.LinkEndChild(&upload_amount_value);
		
		ticpp::Element download_amount_tag("DOWNLOADAMOUNT");
		download_tag.LinkEndChild(&download_amount_tag);
		ticpp::Text download_amount_value(getDownloads().at(i).getStatistics().download_amount);
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
	}
	
	TiXmlPrinter printer;
	
	doc->Accept(&printer);
	std::string xml_stats = printer.Str();
	
	return xml_stats;
}

/**
 * Callback to check if a download is completed.
 */
void downloadCallback(int fd, short event, void* arg) {
		pthread_mutex_lock(&DownloadManager::active_download_mutex);
		
		std::cout << "Download ID: " << DownloadManager::active_download->getID() << std::endl;
		std::cout << "Download Name: " << DownloadManager::active_download->getFilename() << std::endl;
		std::cout << "Percentage downloaded: " << floorf(((swift::Complete(DownloadManager::active_download->getID()) * 10000.0) / 
			swift::Size(DownloadManager::active_download->getID()) * 1.0) + 0.5) / 100 << std::endl;
		
	
	/*
	if (swift::SeqComplete(DownloadManager::getActiveDownload()->getID()) == swift::Size(DownloadManager::getActiveDownload()->getID())) {
		
		if (DownloadManager::getActiveDownload()->getStatus() != UPLOADING) {
			DownloadManager::getActiveDownload()->setStatus(UPLOADING);
			int index = DownloadManager::getIndexFromHash(DownloadManager::getActiveDownload()->getRootHash());
			
			if (index != DownloadManager::getDownloads().size() - 1) {
				DownloadManager::startDownload(DownloadManager::getDownloads().at(index + 1).getRootHash());
			}
		}
	}*/
		
	evtimer_add(&DownloadManager::evcompl, swift::tint2tv(TINT_SEC));
	pthread_mutex_unlock(&DownloadManager::active_download_mutex);
}

void* DownloadManager::dispatch(void* arg) {
	
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
 * Pauses the currently active download.
 */
void DownloadManager::pauseDownload(const std::string download_hash) {
	int index = getIndexFromHash(download_hash);
	std::cout << "Index = " << index << std::endl;
	
	if (index >= 0) {
		pthread_mutex_lock(&mutex);
		
		pthread_mutex_lock(&active_download_mutex);
		std::cout << "Found filename is: " << downloads.at(index).getFilename() << std::endl;
		downloads.at(index).pause();

		pthread_mutex_unlock(&active_download_mutex);
		
		pthread_mutex_unlock(&mutex);
	}
}

/**
 * Resumes a paused download.
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
 * Start a download with a specific download ID.
 */
int DownloadManager::startDownload(const std::string download_hash) {
	
	int index = getIndexFromHash(download_hash);
	std::cout << "Index = " << index << std::endl;
	
	if (index >= 0) {
		pthread_mutex_lock(&mutex);
		setActiveDownload(&downloads.at(index));
		pthread_mutex_unlock(&mutex);
		
		pthread_mutex_lock(&active_download_mutex);
		if (active_download->getID() > -1) {
			pthread_mutex_unlock(&active_download_mutex);
			return -1;
		} else {
			active_download->start();
			pthread_mutex_unlock(&active_download_mutex);
		}
		
		pthread_mutex_lock(&active_download_mutex);
		if (active_download->getID() < 0 ) {
			std::cerr << "Could not download " << active_download->getFilename() << "!" << std::endl;
			pthread_mutex_unlock(&active_download_mutex);
			return -1;
		}
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
 * Add a download to the list
 */
void DownloadManager::add(Download *download) {
	std::cout << "Adding download to vector.\n";
	
	for (int i = 0; i < getDownloads().size(); i++) {
		if (getDownloads().at(i).getRootHash().compare(download->getRootHash()) == 0) {
			return;
		}
	}
	
	int active_index = 0;
	
	if(active_download) {
	
		pthread_mutex_lock(&active_download_mutex);
		active_index = getIndexFromHash(active_download->getRootHash());
		pthread_mutex_unlock(&active_download_mutex);
	}
	
	pthread_mutex_lock(&mutex);
	downloads.push_back(*download);
	pthread_mutex_unlock(&mutex);
	
	if(active_download) {
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
 * Download first element in the download list.
 */
void DownloadManager::downloadFirstInList(){
	
	if(getDownloads().size() > 0) {
		startDownload(getDownloads().front().getRootHash());
	}
}

/**
 * Find the index of a download in the list based on the download ID.
 */
int DownloadManager::getIndexFromHash(const std::string download_hash) {
	
	for (int i = 0; i < getDownloads().size(); i++) {
		if (getDownloads().at(i).getRootHash().compare(download_hash) == 0) {
			std::cout << "Hash found: " << getDownloads().at(i).getRootHash() << std::endl;
			return i;
		}
	}
	return -1;
}

/**
 * Remove a download from the list based on the download ID
 */
void DownloadManager::removeFromList(const std::string download_hash) {
	
	int index = getIndexFromHash(download_hash);
	if(index >= 0) {
		if(getDownloads().at(index).getStatus() == DOWNLOADING) {
			getDownloads().at(index).stop();
			downloadFirstInList();
		}
		
		pthread_mutex_lock(&mutex);
		downloads.erase(downloads.begin() + index);
		pthread_mutex_unlock(&mutex);
	
	}
}

/** 
 * Remove a download from the hard disk based on download ID
 */
void DownloadManager::removeFromDisk(const std::string download_hash) {
	
	int index = getIndexFromHash(download_hash);
	
	if(index >= 0) {
		
		std::string filename = getDownloads().at(index).getFilename();
		removeFromList(download_hash);
		
		filename = download_directory + "/" + filename;
		
		if(filename.c_str() == 0) {
			// File removed successfully
			std::cout << "File at directory: " << filename << " has been removed" <<std::endl;
			
		} else {
			// File not found
			std::cout << "Could not find file at directory: " << filename <<std::endl;
		}
	}
}

/** 
 * Remove all downloads from the list
 */
void DownloadManager::clearList() {
	
	pthread_mutex_lock(&active_download_mutex);
	if(active_download != NULL){
		active_download->stop();
	}
	pthread_mutex_unlock(&active_download_mutex);
	
	pthread_mutex_lock(&mutex);
	downloads.clear();
	pthread_mutex_unlock(&mutex);
}

void DownloadManager::stopStream() {
	Stream::getInstance()->stop();
}

void* DownloadManager::startStreamThread(void* arg) {
	Stream::getInstance()->start();
	pthread_exit(NULL);
}

void DownloadManager::startStream(std::string tracker) {
	
	if (!Stream::getInstance()->readStreaming()) {
		Stream::getInstance()->setTracker(tracker);
		
		std::cout << "Spawning new thread..." << std::endl;
		int return_code = pthread_create(&streaming_thread, NULL, startStreamThread, NULL);
		
		if (return_code) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << return_code << "." << std::endl;
		}
	} else {
		std::cout << "Already Streaming!" <<std::endl;
	}
}

