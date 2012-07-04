#include "DownloadManager.h"

/**
 * Initialises the download manager.
 */
void DownloadManager::init() {
	active_download = NULL;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&active_download_mutex, NULL);
	
	setMaxDownSpeed(Settings::getMaxDownSpeed());
	setMaxUpSpeed(Settings::getMaxUpSpeed());
	std::cout << max_upspeed << std::endl;
	
	startUploads();
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
 * Only for testing purposes.
 * Returns the active_download variable.
 * Method is not thread safe, so refrain from using it.
 * Instead, acces active_download directly and lock it with the _active_download_mutex.
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
 * Fills in XML tags with the specified value.
 * @param value: The value to be set.
 * @param tag: The tag to be filled with the value.
 */
template <class Type>
void fillXMLValue(Type value, ticpp::Element *tag) {
	ticpp::Text text(value);
	tag->LinkEndChild(&text);
}

/**
 * Builds the SIZE tag for the statistics XML document.
 * @param index: index of the download from which the size is needed.
 * @param download_tag: The DOWNLOAD tag in which the SIZE tag is nested.
 */
void buildSizeTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element size_tag("SIZE");
	download_tag->LinkEndChild(&size_tag);
	fillXMLValue<double>(swift::Size(getDownloads().at(index).getID()) / (1024.0 * 1024.0), &size_tag);
}

/**
 * Builds the COMPLETED tag for the statistics XML document.
 * @param index: index of the download from which the size is needed.
 * @param download_tag: The DOWNLOAD tag in which the COMPLETED tag is nested.
 */
void buildCompletedTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element completed_tag("COMPLETED");
	download_tag->LinkEndChild(&completed_tag);
	fillXMLValue<double>((swift::Complete(getDownloads().at(index).getID()) / (1024.0 * 1024.0)), &completed_tag);
}

/**
 * Builds the STATUS tag for the statistics XML document.
 * @param index: index of the download from which the size is needed.
 * @param download_tag: The DOWNLOAD tag in which the STATUS tag is nested.
 */
void buildStatusTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element status_tag("STATUS");
	download_tag->LinkEndChild(&status_tag);
	fillXMLValue<int>(getDownloads().at(index).getStatus(), &status_tag);
}

/**
 * Builds the NAME tag for the statistics XML document.
 * @param index: index of the download from which the size is needed.
 * @param download_tag: The DOWNLOAD tag in which the NAME tag is nested.
 */
void buildNameTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element name_tag("NAME");
	download_tag->LinkEndChild(&name_tag);
	fillXMLValue<std::string>(getDownloads().at(index).getFilename(), &name_tag);
}

/**
 * Builds the DSPEED tag for the statistics XML document.
 * @param index: index of the download from which the size is needed.
 * @param download_tag: The DOWNLOAD tag in which the DSPEED tag is nested.
 */
void buildDSpeedTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element dspeed_tag("DSPEED");
	download_tag->LinkEndChild(&dspeed_tag);
	fillXMLValue<double>(getDownloads().at(index).getStatistics().download_speed, &dspeed_tag);
}

/**
 * Builds the USPEED tag for the statistics XML document.
 * @param index: index of the download from which the size is needed.
 * @param download_tag: The DOWNLOAD tag in which the USPEED tag is nested.
 */
void buildUSpeedTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element uspeed_tag("USPEED");
	download_tag->LinkEndChild(&uspeed_tag);
	fillXMLValue<double>(getDownloads().at(index).getStatistics().upload_speed, &uspeed_tag);
}

/**
 * Builds the PROGRESS tag for the statistics XML document.
 * @param index: index of the download from which the size is needed.
 * @param download_tag: The DOWNLOAD tag in which the PROGRESS tag is nested.
 */
void buildProgressTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element progress_tag("PROGRESS");
	download_tag->LinkEndChild(&progress_tag);
	fillXMLValue<double>(getDownloads().at(index).getStatistics().download_percentage, &progress_tag);
}

/**
 * Builds the RATIO tag for the statistics XML document.
 * @param download_tag: The DOWNLOAD tag in which the RATIO tag is nested.
 */
void buildRatioTag(ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element ratio_tag("RATIO");
	download_tag->LinkEndChild(&ratio_tag);
	fillXMLValue<double>(getRatio(), &ratio_tag);
}

/**
 * Builds the UPLOADAMOUNT tag for the statistics XML document.
 * @param download_tag: The DOWNLOAD tag in which the UPLOADAMOUNT tag is nested.
 */
void buildUploadAmountTag(ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element upload_amount_tag("UPLOADAMOUNT");
	download_tag->LinkEndChild(&upload_amount_tag);
	
	std::ostringstream upload_amount;
	upload_amount << getUploadAmount().amount << " " << getUploadAmount().unit;
	
	fillXMLValue<std::string>(upload_amount.str(), &upload_amount_tag);
}

/**
 * Builds the DOWNLOADAMOUNT tag for the statistics XML document.
 * @param download_tag: The DOWNLOAD tag in which the DOWNLOADAMOUNT tag is nested.
 */
void buildDownloadAmountTag(ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element download_amount_tag("DOWNLOADAMOUNT");
	download_tag->LinkEndChild(&download_amount_tag);
	
	std::ostringstream download_amount;
	download_amount << getDownloadAmount().amount << " " << getDownloadAmount().unit;
	
	fillXMLValue<std::string>(download_amount.str(), &download_amount_tag);
}

/**
 * Builds the SEEDERS tag for the statistics XML document.
 * @param index: index of the download from which the no. of seeders is needed.
 * @param download_tag: The DOWNLOAD tag in which the SEEDERS tag is nested.
 */
void buildSeedersTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element seeders_tag("SEEDERS");
	download_tag->LinkEndChild(&seeders_tag);
	fillXMLValue<int>(getDownloads().at(index).getStatistics().seeders, &seeders_tag);
}

/**
 * Builds the PEERS tag for the statistics XML document.
 * @param index: index of the download from which the no. of peers is needed.
 * @param download_tag: The DOWNLOAD tag in which the PEERS tag is nested.
 */
void buildPeersTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element peers_tag("PEERS");
	download_tag->LinkEndChild(&peers_tag);
	fillXMLValue<int>(getDownloads().at(index).getStatistics().peers, &peers_tag);
}

/**
 * Builds the HASH tag for the statistics XML document.
 * @param index: index of the download from which the root hash is needed.
 * @param download_tag: The DOWNLOAD tag in which the HASH tag is nested.
 */
void buildHashTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element hash_tag("HASH");
	download_tag->LinkEndChild(&hash_tag);
	fillXMLValue<std::string>(getDownloads().at(index).getRootHash(), &hash_tag);
}

/**
 * Builds the TIME tags for the statistics XML document.
 * @param index: index of the download from which the estimated times are needed.
 * @param download_tag: The DOWNLOAD tag in which the TIME tags are nested.
 */
void buildTimeTag(int index, ticpp::Element *download_tag) {
	using namespace DownloadManager;
	
	ticpp::Element timedays_tag("TIMEDAYS");
	download_tag->LinkEndChild(&timedays_tag);
	
	ticpp::Element timehours_tag("TIMEHOURS");
	download_tag->LinkEndChild(&timehours_tag);
	
	ticpp::Element timeminutes_tag("TIMEMINUTES");
	download_tag->LinkEndChild(&timeminutes_tag);
	
	ticpp::Element timeseconds_tag("TIMESECONDS");
	download_tag->LinkEndChild(&timeseconds_tag);
	
	if (getDownloads().at(index).getStatus() == DOWNLOADING) {
		fillXMLValue<int>(getDownloads().at(index).getStatistics().estimated.days, &timedays_tag);
		fillXMLValue<int>(getDownloads().at(index).getStatistics().estimated.hours, &timehours_tag);
		fillXMLValue<int>(getDownloads().at(index).getStatistics().estimated.minutes, &timeminutes_tag);
		fillXMLValue<int>(getDownloads().at(index).getStatistics().estimated.seconds, &timeseconds_tag);
		
	} else if(getDownloads().at(index).getStatus() == PAUSED) {
		fillXMLValue<char>('-', &timedays_tag);
		fillXMLValue<char>('-', &timehours_tag);
		fillXMLValue<char>('-', &timeminutes_tag);
		fillXMLValue<char>('-', &timeseconds_tag);
		
	} else {
		fillXMLValue<int>(0, &timedays_tag);
		fillXMLValue<int>(0, &timehours_tag);
		fillXMLValue<int>(0, &timeminutes_tag);
		fillXMLValue<int>(0, &timeseconds_tag);
	}
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
		
		buildSizeTag(i, &download_tag);
		buildCompletedTag(i, &download_tag);
		buildStatusTag(i, &download_tag);
		buildNameTag(i, &download_tag);
		buildDSpeedTag(i, &download_tag);
		buildUSpeedTag(i, &download_tag);
		buildProgressTag(i, &download_tag);
		buildRatioTag(&download_tag);
		buildUploadAmountTag(&download_tag);
		buildDownloadAmountTag(&download_tag);
		buildSeedersTag(i, &download_tag);
		buildPeersTag(i, &download_tag);
		buildHashTag(i, &download_tag);
		buildTimeTag(i, &download_tag);
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
		evtimer_add(&DownloadManager::evcompl, swift::tint2tv(TINT_SEC));
	} else {
			
		pthread_mutex_lock(&DownloadManager::active_download_mutex);
		
		// Temporary debug prints.
		std::cout << "Download ID: " << DownloadManager::active_download->getID() << std::endl;
		std::cout << "Download Name: " << DownloadManager::active_download->getFilename() << std::endl;
		std::cout << "Status: " << DownloadManager::active_download->getStatus() << std::endl;
		
		if (DownloadManager::active_download->getStatus() == UPLOADING || DownloadManager::active_download->getStatus() == DOWNLOADING){
			std::cout << "Percentage downloaded: " << floorf(((swift::Complete(DownloadManager::active_download->getID()) * 10000.0) /
			swift::Size(DownloadManager::active_download->getID()) * 1.0) + 0.5) / 100 << std::endl;
		}
		bool is_unlocked = false;
		
		if (swift::SeqComplete(DownloadManager::active_download->getID()) == swift::Size(DownloadManager::active_download->getID()) ||
		    DownloadManager::active_download->getStatus() == PAUSED) {
			
			if (DownloadManager::active_download->getStatus() != UPLOADING && DownloadManager::active_download->getStatus() != PAUSED) {
				DownloadManager::active_download->setStatus(UPLOADING);
			}
			
			int i = 0;
			while (i < DownloadManager::getDownloads().size() &&
			  DownloadManager::getDownloads().at(i).getStatus() != READY) {
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
 * @param arg: Unused argument of p`thread_create.
 */
void *DownloadManager::dispatch(void* arg) {
	// Assign callbacks to the event base.
	evtimer_assign(&evcompl, swift::Channel::evbase, downloadCallback, NULL);
	evtimer_add(&evcompl, swift::tint2tv(TINT_SEC));
	
	// Dispatch the event base to enter the swift loop.
	
	event_base_dispatch(swift::Channel::evbase);
	std::cout << "Dispatched event base." << std::endl;
	pthread_exit(NULL);
}

/**
 * Pauses a download with the given root hash.
 * @param download_hash: The root hash of the download.
 */
void DownloadManager::pauseDownload(const std::string download_hash) {
	int index = getIndexFromHash(download_hash);
	
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
	
	pthread_mutex_lock(&active_download_mutex);
	if (active_download->getRootHash().compare(download_hash) == 0) {
		
		active_download->resume();
		pthread_mutex_unlock(&active_download_mutex);
		
	} else if (index >= 0 && index < getDownloads().size() && getDownloads().at(index).getStatus() == PAUSED) {
		
		pthread_mutex_unlock(&active_download_mutex);
		
		if (getDownloads().at(index).getStatus() == PAUSED && getDownloads().at(index).getID() > -1) {
			pthread_mutex_lock(&active_download_mutex);
			std::string active_hash = active_download->getRootHash();
			pthread_mutex_unlock(&active_download_mutex);
			
			pthread_mutex_lock(&active_download_mutex);
			if (active_download->getStatus() == DOWNLOADING && !getDownloads().at(index).isComplete()) {
				pauseDownload(active_hash);
			}
			pthread_mutex_unlock(&active_download_mutex);
			
			pthread_mutex_lock(&mutex);
			setActiveDownload(&downloads.at(index));
			pthread_mutex_unlock(&mutex);
			
			pthread_mutex_lock(&active_download_mutex);
			active_download->resume();
			active_download->limitDownSpeed(max_downspeed);
			active_download->limitUpSpeed(max_upspeed);
			pthread_mutex_unlock(&active_download_mutex);
		} else {
			CannotResumeException *e = new CannotResumeException();
			throw *e;
		}
	} else {
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
	
	if (index >= 0) {
		pthread_mutex_lock(&mutex);
		setActiveDownload(&downloads.at(index));
		pthread_mutex_unlock(&mutex);
		
		pthread_mutex_lock(&active_download_mutex);
		active_download->start();
		
		active_download->limitDownSpeed(max_downspeed);
		active_download->limitUpSpeed(max_upspeed);
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
		catch (FileNotFoundException e) {
			active_download->setStatus(original_state);
			std::cout << "FileNotFoundException caught in switchDownload, exception was thrown" << std::endl;
			throw e;
		}
	}
	else {
		pthread_mutex_unlock(&active_download_mutex);
		AlreadyDownloadingException *e = new AlreadyDownloadingException();
		throw *e;
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
	
	pthread_mutex_lock(&mutex);
	downloads.push_back(*download);
	pthread_mutex_unlock(&mutex);
	
	if (active_download) {
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
 * Checks whether a file exists.
 * @param filename: Name of the file to be checked.
 */
bool fileExists(const std::string filename) {
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1) {
		return true;
	}
	
	return false;
}

/**
 * Uploads a file.
 * @param filename: Name of the file to be uploaded.
 */
void DownloadManager::upload(std::string filename) {
	if (fileExists(filename)) {
		std::string root_hash = "";
		int id = swift::Open(filename.c_str(), root_hash.c_str(), swift::Address());
		
		if (id < 0) {
			return;
		}
		
		root_hash = swift::RootMerkleHash(id).hex().c_str();
		
		std::ostringstream stream;
		stream << Settings::getIP() << ":" << DEFAULT_PORT;
		
		Download file(stream.str(), root_hash, filename);
		file.setID(id);
		
		try {
			add(&file);
		} catch (AlreadyDownloadingException e) {
			std::cout << "Exception caught in startUploads()" << std::endl;
			std::cout << e.what() << std::endl;
		}
	} else {
		FileNotFoundException *e = new FileNotFoundException();
		throw *e;
	}
}

/**
 * Starts all Uploads located in the public folder.
 */
void DownloadManager::startUploads() {
	DIR *dp;
	struct dirent *dirp;
	
	if ((dp = opendir(Settings::getDownloadDirectory().c_str())) == NULL) {
		std::cout << "Failed opening Downloads directory" << std::endl;
	}
	
	while ((dirp = readdir(dp)) != NULL) {
		std::string filename(dirp->d_name);
		if (filename.at(0) != '.' && filename.find(".mhash") == std::string::npos && filename.find(".mbinmap") == std::string::npos) {
			upload(filename.c_str());
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
 * Returns the maximum download speed in kb/s.
 */
double DownloadManager::getMaxDownSpeed() {
	return max_downspeed;
}

/**
 * Returns the maximum download speed in kb/s.
 */
double DownloadManager::getMaxUpSpeed() {
	return max_upspeed;
}

/**
 * Sets the maximum download speed in kb/s.
 * @param speed: speed in byte/s.
 */
void DownloadManager::setMaxDownSpeed(double speed) {
	if (speed > 0) {
		max_downspeed = speed * 1024;
	} else {
		max_downspeed = DBL_MAX;
	}
	
	for (int i = 0; i < getDownloads().size(); i++) {
		pthread_mutex_lock(&mutex);
		downloads.at(i).limitDownSpeed(max_downspeed);
		pthread_mutex_unlock(&mutex);
	}
}

/**
 * Sets the maximum upload speed in kb/s.
 * @param speed: speed in byte/s.
 */
void DownloadManager::setMaxUpSpeed(double speed) {
	if (speed > 0) {
		max_upspeed = speed * 1024;
	} else {
		max_upspeed = DBL_MAX;
	}
	
	for (int i = 0; i < getDownloads().size(); i++) {
		pthread_mutex_lock(&mutex);
		downloads.at(i).limitUpSpeed(max_upspeed);
		pthread_mutex_unlock(&mutex);
	}
}

/**
 * Sets the maximum download speed in kb/s.
 * @param speed: speed in kb/s.
 */
void DownloadManager::limitDownSpeeds(double speed) {
	if (speed > 0) {
		Settings::setMaxDownSpeed(speed * 1024);
	} else {
		Settings::setMaxDownSpeed(DBL_MAX);
	}
	
	for (int i = 0; i < getDownloads().size(); i++) {
		pthread_mutex_lock(&mutex);
		downloads.at(i).limitDownSpeed(Settings::getMaxDownSpeed());
		pthread_mutex_unlock(&mutex);
	}
}

/**
 * Sets the maximum upload speed in kb/s.
 * @param speed: speed in kb/s.
 */
void DownloadManager::limitUpSpeeds(double speed) {
	if (speed > 0) {
		Settings::setMaxUpSpeed(speed * 1024);
	} else {
		Settings::setMaxUpSpeed(DBL_MAX);
	}

	for (int i = 0; i < getDownloads().size(); i++) {
		pthread_mutex_lock(&mutex);
		downloads.at(i).limitUpSpeed(Settings::getMaxUpSpeed());
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
		
		filename = Settings::getDownloadDirectory() + "/" + filename;
		
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
	if (Stream::getInstance()->readStreaming()) {
	Stream::getInstance()->stop();
	
		if(active_download) {
			pthread_mutex_lock(&active_download_mutex);
			active_download->resume();
			pthread_mutex_unlock(&active_download_mutex);
		}
	}
}

/**
 * Pause all the downloads.
 */
void DownloadManager::pauseAllDownloads() {
	for (int i = 0; i < getDownloads().size(); i++) {
		if (getDownloads().at(i).getStatus() == DOWNLOADING) {
			
			pauseDownload(downloads.at(i).getRootHash());
			std::cout << "Paused: " << downloads.at(i).getFilename() << std::endl;
		}
	}
}

/**
 * Resume all the downloads.
 */
void DownloadManager::resumeAllDownloads() {
	for (int i = 0; i < getDownloads().size(); i++) {
		if (getDownloads().at(i).getStatus() == PAUSED) {
			
			resumeDownload(downloads.at(i).getRootHash());
			std::cout << "Resumed: " << downloads.at(i).getFilename() << std::endl;
		}
	}
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
void DownloadManager::startStream(std::string tracker, std::string hash) {
	if(active_download) {
		pthread_mutex_lock(&active_download_mutex);
		active_download->pause();
		pthread_mutex_unlock(&active_download_mutex);
	}
	
	if (!Stream::getInstance()->readStreaming()) {
		Stream::getInstance()->setTracker(tracker);
		Stream::getInstance()->setRoothash(hash);
		
		std::cout << "Spawning new thread..." << std::endl;
		int return_code = pthread_create(&streaming_thread, NULL, startStreamThread, NULL);
		
		if (return_code) {
			std::cerr << "ERROR: failed to create stream thread. Code: " << return_code << "." << std::endl;
		}
	} else {
		std::cout << "Already Streaming!" << std::endl;
	}
}

