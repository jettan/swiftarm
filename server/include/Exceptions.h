#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>
#include <string>


class FileNotFoundException : public std::exception {
	public:
	
	FileNotFoundException() throw() {}
	~FileNotFoundException() throw() {}
	
	virtual const char* what() const throw() {
		return "Could not find the file";
	}
	
};

class DownloadWhileStreamingException : public std::exception {
	public:
	
	DownloadWhileStreamingException() throw() {}
	~DownloadWhileStreamingException() throw() {}
	
	virtual const char* what() const throw() {
		return "Cannot download during a stream";
	}
	
};

class AlreadyDownloadingException : public std::exception {
	public:
	
	AlreadyDownloadingException() throw() {}
	~AlreadyDownloadingException() throw() {}
	
	virtual const char* what() const throw() {
		return "This file is already being downloaded";
	}
	
};

#endif //_EXCEPTIONS_H
