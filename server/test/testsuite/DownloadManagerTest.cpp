#include "DownloadManager.h"
#include "Download.h"
#include "gtest.h"
#include <string>

class DownloadManagerTest : public ::testing::Test {
	protected:
	
	virtual ~DownloadManagerTest() {}
	
	virtual void SetUp(){
		
		DownloadManager::setDownloadDirectory("");
		DownloadManager::clearList();
		DownloadManager::stopStream();
	}
	
	virtual void TearDown() {}
};

Download* createDownload(std::string tracker, std::string hash, std::string filename, int ID){
	
	Download *download = new Download(tracker, hash, filename);
	
	return download;
}

void testDownloadsAreEqual(Download *dl1, Download *dl2){
	
	EXPECT_EQ(dl1->getID(), dl2->getID());
	EXPECT_EQ(dl1->getTrackerAddress(), dl2->getTrackerAddress());
	EXPECT_EQ(dl1->getRootHash(), dl2->getRootHash());
	EXPECT_EQ(dl1->getFilename(), dl2->getFilename());
}

/* Download Directory */

// Trivial
TEST_F(DownloadManagerTest, setDirectoryTrivial) {
	std::string test = "/blah/easy/test";
	DownloadManager::setDownloadDirectory(test);
	
	EXPECT_EQ(test, DownloadManager::getDownloadDirectory());
}

/* Add Download */

// Trivial
TEST_F(DownloadManagerTest, addDownloadTrivial) {
	/*
	int ID = 5;
	Download *testDL = createDownload("tracker", "abcd1234abcd1234abcd1234abcd1234abcd1234", "test", ID);
	DownloadManager::add(*testDL);
	
	Download *returnedDL = DownloadManager::getDownloadWithID(ID);
	
	testDownloadsAreEqual(testDL, returnedDL);
	*/
}


/* Get Acitve Download */

//Trivial
TEST_F(DownloadManagerTest, getActiveDownloadTrivial) {
	/*
	int ID = 6;
	Download *testDL = createDownload("track", "1234abcd1234abcd1234abcd1234abcd1234abcd", "name", ID);
	DownloadManager::add(*testDL);
	
	Download *returnedDL = DownloadManager::getActiveDownload();
	
	testDownloadsAreEqual(testDL, returnedDL);
	*/
}

/* Start Download */

// Trivial
TEST_F(DownloadManagerTest, startDownloadTrivial){
	
	
}

/* Remove From List */

// Trivial
TEST_F(DownloadManagerTest, removeFromListTrivial){
	
	
}

