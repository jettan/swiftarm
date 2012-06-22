#include "SearchEngine.h"
#include <Python.h>

namespace SearchEngine {
	PyObject *p_module_name, *p_module, *p_main, *p_function;
	PyObject *p_args, *p_main_value, *p_function_value;
	PyObject *p_result_string;
	PyGILState_STATE gstate;
	pthread_mutex_t gstate_mutex;
	PyThreadState *save;
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
std::string SearchEngine::getResults() {
	clearSearchResults();
	p_function = PyObject_GetAttrString(p_module, "getSearchResults");
	
	if (p_function && PyCallable_Check(p_function)) {
		std::cout << "Function callable." << std::endl;
		gstate = PyGILState_Ensure();
		
		p_function_value = PyObject_CallObject(p_function, NULL);
		int py_list_size = PyList_Size(p_function_value);
		
		for (int i = 0; i < py_list_size; i++) {
			p_result_string              = PyList_GetItem(p_function_value, i);
			std::string result_string    = PyString_AsString(p_result_string);
			
			std::vector<std::string> item = Settings::split(result_string, ':');
			struct SearchEngine::result r;
			r.tracker   = Settings::getIP() + ":9999";
			r.hash      = item[0];
			r.filename  = item[1];
			search_results.push_back(r);
		}
		
		PyGILState_Release(gstate);
	} else {
		if (PyErr_Occurred())
			PyErr_Print();
		fprintf(stderr, "Could not find function getSearchResults()");
	}
	Py_XDECREF(p_function);
	Py_DECREF(p_function_value);
	// For now search results will be hard coded.
	// Once dispersy works we can do the real deal.
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
	p_main_value = PyObject_CallObject(p_main, NULL);
	
	pthread_mutex_lock(&gstate_mutex);
	PyGILState_Release(gstate);
	pthread_mutex_unlock(&gstate_mutex);
	
	Py_Finalize();
	std::cout << "For some reason the dispersy thread has kicked you out." << std::endl;
	pthread_exit(NULL);
}

/**
 * Search.
 */
void SearchEngine::search(std::string search_term) {
	clearSearchResults();
	
	p_function = PyObject_GetAttrString(p_module, "search");
	
	if (p_function && PyCallable_Check(p_function)) {
		std::cout << "Function callable." << std::endl;
		pthread_mutex_lock(&gstate_mutex);
		gstate = PyGILState_Ensure();
		pthread_mutex_unlock(&gstate_mutex);
		
		p_args = PyTuple_New(1);
		PyTuple_SetItem(p_args, 0, PyString_FromString(search_term.c_str()));
		
		p_function_value = PyObject_CallObject(p_function, p_args);
		
		pthread_mutex_lock(&gstate_mutex);
		PyGILState_Release(gstate);
		pthread_mutex_unlock(&gstate_mutex);
		
		std::cout << "Called the function" << std::endl;
	} else {
		if (PyErr_Occurred())
			PyErr_Print();
		fprintf(stderr, "Could not find function search()");
	}
	Py_XDECREF(p_function);
	Py_DECREF(p_function_value);
}

/**
 * Init function to set up python calls and start dispersy.
 */
void SearchEngine::init() {
	pthread_mutex_init(&gstate_mutex, NULL);
	pthread_mutex_init(&dispersy_mutex, NULL);
	
	// Initialize python.
	Py_Initialize();
	
	// The module or file name we want to import.
	p_module_name = PyString_FromString("DispersyInterface");
	
	// Import DispersyInterface.py.
	p_module = PyImport_Import(p_module_name);
	
	// Garbage cleaning.
	Py_DECREF(p_module_name);
	
	if (p_module != NULL) {
		// Get the main() python function.
		p_main = PyObject_GetAttrString(p_module, "main");
		
		if (p_main && PyCallable_Check(p_main)) {
			// Start the blocking main() function in another thread.
			int pid = pthread_create(&dispersy_thread, NULL, startDispersy, NULL);
			if (pid) {
				std::cerr << "Could not spawn dispersy thread." << std::endl;
			}
		} else {
			if (PyErr_Occurred())
				PyErr_Print();
			std::cerr <<  "Could not find function main()" << std::endl;
		}
		//Py_XDECREF(p_main);
	} else {
		PyErr_Print();
		std::cerr <<  "Failed to load DispersyInterface" << std::endl;
		return;
	}
}

