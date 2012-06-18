#include "Utils.h"

/**
 * Sets the directory where swift will download files to.
 * @param dir: The directory where swift will download files to.
 */
void Settings::setDownloadDirectory(std::string dir) {
	pthread_mutex_lock(&mutex);
	download_directory = dir;
	chdir(download_directory.c_str());
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
	setDownloadDirectory(download_dir);
	
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

