#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>
#include <string>


class FileNotFoundException : public std::exception {
	public:
	virtual const char* what() const throw() {
		return "Could not find the file";
	}
	
};

class DownloadWhileStreamingException : public std::exception {
	public:
	virtual const char* what() const throw() {
		return "Cannot download during a stream";
	}
	
};

#endif //_EXCEPTIONS_H
