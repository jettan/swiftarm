#include "SearchEngine.h"
#include "gtest.h"
#include "Exceptions.h"
#include <string>

class SearchEngineTest : public ::testing::Test {
	protected:
	
	virtual ~SearchEngineTest() {}
	
	virtual void SetUp(){}
	
	virtual void TearDown() {}
};

std::vector<struct SearchEngine::result> toVector(std::string result) {
	
	
}


/* Search Tests */

// Trivial
TEST_F(SearchEngineTest, searchTrivial) {
	
	SearchEngine::clearSearchResults();
	SearchEngine::search("test");
	
	EXPECT_LT(0, SearchEngine::getResults().size());
}

/* Get Results Test */

// Trivial
TEST_F(SearchEngineTest, getResultsTrivial) {
	
	SearchEngine::clearSearchResults();
	EXPECT_EQ(0, SearchEngine::getResults().size());
	
	SearchEngine::search("test");
	EXPECT_LT(0, SearchEngine::getResults().size());
}

// Empty
TEST_F(SearchEngineTest, getResultsEmpty) {
	
	SearchEngine::clearSearchResults();
	EXPECT_EQ(0, SearchEngine::getResults().size());
}

/* Get Result With Name Tests */

// Trivial
TEST_F(SearchEngineTest, getResultWithNameTrivial) {
	
	SearchEngine::search("test");
	
	std::string name = SearchEngine::getResults().at(0).filename;
	std::string hash = SearchEngine::getResults().at(0).hash;
	
	EXPECT_EQ(hash, SearchEngine::getResultWithName(name).hash);
}

// Doesn't exist
TEST_F(SearchEngineTest, getResultWithNameNonexistent) {
	
	SearchEngine::search("test");
	
	ASSERT_THROW(SearchEngine::getResultWithName("re.tard"), FileNotFoundException);
}

/* Get Result With Hash Tests */

// Trivial
TEST_F(SearchEngineTest, getResultWithHashTrivial) {
	
	SearchEngine::search("test");
	std::string result = SearchEngine::getResults();
	
	ticpp::Document doc;
	doc.Parse(result, true, TIXML_DEFAULT_ENCODING);
	
	
	
	std::cout
}

// Doesn't exist
TEST_F(SearchEngineTest, getResultWithHashNonexistent) {
	
	SearchEngine::search("test");
	
	ASSERT_THROW(SearchEngine::getResultWithName("LOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLO"), FileNotFoundException);
}


