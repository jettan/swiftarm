#ifndef _SEARCHENGINE_H
#define _SEARCHENGINE_H

#include <vector>
#include <string>
#include <iostream>

#include "HttpServer.h"
#include "Exceptions.h"
#include "ticpp.h"

namespace SearchEngine {
	
	struct result {
		std::string filename;
		std::string tracker;
		std::string hash;
	};
	
	static std::vector<struct result> search_results;
	static ticpp::Document *searchdoc;
	
	void clearSearchResults();
	std::string buildSearchXML();
	
	std::string search(std::string search_term);
	std::vector<struct result> getResults();
	
	struct result getResultWithHash(std::string hash);
	struct result getResultWithName(std::string name);
}

#endif //_SEARCHENGINE_H
