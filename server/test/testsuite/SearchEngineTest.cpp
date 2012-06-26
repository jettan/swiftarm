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
	
	struct SearchEngine::result r = SearchEngine::getResultWithName("Test1.test");
	EXPECT_EQ("367d26a6ce626e049a21921100e24eac86dbcd32", r.hash);
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
	
	struct SearchEngine::result r = SearchEngine::getResultWithHash("367d26a6ce626e049a21921100e24eac86dbcd32");
	EXPECT_EQ("Test1.test", r.filename);
}

// Doesn't exist
TEST_F(SearchEngineTest, getResultWithHashNonexistent) {
	
	SearchEngine::search("test");
	
	ASSERT_THROW(SearchEngine::getResultWithName("1234554321123455432112345543211234554321"), FileNotFoundException);
}


