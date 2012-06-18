#ifndef _UTILS_H
#define _UTILS_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h>

#include "Exceptions.h"

#define DEFAULT_DOWNLOAD_DIR "/dtv/usb/sda1/Downloads"
#define DEFAULT_PORT 25000
#define DEFAULT_IP "127.0.0.1"

namespace Settings {
static std::string ip_address;			/// Keeps track of the current ip address.
static std::string download_directory;	/// Keeps track of the current download directory.
static pthread_mutex_t mutex;			/// Mutex to make download_directory variable thread safe.
	
	/**
	 * Sets the directory where swift will download files to.
	 * @param dir: The directory where swift will download files to.
	 */
	void setDownloadDirectory(std::string dir);
	
	/**
	 * Sets the current ip.
	 * @param ip: The ip to be set.
	 */
	void setIP(std::string ip);
	
	/**
	 * Returns the current ip.
	 */
	std::string getIP();
	
	/**
	 *  Returns the current download directory.
	 * @param ip: The ip to be set.
	 */
	std::string getDownloadDirectory();
	
	/**
	 * Initialises Utils.
	 */
	void init(std::string download_dir);
}

#endif
