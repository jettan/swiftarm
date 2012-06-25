#include <string>

#include "curl.h"
#include "easy.h"
#include "HttpServer.h"
#include "gtest.h"
#include "SearchEngine.h"

class HTTPServerTest : public ::testing::Test {
	protected:
	
	CURL *easyHandle;
	CURLcode res;
	std::string response;
	
	virtual ~HTTPServerTest() {}
	
	size_t static getResponse(char *response, size_t size, size_t count, void *stream) {
		
		((std::string*)stream)->append((char*)response, 0, size * count);
		return size*count;
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

// Test whether server is running
TEST_F(HTTPServerTest, aliveTest) {
	
	std::string addr = Settings::getIP() + ":1337/alive";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Alive", response);
}

/* Search */

// Test whether a search returns results
TEST_F(HTTPServerTest, searchTrivial) {
	
	SearchEngine::clearSearchResults();
	
	std::string addr = Settings::getIP() + ":1337/search:test";
	std::cout << "Got IP" << std::endl;
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr.c_str());
	res = curl_easy_perform(easyHandle);
	std::cout << "Sent Http request" << std::endl;
	
	EXPECT_EQ("Search request sent.", response);
}

// Test whether an empty searh is handled properly
TEST_F(HTTPServerTest, searchEmpty) {
	
	SearchEngine::clearSearchResults();
	
	std::string addr = Settings::getIP() + ":1337/search:";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr.c_str());
	res = curl_easy_perform(easyHandle);
	
	// PARSE EMPTY XML HERE
	EXPECT_EQ("Bad Request", response);
}

/* Add and Download */

// Test whether you can add and download a file after a search
TEST_F(HTTPServerTest, downloadTrivial) {
	
	std::string addr = Settings::getIP() + ":1337/search:test";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr.c_str());
	res = curl_easy_perform(easyHandle);
	
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

// Test whether adding a nonexistent download is handled properly
TEST_F(HTTPServerTest, addNonexistent){
	
	std::string addr1 = Settings::getIP() + ":1337/add:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Could not find the file.", response);
}

// Test whether downloading a nonexistent download is handled properly
TEST_F(HTTPServerTest, downloadNonexistent){
	
	std::string addr1 = Settings::getIP() + ":1337/download:367d26a6ce626e049a21921100e24eac86dbcd32";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Could not find the file.", response);
}

// Try downloading the same file twice
TEST_F(HTTPServerTest, downloadTwice){
	
	std::string addr1 = Settings::getIP() + ":1337/search:test";
	curl_easy_setopt(easyHandle, CURLOPT_URL, addr1.c_str());
	res = curl_easy_perform(easyHandle);
	
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

/* Remove */

TEST_F(HTTPServerTest, removeTrivial){
	
	
}
