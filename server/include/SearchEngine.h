#ifndef _SEARCHENGINE_H
#define _SEARCHENGINE_H

#include <vector>
#include <string>
#include <iostream>

#include <pthread.h>

#include "Exceptions.h"
#include "Settings.h"
#include "ticpp.h"

namespace SearchEngine {
	
	static pthread_t dispersy_thread;					/// Thread to run dispersy module in.
	static pthread_mutex_t dispersy_mutex;				/// Mutex to protect dispersy thread.
	static std::vector<struct result> search_results;	/// Vector of all search results.
	static ticpp::Document *searchdoc;					/// Document used to build the list of search results.
	
	void *startDispersy(void *arg);
	
	/// Struct holding the data of a search result.
	struct result {
		std::string filename;
		std::string tracker;
		std::string hash;
	};
	
	void clearSearchResults();
	std::string buildSearchXML();
	
	void search(std::string search_term);
	std::string getResults();
	
	struct result getResultWithHash(std::string hash);
	struct result getResultWithName(std::string name);
	
	void init();
}

#endif //_SEARCHENGINE_H
