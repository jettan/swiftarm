#include "DownloadManager.h"
#include "Download.h"
#include "gtest.h"
#include <string>

class DownloadManagerTest : public ::testing::Test {
	protected:
	
	virtual ~DownloadManagerTest() {}
	
	virtual void SetUp(){
	}
	
	virtual void TearDown() {}
};

Download* createDownload(std::string tracker, std::string hash, std::string filename, int ID){
	
	Download *download = new Download(tracker, hash, filename);
	
	return download;
}

void testDownloadsAreEqual(Download dl1, Download dl2){
	
	EXPECT_EQ(dl1.getID(), dl2.getID());
	EXPECT_EQ(dl1.getTrackerAddress(), dl2.getTrackerAddress());
	EXPECT_EQ(dl1.getRootHash(), dl2.getRootHash());
	EXPECT_EQ(dl1.getFilename(), dl2.getFilename());
}

/* Download Directory */

// Trivial
TEST_F(DownloadManagerTest, setDirectoryTrivial) {
	
	std::string test = "/dtv/usb/sda1/Test";
	DownloadManager::setDownloadDirectory(test);
	
	EXPECT_EQ(test, DownloadManager::getDownloadDirectory());
	
	DownloadManager::setDownloadDirectory("/dtv/usb/sda1/Downloads");
	
}

/* Add Download */

// Trivial
TEST_F(DownloadManagerTest, addDownloadTrivial) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	std::cout << " Going to create Downoad..." << std::endl;
	Download *testDL = new Download("tracker", hash, "test");
	std::cout << " Created Download." << std::endl;
	DownloadManager::add(testDL);
	std::cout << " Added download" << std::endl;
	
	int index = DownloadManager::getIndexFromHash(hash);
	Download returnedDL = DownloadManager::getDownloads().at(index);
	testDL->setID(returnedDL.getID());
	
	testDownloadsAreEqual(*testDL, returnedDL);
	
}

/* Get Active Download */

//Trivial
TEST_F(DownloadManagerTest, getActiveDownloadTrivial) {
	
	//DownloadManager::clearList();
	Download testDL("track", "1234abcd1234abcd1234abcd1234abcd1234abcd", "name");
	testDL.setID(6);
	DownloadManager::add(&testDL);
	
	Download returnedDL = DownloadManager::getActiveDownload();
	
	testDownloadsAreEqual(testDL, returnedDL);
	
}

/* Start Download */

// Trivial
TEST_F(DownloadManagerTest, startDownloadTrivial){
	
	
}

/* Remove From List */

// Trivial
TEST_F(DownloadManagerTest, removeFromListTrivial){
	
	
}

