#ifndef _SEARCHENGINE_H
#define _SEARCHENGINE_H

#include <vector>
#include <string>
#include <iostream>

#include <pthread.h>

#include "Exceptions.h"
#include "Settings.h"
#include "ticpp.h"

/**
 * Interface to Dispersy module.
 * Makes use of Dispersy methods to find files online.
 */
namespace SearchEngine {
	/// Thread to run dispersy module in.
	static pthread_t dispersy_thread;
	
	/// Mutex to protect dispersy thread.
	static pthread_mutex_t dispersy_mutex;
	
	/// Vector of all search results.
	static std::vector<struct result> search_results;
	
	/// Document used to build the list of search results.
	static ticpp::Document *searchdoc;
	
	/// Struct holding the data of a search result.
	struct result {
		std::string filename;
		std::string tracker;
		std::string hash;
	};
	
	void *startDispersy(void *arg);
	void clearSearchResults();
	std::string buildSearchXML();
	
	void search(std::string search_term);
	std::string getResults();
	
	struct result getResultWithHash(std::string hash);
	struct result getResultWithName(std::string name);
	
	void init();
}

#endif //_SEARCHENGINE_H
