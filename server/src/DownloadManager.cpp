#include "DownloadManager.h"

void DownloadManager::setDownloadDirectory(std::string dir) {
	download_directory = dir;
	chdir(download_directory.c_str());
}

std::string DownloadManager::getDownloadDirectory(){
	return download_directory;
}

Download* DownloadManager::getActiveDownload(){
	return active_download;
}

// Download first element in the download list
void DownloadManager::downloadFirstInList(){
	
	if(downloads.size() > 0) {
		startDownload(downloads.front().getFilename());
		active_download = &downloads.front();
	}
}

/**
 * Callback to check if a download is completed.
 */
void downloadCallback(int fd, short event, void* arg) {
//	std::cout << "Download ID: " << DownloadManager::active_download->getID() << std::endl;
//	std::cout << "Percentage downloaded: " << floorf(((swift::Complete(DownloadManager::active_download->getID()) * 10000.0) / 
//	        swift::Size(DownloadManager::active_download->getID()) * 1.0) + 0.5) / 100 << std::endl;
	
	evtimer_add(&DownloadManager::evcompl, swift::tint2tv(TINT_SEC));
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
	
	for(int i = 0; i < downloads.size(); i++) {
		
		ticpp::Element download("DOWNLOAD");
		
		downloads_tag.LinkEndChild(&download);
		
		ticpp::Element name("NAME");
		download.LinkEndChild(&name);
		ticpp::Text name_value(downloads.at(i).getFilename());
		name.LinkEndChild(&name_value);
		
		ticpp::Element dspeed("DSPEED");
		download.LinkEndChild(&dspeed);
		ticpp::Text dspeed_value(downloads.at(i).getStatistics().download_speed);
		dspeed.LinkEndChild(&dspeed_value);
		
		ticpp::Element uspeed("USPEED");
		download.LinkEndChild(&uspeed);
		ticpp::Text uspeed_value(downloads.at(i).getStatistics().upload_speed);
		uspeed.LinkEndChild(&uspeed_value);
		
		ticpp::Element progress("PROGRESS");
		download.LinkEndChild(&progress);
		ticpp::Text progress_value(downloads.at(i).getStatistics().download_percentage);
		progress.LinkEndChild(&progress_value);
		
		ticpp::Element ratio("RATIO");
		download.LinkEndChild(&ratio);
		ticpp::Text ratio_value(downloads.at(i).getStatistics().ratio);
		ratio.LinkEndChild(&ratio_value);
		
		ticpp::Element upload_amount("UPLOADAMOUNT");
		download.LinkEndChild(&upload_amount);
		ticpp::Text upload_amount_value(downloads.at(i).getStatistics().upload_amount);
		upload_amount.LinkEndChild(&upload_amount_value);
		
		ticpp::Element download_amount("DOWNLOADAMOUNT");
		download.LinkEndChild(&download_amount);
		ticpp::Text download_amount_value(downloads.at(i).getStatistics().download_amount);
		download_amount.LinkEndChild(&download_amount_value);
		
		ticpp::Element seeders("SEEDERS");
		download.LinkEndChild(&seeders);
		ticpp::Text seeders_value(downloads.at(i).getStatistics().seeders);
		seeders.LinkEndChild(&seeders_value);
		
		ticpp::Element peers("PEERS");
		download.LinkEndChild(&peers);
		ticpp::Text peers_value(downloads.at(i).getStatistics().peers);
		peers.LinkEndChild(&peers_value);
		
		ticpp::Element timedays("TIMEDAYS");
		download.LinkEndChild(&timedays);
		ticpp::Text timedays_value(downloads.at(i).getStatistics().estimated.days);
		timedays.LinkEndChild(&timedays_value);
		
		ticpp::Element timehours("TIMEHOURS");
		download.LinkEndChild(&timehours);
		ticpp::Text timehours_value(downloads.at(i).getStatistics().estimated.hours);
		timehours.LinkEndChild(&timehours_value);
		
		ticpp::Element timeminutes("TIMEMINUTES");
		download.LinkEndChild(&timeminutes);
		ticpp::Text timeminutes_value(downloads.at(i).getStatistics().estimated.minutes);
		timeminutes.LinkEndChild(&timeminutes_value);
		
		ticpp::Element timeseconds("TIMESECONDS");
		download.LinkEndChild(&timeseconds);
		ticpp::Text timeseconds_value(downloads.at(i).getStatistics().estimated.seconds);
		timeseconds.LinkEndChild(&timeseconds_value);
		
		
	}
	
	TiXmlPrinter printer;
	
	doc->Accept(&printer);
	std::string xml_stats = printer.Str();
	
	return xml_stats;
	
}

void* DownloadManager::dispatch(void* arg) {
	
	// Assign callbacks to the event base.
	std::cout << "Entered thread." << std::endl;
	evtimer_assign(&evcompl, swift::Channel::evbase, downloadCallback, NULL);
	evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	
	// Dispatch the event base to enter the swift loop.
	
	std::cout << "Now dispatching event base." << std::endl;
	event_base_dispatch(swift::Channel::evbase);
	
	pthread_exit(NULL);
}

/**
 * Start a download with a specific download ID.
 */
void DownloadManager::startDownload(const std::string download_name) {
	if (active_download) {
		std::cout << "Pausing current download.\n";
		active_download->pause();
		std::cout << "Paused active download.\n";
	}
	
	int index = getIndexFromName(download_name);
	std::cout << "Index = " << index << std::endl;
	if (index >= 0) {
		active_download = &downloads.at(index);
		if (active_download->getID() > -1) {
			active_download->resume();
		} else {
			active_download->start();
		}
		
		if (active_download->getID() < 0 ) {
			std::cerr << "Could not download " << active_download->getFilename() << "!" << std::endl;
			return;
		}
		
		if(d_pid != 0) {
			d_pid = pthread_create(&thread, NULL, dispatch, NULL);
			if (d_pid) {
				std::cerr << "Could not create download thread!" << std::endl;
				return;
			}
			std::cout << "Download pid = " << d_pid << std::endl;
		}
	}
	else if(active_download) {
		active_download->resume();
	}
}

/**
 * Add a download to the list
 */
void DownloadManager::add(Download *download) {
	std::cout << "Adding download to vector.\n";
	downloads.push_back(*download);
	std::cout << "Added download.\n";
	
	for (int i = 0; i < downloads.size(); i++) {
		std::cout << "Element " << i << " of vector: " << downloads.at(i).getFilename() << std::endl;
	}
	
	//if(downloads.size() == 1) {
	//	downloadFirstInList();
	//}
}

/**
 * Find the index of a download in the list based on the download ID
 */
int DownloadManager::getIndexFromName(const std::string download_name) {
	
	for (int i = 0; i < downloads.size(); i++) {
		if (downloads.at(i).getFilename().compare(download_name) == 0) {
			std::cout << "Name found: " << downloads.at(i).getFilename() << std::endl;
			return i;
		}
	}
	return -1;
}

/**
 * Remove a download from the list based on the download ID
 */
void DownloadManager::removeFromList(const std::string download_name) {
	
	int index = getIndexFromName(download_name);
	if(index >= 0) {
		if(downloads.at(index).getStatus() == DOWNLOADING) {
			downloads.at(index).stop();
			downloadFirstInList();
		}
		
		downloads.erase(downloads.begin() + index);
	
	}
}

/** 
 * Remove a download from the hard disk based on download ID
 */
void DownloadManager::removeFromDisk(const std::string download_name) {
	
	int index = getIndexFromName(download_name);
	
	if(index >= 0) {
		
		std::string filename = downloads.at(index).getFilename();
		removeFromList(download_name);
		
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
	
	if(active_download != NULL){
		active_download->stop();
	}
	downloads.clear();
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

