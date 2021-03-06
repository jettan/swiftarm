#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * Exception thrown when a file is not found.
 */
class FileNotFoundException : public std::exception {
	public:
	
	FileNotFoundException() throw() {}
	~FileNotFoundException() throw() {}
	
	virtual const char* what() const throw() {
		return "Could not find the file.";
	}
	
};

/**
 * Exception thrown when a download attempt is made during streaming.
 */
class DownloadWhileStreamingException : public std::exception {
	public:
	
	DownloadWhileStreamingException() throw() {}
	~DownloadWhileStreamingException() throw() {}
	
	virtual const char* what() const throw() {
		return "Cannot download during a stream.";
	}
	
};

/**
 * Exception thrown when a download is added which was already added before.
 */
class AlreadyDownloadingException : public std::exception {
	public:
	
	AlreadyDownloadingException() throw() {}
	~AlreadyDownloadingException() throw() {}
	
	virtual const char* what() const throw() {
		return "This file is already being downloaded.";
	}
	
};

/**
 * Exception thrown when the format of the IP address is incorrect.
 */
class InvalidIPException : public std::exception {
	public:
	
	InvalidIPException() throw() {}
	~InvalidIPException() throw() {}
	
	virtual const char* what() const throw() {
		return "The given IP address is invalid.";
	}
	
};

/**
 * Exception thrown when something went wrong while trying to resume a download.
 */
class CannotResumeException : public std::exception {
	public:
	
	CannotResumeException() throw() {}
	~CannotResumeException() throw() {}
	
	virtual const char* what() const throw() {
		return "This download can not be resumed. It was probably never started and paused.";
	}
	
};

#endif //_EXCEPTIONS_H

