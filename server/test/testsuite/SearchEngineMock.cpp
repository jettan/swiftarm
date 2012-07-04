#include "SearchEngine.h"

/**
 * Clears the list of search results
 */
void SearchEngine::clearSearchResults() {
	search_results.clear();
}

/**
 * Builds an XML string from the vector of results
 **/
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
std::string SearchEngine::getResults() {
	
	std::string xml_results = buildSearchXML();
	return xml_results;
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

/**
 * Call the main() from DispersyInterface.
 */
void *SearchEngine::startDispersy(void *arg) {
	std::cout << "Entered dispersy thread." <<std::endl;
}

/**
 * Search.
 */
void SearchEngine::search(std::string search_term) {
	clearSearchResults();
	
	struct SearchEngine::result r1;
	r1.tracker   = Settings::getIP() + ":9999";
	r1.hash      = "367d26a6ce626e049a21921100e24eac86dbcd32";
	r1.filename  = "Test1.test";
	search_results.push_back(r1);
	
	struct SearchEngine::result r2;
	r2.tracker   = Settings::getIP() + ":9999";
	r2.hash      = "012b5549e2622ea8bf3d694b4f55c959539ac848";
	r2.filename  = "Test2.test";
	search_results.push_back(r2);
}

/**
 * Init function to set up python calls and start dispersy.
 */
void SearchEngine::init() {
	
	std::cout <<  "SearchEngine Initialised." << std::endl;
}
