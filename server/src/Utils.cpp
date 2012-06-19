#include "Utils.h"

/**
 * Used to tokenize strings.
 */
std::vector<std::string> &Settings::split(const std::string &str, char delim, std::vector<std::string> &elems) {
	std::stringstream stream(str);
	std::string item;
	
	while(std::getline(stream, item, delim)) {
		elems.push_back(item);
	}
	
	return elems;
}

/**
 * Used to tokenize strings.
 */
std::vector<std::string> Settings::split(const std::string &str, char delim) {
	std::vector<std::string> elems;
	return split(str, delim, elems);
}

/**
 * Loads the settings file.
 * @param filename: Settings file to be loaded.
 */
void Settings::loadSettings(std::string filename) {
	std::string line;
	std::ifstream settings;
	
	settings.open(DEFAULT_SETTINGS_FILE);
	if (settings.is_open()) {
		
		getline(settings, line);
		std::cout << line << std::endl;
		std::vector<std::string> result = split(line, ';');
		
		if (result.at(0).compare("max_down_speed")) {
			std::string max_down_speed_str = result.at(0);
			
			double max_down_speed_value = std::atof(split(max_down_speed_str, ':').at(1).c_str());
			
			setMaxDownSpeed(max_down_speed_value);
		}
		
		if (result.at(1).compare("max_up_speed")) {
			std::string max_up_speed_str = result.at(1);
			
			double max_up_speed_value = std::atof(split(max_up_speed_str, ':').at(1).c_str());
			
			setMaxUpSpeed(max_up_speed_value);
		}
		
		if (result.at(2).compare("download_dir")) {
			std::string download_dir_str = result.at(2);
			
			std::string download_dir_value = split(download_dir_str, ':').at(1);
			
			setDownloadDirectory(download_dir_value);
		}
		settings.close();
	}
	else {
		std::string settings = "/settings:0:0:/dtv/usb/sda1/Downloads";
		saveSettings(split(settings, ':'));
		setDownloadDirectory(DEFAULT_DOWNLOAD_DIR);
		setMaxDownSpeed(0);
		setMaxUpSpeed(0);
	}
}

/**
 * Saves the settings.
 */
void Settings::saveSettings(std::vector<std::string> settings) {
//std::cout << remove(DEFAULT_SETTINGS_FILE) << std::endl;
	std::ofstream settings_file;
	settings_file.open(DEFAULT_SETTINGS_FILE, std::ios_base::out | std::ios_base::trunc);
	
	if (!settings_file.is_open()) {
		FileNotFoundException *e = new FileNotFoundException();
		throw *e;
	
	} else {
		settings_file << "max_down_speed:" << settings.at(1) << ";";
		settings_file << "max_up_speed:" << settings.at(2) << ";";
		settings_file << "download_dir:" << settings.at(3) << ";";
	}
	
	settings_file.close();
}

bool Settings::directoryExists(std::string path) {
	if (access(path.c_str(), 0 ) == 0) {
		struct stat status;
		stat(path.c_str(), &status);
		
		if (status.st_mode & S_IFDIR) {
			std::cout << "The directory exists." << std::endl;
			return true;
		} else {
			std::cout << "The path you entered is a file." << std::endl;
		}
	} else {
		std::cout << "Path doesn't exist." << std::endl;
	}
	
	return false;
}

/**
 * Sets the directory where swift will download files to.
 * @param dir: The directory where swift will download files to.
 */
void Settings::setDownloadDirectory(std::string dir) {
	pthread_mutex_lock(&mutex);
	
	if (directoryExists(dir)) {
		download_directory = dir;
		chdir(download_directory.c_str());
	} else {
		std::cout << "No device found, going to /tmp" << std::endl;
		download_directory = "/tmp/Downloads";
		mkdir("/tmp/Downloads", 0777);
		chdir(download_directory.c_str());
	}
	pthread_mutex_unlock(&mutex);
}

/**
 * Sets the current ip.
 * @param ip: The ip to be set.
 */
void Settings::setIP(std::string ip) {
	struct sockaddr_in sa;
	
	if (inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1) {
		ip_address = ip;
	} else {
		InvalidIPException *e = new InvalidIPException();
		throw *e;
	}
}

/**
 * Returns the current ip.
 */
std::string Settings::getIP() {
	return ip_address;
}

/**
 * Returns the maximum download speed.
 */
double Settings::getMaxDownSpeed() {
	pthread_mutex_lock(&max_mutex);
	double res = max_download_speed;
	pthread_mutex_unlock(&max_mutex);
	return res;
}

/**
 * Returns the maximum upload speed.
 */
double Settings::getMaxUpSpeed() {
	pthread_mutex_lock(&max_mutex);
	double res = max_upload_speed;
	pthread_mutex_unlock(&max_mutex);
	return res;
}

/**
 * Sets the maximum download speed.
 */
void Settings::setMaxDownSpeed(double speed) {
	pthread_mutex_lock(&max_mutex);
	max_download_speed = speed;
	pthread_mutex_unlock(&max_mutex);
}

/**
 * Sets the maximum upload speed.
 */
void Settings::setMaxUpSpeed(double speed) {
	pthread_mutex_lock(&max_mutex);
	max_upload_speed = speed;
	pthread_mutex_unlock(&max_mutex);
}

/**
 *  Returns the current download directory.
 * @param ip: The ip to be set.
 */
std::string Settings::getDownloadDirectory() {
	pthread_mutex_lock(&mutex);
	std::string res = download_directory;
	pthread_mutex_unlock(&mutex);
	return res;
}

/**
 * Initialises Utils.
 */
void Settings::init(std::string download_dir) {
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&max_mutex, NULL);
	
	loadSettings(DEFAULT_SETTINGS_FILE);
	
	// Getting the IP address.
	struct ifaddrs * ifAddrStruct = NULL;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;
	std::string ip = DEFAULT_IP;
	setIP(ip);
	
	getifaddrs(&ifAddrStruct);
	
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		
		if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
			
			// is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			if(strcmp(ifa->ifa_name, "eth0") == 0 || strcmp(ifa->ifa_name, "eth1") == 0 || strcmp(ifa->ifa_name, "wlan0") == 0) {
				printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
				ip = std::string(addressBuffer);
				setIP(ip);
			}
		}
	}
	
	if (ifAddrStruct!=NULL) {
		freeifaddrs(ifAddrStruct);
	}
	
	std::cout << "In set method " << download_directory << std::endl;
}

