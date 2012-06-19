#ifndef _UTILS_H
#define _UTILS_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
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
#define DEFAULT_PORT          25000

namespace Settings {
	static std::string ip_address;			/// Keeps track of the current ip address.
	static std::string download_directory;	/// Keeps track of the current download directory.
	static pthread_mutex_t mutex;			/// Mutex to make download_directory variable thread safe.
	static pthread_mutex_t max_mutex;		/// Mutex to make max speed variables thread safe.
	static double max_download_speed;		/// Maximum download speed.
	static double max_upload_speed;			/// Maximum upload speed.
	static std::string settings_file;		/// Settings file.
	
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
	
	double getMaxUpSpeed();
	double getMaxDownSpeed();
}

#endif
