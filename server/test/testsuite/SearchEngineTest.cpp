#include "SearchEngine.h"
#include "gtest.h"
#include <string>

class SearchEngineTest : public ::testing::Test {
	protected:
	
	virtual ~SearchEngineTest() {}
	
	virtual void SetUp(){
		SearchEngine::init();
	}
	
	virtual void TearDown() {}
};

TEST_F(SearchEngineTest, searchTrivial) {
	
	std::string res = SearchEngine::search("test");
	
	std::cout << res << std::endl;
}
