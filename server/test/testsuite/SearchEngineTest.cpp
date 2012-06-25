#include "SearchEngine.h"
#include "gtest.h"
#include "Exceptions.h"
#include <string>

class SearchEngineTest : public ::testing::Test {
	protected:
	
	virtual ~SearchEngineTest() {}
	
	virtual void SetUp() {}
	
	virtual void TearDown() {}
};

std::vector<struct SearchEngine::result> toVector(std::string result) {
	
	ticpp::Document doc;
	doc.Parse(result, true, TIXML_DEFAULT_ENCODING);
	ticpp::Iterator<ticpp::Element> iterator;
	iterator = iterator.begin(&doc);
	
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
	// PARSE XML HERE
	
	SearchEngine::search("test");
}

// Empty
TEST_F(SearchEngineTest, getResultsEmpty) {
	
	SearchEngine::clearSearchResults();
	// PARSE XML HERE
}

/* Get Result With Name Tests */

// Trivial
TEST_F(SearchEngineTest, getResultWithNameTrivial) {
	
	SearchEngine::search("test");
	// TODO
	
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
	
	// UNFINISHED
	
	
}

// Doesn't exist
TEST_F(SearchEngineTest, getResultWithHashNonexistent) {
	
	SearchEngine::search("test");
	
	ASSERT_THROW(SearchEngine::getResultWithName("LOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLOLO"), FileNotFoundException);
}


