#include "SearchEngine.h"

/**
 * Generates a new list of result based on the search term
 * and returns the list.
 * @param search_term: Keyword used to find files.
 */
//std::vector<struct SearchEngine::result> SearchEngine::search(std::string search_term) {
std::string SearchEngine::search(std::string search_term) {
	
	// For now search results will be hard coded
	// Once dispersy works we can do the real deal
	
	search_results.clear();
	
	std::string tracker     = "130.161.158.52:20000";
	//std::string tracker     = "127.0.0.1:20000";
	std::string root_hash   = "012b5549e2622ea8bf3d694b4f55c959539ac848";
	std::string name        = "stream.mp4";
	
	struct SearchEngine::result r;
	r.tracker = tracker;
	r.hash = root_hash;
	r.filename = name;
	
	root_hash = "367d26a6ce626e049a21921100e24eac86dbcd32";
	name      = "SG.mkv";
	
	struct SearchEngine::result r2;
	r2.tracker = tracker;
	r2.hash = root_hash;
	r2.filename = name;
	
	search_results.push_back(r);
	search_results.push_back(r2);
	
	std::string result = buildSearchXML();
	return result;
}

void SearchEngine::clearSearchResults() {
	
	search_results.clear();
}

std::string SearchEngine::buildSearchXML() {
	searchdoc = new ticpp::Document("searchdoc");
	
	ticpp::Declaration dec("1.0", "", "");
	searchdoc->LinkEndChild(&dec);
	
	ticpp::Element results_tag("RESULTS");
	searchdoc->LinkEndChild(&results_tag);
	
	for (int i = 0; i < search_results.size(); i++) {
		ticpp::Element result_tag("RESULT");
		results_tag.LinkEndChild(&result_tag);
		
		ticpp::Element name_tag("NAME");
		result_tag.LinkEndChild(&name_tag);
		ticpp::Text name_value(search_results.at(i).filename);
		name_tag.LinkEndChild(&name_value);
		
		ticpp::Element tracker_tag("TRACKER");
		result_tag.LinkEndChild(&tracker_tag);
		ticpp::Text tracker_value(search_results.at(i).tracker);
		tracker_tag.LinkEndChild(&tracker_value);
		
		ticpp::Element hash_tag("HASH");
		result_tag.LinkEndChild(&hash_tag);
		ticpp::Text hash_value(search_results.at(i).hash);
		hash_tag.LinkEndChild(&hash_value);
	}
	
	TiXmlPrinter printer;
	
	searchdoc->Accept(&printer);
	std::string result = printer.Str();
	
	return result;
}

/**
 * Returns the current list of results.
 */
std::vector<struct SearchEngine::result> SearchEngine::getResults() {
	
	// For now search results will be hard coded.
	// Once dispersy works we can do the real deal.
	
	return search_results;
}

/**
 * Returns the result with a cetain hash.
 */
struct SearchEngine::result SearchEngine::getResultWithHash(std::string hash) {
	
	for (int i = 0; i < search_results.size(); i++) {
		
		if (search_results.at(i).hash.compare(hash) == 0) {
			return search_results.at(i);
		}
	}
	
	// Throw exception if file can't be found.
	FileNotFoundException *e = new FileNotFoundException();
	throw *e;
}

/**
 * Returns the result with a cetain filename.
 */
struct SearchEngine::result SearchEngine::getResultWithName(std::string name) {
	
	for(int i = 0; i < search_results.size(); i++) {
		
		if(search_results.at(i).filename.compare(name) == 0) {
			return search_results.at(i);
		}
	}
	
	// Throw exception if file can't be found.
	FileNotFoundException *e = new FileNotFoundException();
	throw *e;
}

