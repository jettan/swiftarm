#include "SearchEngine.h"
#include "gtest.h"
#include "Exceptions.h"
#include <string>

/**
 * This class tests the SearchEngine.
 * It doesn not test the search function because it is mocked
 * The setup clears the list of search results
 * The tear down is empty
 */
class SearchEngineTest : public ::testing::Test {
	protected:
	
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
	
	virtual ~SearchEngineTest() {}
	
	virtual void SetUp() {
		
		SearchEngine::clearSearchResults();
	}
	
	virtual void TearDown() {}
};

/* Get Results Test */

/**
 * Trivial test for getResults
 */
TEST_F(SearchEngineTest, getResultsTrivial) {
	
	SearchEngine::search("test");
	std::vector<struct SearchEngine::result> results = toVector(SearchEngine::getResults());
	
	EXPECT_EQ(2, results.size());
}

/**
 * Get resuts when there are none
 */
TEST_F(SearchEngineTest, getResultsEmpty) {
	
	SearchEngine::search("test");
	SearchEngine::clearSearchResults();
	std::vector<struct SearchEngine::result> results = toVector(SearchEngine::getResults());
	
	EXPECT_EQ(0, results.size());
}

/* Get Result With Name Tests */

/**
 * Trivial test for getResultWithName
 */
TEST_F(SearchEngineTest, getResultWithNameTrivial) {
	
	SearchEngine::search("test");
	
	struct SearchEngine::result r = SearchEngine::getResultWithName("Test1.test");
	EXPECT_EQ("367d26a6ce626e049a21921100e24eac86dbcd32", r.hash);
}

/**
 * Try to get a result by name that doesn't exist
 */
TEST_F(SearchEngineTest, getResultWithNameNonexistent) {
	
	SearchEngine::search("test");
	
	ASSERT_THROW(SearchEngine::getResultWithName("re.tard"), FileNotFoundException);
}

/* Get Result With Hash Tests */

/**
 * Trivial test for getResultWithHash
 */
TEST_F(SearchEngineTest, getResultWithHashTrivial) {
	
	SearchEngine::search("test");
	std::string result = SearchEngine::getResults();
	
	struct SearchEngine::result r = SearchEngine::getResultWithHash("367d26a6ce626e049a21921100e24eac86dbcd32");
	EXPECT_EQ("Test1.test", r.filename);
}

/**
 * Try to get a result by name that doesn't exist
 */
TEST_F(SearchEngineTest, getResultWithHashNonexistent) {
	
	SearchEngine::search("test");
	
	ASSERT_THROW(SearchEngine::getResultWithName("1234554321123455432112345543211234554321"), FileNotFoundException);
}

