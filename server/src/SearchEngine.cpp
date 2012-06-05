#include "SearchEngine.h"

// Generates a new list of results based on the search term
// and returns the list
std::vector<struct SearchEngine::result> SearchEngine::search(std::string searchTerm) {
	
	// For now search results will be hard coded
	// Once dispersie works we can do the real deal
	
	searchResults.clear();
	
	std::string tracker     = "127.0.0.1:20000";
	std::string root_hash   = "012b5549e2622ea8bf3d694b4f55c959539ac848";
	std::string name        = "bla.mp4";
	
	struct SearchEngine::result r;
	r.tracker = tracker;
	r.hash = root_hash;
	r.filename = name;
	
	searchResults.push_back(r);
	
	return searchResults;
}

// Returns the current list of results
std::vector<struct SearchEngine::result> SearchEngine::getResults() {
	
	// For now search results will be hard coded
	// Once dispersie works we can do the real deal
	
	return searchResults;
}

// Returns the result with a certain filename
struct SearchEngine::result SearchEngine::getResultWithName(std::string filename) {
	
	for(int i = 0; i < searchResults.size(); i++) {
		
		if(searchResults.at(i).filename.compare(filename) == 0) {
			return searchResults.at(i);
		}
	}
	
	// Throw exception if file can't be found
	throw 1;
}
