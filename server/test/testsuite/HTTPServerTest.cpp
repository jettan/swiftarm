#include <string>

#include "curl.h"
#include "easy.h"
#include "HttpServer.h"
#include "gtest.h"
#include "SearchEngine.h"
#include "DownloadManager.h"

/**
 * This is the test class for HttpServer.cpp
 * The setup initialises the Http Client in C++
 * and also clears the search engine list and downloads list
 * The teardown is empty
 */
class HTTPServerTest : public ::testing::Test {
	protected:
	
	CURL *easyHandle;
	CURLcode res;
	std::string response;
	
	virtual ~HTTPServerTest() {}
	
	/**
	 * Returns the Http response
	 */
	size_t static getResponse(char *response, size_t size, size_t count, void *stream) {
		
		((std::string*)stream)->append((char*)response, 0, size * count);
		return size*count;
	}
	
	/**
	 * Do a search in SearchEngine.cpp
	 */
	void search() {
		
		std::string addr1 = Settings::getIP() + ":1337/search:test";
		curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
		res = curl_easy_perform(easyHandle);
	}
	
	/**
	 * Turns an XML string into a vector of results
	 */
	std::vector<struct SearchEngine::result> toVector(std::string response) {
		
		std::vector<struct SearchEngine::result> results;
		ticpp::Document doc;
		doc.Parse(response, true, TIXML_DEFAULT_ENCODING);
		
		ticpp::Element *list = doc.FirstChildElement();
		if (list->NoChildren()) {
			return results;
			
		} else {
			ticpp::Element *result = list->FirstChildElement();
			
			while (result) {
				
				ticpp::Element *info = result->FirstChildElement();
				
				struct SearchEngine::result r;
				while (info) {
					
					if(info->Value() == "NAME") {
						r.filename = info->GetText();
					} else if (info->Value() == "HASH") {
						r.hash = info->GetText();
					} else if (info->Value() == "TRACKER") {
						r.tracker = info->GetText();
					}
					
					info = info->NextSiblingElement(false);
				}
				results.push_back(r);
				result = result->NextSiblingElement(false);
			}
			return results;
		}
	}
	
	virtual void SetUp(){
		
		easyHandle = curl_easy_init();
		curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, &getResponse);
		curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, &response);
		
		SearchEngine::clearSearchResults();
		DownloadManager::clearList();
	}
	
	virtual void TearDown() {
		
		curl_easy_cleanup(easyHandle);
	}
};

/* Alive */

/**
 * Test whether the server is running
 */
TEST_F(HTTPServerTest, aliveTest) {
	
	std::string addr = Settings::getIP() + ":1337/alive";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Alive", response);
}

/* Search */

/**
 * Test whether a search returns results
 */
TEST_F(HTTPServerTest, searchTrivial) {
	
	SearchEngine::clearSearchResults();
	
	std::string addr1 = Settings::getIP() + ":1337/search:test";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Search request sent.", response);
	response = "";
	
	std::string addr2 = Settings::getIP() + ":1337/results";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	std::vector<struct SearchEngine::result> results = toVector(response);
	EXPECT_LT(0, results.size());
}

/**
 * Test whether an empty searh is handled properly
 */
TEST_F(HTTPServerTest, searchEmpty) {
	
	SearchEngine::clearSearchResults();
	
	std::string addr = Settings::getIP() + ":1337/search:";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Bad Request", response);
	response = "";
	
	std::string addr2 = Settings::getIP() + ":1337/results";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	std::vector<struct SearchEngine::result> results = toVector(response);
	EXPECT_EQ(0, results.size());
}

/* Add and Download */

/**
 * Test whether you can add and download a file after a search
 */
TEST_F(HTTPServerTest, downloadTrivial) {
	
	search();
	
	EXPECT_EQ("Search request sent.", response);
	response = "";
	
	std::string addr2 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download Added", response);
	response = "";
	
	// Need to add a second download in order to use /download as the first download
	// starts automatically
	std::string addr3 = Settings::getIP() + ":1337/add:012b5549e2622ea8bf3d694b4f55c959539ac848";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr3.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download Added", response);
	response = "";
	
	std::string addr4 = Settings::getIP() + ":1337/download:012b5549e2622ea8bf3d694b4f55c959539ac848";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr4.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download Started", response);
}

/**
 * Test whether adding a nonexistent download is handled properly
 */
TEST_F(HTTPServerTest, addNonexistent){
	
	std::string addr1 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Could not find the file.", response);
}

/**
 * Test whether downloading a nonexistent download is handled properly
 */
TEST_F(HTTPServerTest, downloadNonexistent){
	
	std::string addr1 = Settings::getIP() + ":1337/download:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Could not find the file.", response);
}

/**
 * Try downloading the same file twice
 */
TEST_F(HTTPServerTest, downloadTwice){
	
	search();
	response = "";
	
	std::string addr2 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download Added", response);
	response = "";
	
	std::string addr3 = Settings::getIP() + ":1337/add:012b5549e2622ea8bf3d694b4f55c959539ac848";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr3.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download Added", response);
	response = "";
	
	std::string addr4 = Settings::getIP() + ":1337/download:012b5549e2622ea8bf3d694b4f55c959539ac848";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr4.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download Started", response);
	response = "";
	
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr4.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("This file is already being downloaded.", response);
	response = "";
}

/**
 * Test the infamous pause-resume-pause segfault.
 */
TEST_F(HTTPServerTest, pauseResumePause) {
	
	search();
	
	std::string addr1 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	std::string addr2 = Settings::getIP() + ":1337/pause:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	std::string addr3 = Settings::getIP() + ":1337/resume:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr3.c_str());
	res = curl_easy_perform(easyHandle);
	
	response = "";
	
	std::string addr4 = Settings::getIP() + ":1337/pause:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr4.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download Paused", response);
	response = "";
}

/**
 * Test the infamous pause-resume-stats segfault.
 */
TEST_F(HTTPServerTest, pauseResumeStats) {
	
	search();
	
	std::string addr1 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	
	std::string addr2 = Settings::getIP() + ":1337/pause:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	std::string addr3 = Settings::getIP() + ":1337/resume:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr3.c_str());
	res = curl_easy_perform(easyHandle);
	
	std::string addr4 = Settings::getIP() + ":1337/stats";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr4.c_str());
	res = curl_easy_perform(easyHandle);
	
	// We don't need to check for any specific outcome,
	// just making sure there is no segfault.
}

/* Remove */

/**
 * Trivial test for remove
 */
TEST_F(HTTPServerTest, removeTrivial){
	
	search();
	
	std::string addr1 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	response = "";
	std::string addr2 = Settings::getIP() + ":1337/add:012b5549e2622ea8bf3d694b4f55c959539ac848";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ(2, DownloadManager::getDownloads().size());
	EXPECT_EQ("Download Added", response);
	response = "";
	
	std::string addr3 = Settings::getIP() + ":1337/remove:012b5549e2622ea8bf3d694b4f55c959539ac848";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr3.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
	EXPECT_EQ("Download removed from disk", response);
}

/**
 * Remove the active download
 */
TEST_F(HTTPServerTest, removeActiveDownload) {
	
	search();
	
	std::string addr1 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	response = "";
	std::string addr2 = Settings::getIP() + ":1337/add:012b5549e2622ea8bf3d694b4f55c959539ac848";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("367d26a6ce626e049a21921100e24eac86dbcd32", DownloadManager::getActiveDownload().getRootHash());
	EXPECT_EQ(2, DownloadManager::getDownloads().size());
	response = "";
	
	// After removing the active download, the second download should become active
	std::string addr3 = Settings::getIP() + ":1337/remove:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr3.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Download removed from disk", response);
	EXPECT_EQ("012b5549e2622ea8bf3d694b4f55c959539ac848", DownloadManager::getActiveDownload().getRootHash());
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
}

/**
 * Try Removing a nonexistent download
 */
TEST_F(HTTPServerTest, removeNonexistent) {
	
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
	
	std::string addr1 = Settings::getIP() + ":1337/remove:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Could not find the file.", response);
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
}

/**
 * Start a stream
 */
TEST_F(HTTPServerTest, startStreamTrivial) {
	
	search();
	response = "";
	
	std::string hash = "367d26a6ce626e049a21921100e24eac86dbcd32";
	std::string addr1 = Settings::getIP() + ":1337/stream:" + hash;
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("http://" + Settings::getIP() + ":17758/" + hash, response);
}

/**
 * Stop a stream
 */
TEST_F(HTTPServerTest, stopStreamTrivial) {
	
	search();
	
	std::string hash = "367d26a6ce626e049a21921100e24eac86dbcd32";
	std::string addr1 = Settings::getIP() + ":1337/stream:" + hash;
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	response = "";
	std::string addr2 = Settings::getIP() + ":1337/stopStream";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr2.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Not streaming anymore.", response);
}
