#ifndef _SEARCHENGINE_H
#define _SEARCHENGINE_H

#include <vector>
#include <string>
#include <iostream>

#include <pthread.h>

#include "Exceptions.h"
#include "Utils.h"
#include "ticpp.h"

namespace SearchEngine {
	
	static pthread_t dispersy_thread;
	static pthread_mutex_t dispersy_mutex;
	
	void *startDispersy(void *arg);
	
	struct result {
		std::string filename;
		std::string tracker;
		std::string hash;
	};
	
	static std::vector<struct result> search_results;
	static ticpp::Document *searchdoc;
	
	void clearSearchResults();
	std::string buildSearchXML();
	
	void search(std::string search_term);
	std::string getResults();
	
	struct result getResultWithHash(std::string hash);
	struct result getResultWithName(std::string name);
	
	void init();
}

#endif //_SEARCHENGINE_H
