#ifndef _UTILS_H
#define _UTILS_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h>
#include <vector>

#include "Exceptions.h"

#define DEFAULT_SETTINGS_FILE "/mtd_down/widgets/user/SamyGO/SamyGO/.p2psettings"
#define DEFAULT_DOWNLOAD_DIR  "/dtv/usb/sda1/Downloads"
#define DEFAULT_IP            "127.0.0.1"
#define DEFAULT_PORT          7758
#define DHT_PORT              9999

/**
 * General Settings manager.
 */
namespace Settings {
	/// Keeps track of the current ip address.
	static std::string ip_address;
	
	/// Keeps track of the current download directory.
	static std::string download_directory;
	
	/// Mutex to make download_directory variable thread safe.
	static pthread_mutex_t mutex;
	
	/// Mutex to make max speed variables thread safe.
	static pthread_mutex_t max_mutex;
	
	/// Maximum download speed.
	static double max_download_speed;
	
	/// Maximum upload speed.
	static double max_upload_speed;
	
	/// Settings file.
	static std::string settings_file;
	
	void init(std::string download_dir);
	void loadSettings(std::string filename);
	void saveSettings(std::vector<std::string> settings);
	void setDownloadDirectory(std::string dir);
	void setIP(std::string ip);
	void setMaxUpSpeed(double speed);
	void setMaxDownSpeed(double speed);
	
	std::vector<std::string> &split(const std::string &str, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &str, char delim);
	std::string getIP();
	std::string getDownloadDirectory();
	
	bool directoryExists(std::string path);
	
	double getMaxUpSpeed();
	double getMaxDownSpeed();
	
	std::string replaceSubstring(std::string str, const std::string from, const std::string to);
}

#endif
