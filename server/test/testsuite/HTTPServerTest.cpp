#include <string>

#include "curl.h"
#include "easy.h"
#include "HttpServer.h"
#include "gtest.h"

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
		
	}
	
	virtual void TearDown() {
		
		curl_easy_cleanup(easyHandle);
	}
};

TEST_F(HTTPServerTest, aliveTest) {
	
	curl_easy_setopt(easyHandle, CURLOPT_URL, "http://127.0.0.1:1337/alive");
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ("Alive", response);
}

TEST_F(HTTPServerTest, searchTrivial) {
	
	EXPECT_EQ(0, SearchEngine::getResults().size());
	
	curl_easy_setopt(easyHandle, CURLOPT_URL, "http://127.0.0.1:1337/search:test");
	res = curl_easy_perform(easyHandle);
	
	EXPECT_LT(0, SearchEngine::getResults().size());
}

TEST_F(HTTPServerTest, searchEmpty) {
	
	EXPECT_EQ(0, SearchEngine::getResults().size());
	
	curl_easy_setopt(easyHandle, CURLOPT_URL, "http://127.0.0.1:1337/search:");
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ(0, SearchEngine::getResults().size());
	EXPECT_EQ("Invalid Search Term", response);
}

TEST_F(HTTPServerTest, downloadTrivial) {
	
	curl_easy_setopt(easyHandle, CURLOPT_URL, "http://127.0.0.1:1337/search:test");
	res = curl_easy_perform(easyHandle);
	
	curl_easy_setopt(easyHandle, CURLOPT_URL, "http://127.0.0.1:1337/download:367d26a6ce626e049a21921100e24eac86dbcd32");
	res = curl_easy_perform(easyHandle);
	
	EXPECT_EQ(response, "Download Started");
}
