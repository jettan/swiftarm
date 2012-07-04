#include "DownloadManager.h"
#include "Download.h"
#include "Stream.h"
#include "Settings.h"
#include "gtest.h"
#include <string>

/**
 * This is the class for testing DownloadManager.cpp
 * The setup clear the download list
 * The teardown also clears the download list and removes all downloaded files
 */
class DownloadManagerTest : public ::testing::Test {
	protected:
	
	virtual ~DownloadManagerTest() {}
	
	virtual void SetUp(){
		DownloadManager::clearList();
	}
	
	virtual void TearDown() {
		DownloadManager::clearList();
		
		DIR *dp;
		struct dirent *dirp;
		
		if((dp = opendir(Settings::getDownloadDirectory().c_str())) == NULL) {
			std::cout << "Failed opening Downloads directory in DownloadManagerTest" << std::endl;
			
		} else {
			while ((dirp = readdir(dp)) != NULL) {
				std::string filename(dirp->d_name);
				if (filename.at(0) != '.') {
					remove(filename.c_str());
				}
			}
		}
		
		if(Stream::getInstance()->readStreaming()) {
			Stream::getInstance()->stop();
		}
		
	}
};


/**
 * Check whether two downloads are the same
 */
void testDownloadsAreEqual(Download dl1, Download dl2){
	
	EXPECT_EQ(dl1.getID(), dl2.getID());
	EXPECT_EQ(dl1.getTrackerAddress(), dl2.getTrackerAddress());
	EXPECT_EQ(dl1.getRootHash(), dl2.getRootHash());
	EXPECT_EQ(dl1.getFilename(), dl2.getFilename());
}

/* Download Directory */

/**
 * Trivial test for setDirectory
 */
TEST_F(DownloadManagerTest, setDirectoryTrivial) {
	
	std::string test = "/dtv/usb/sda1/Test";
	Settings::setDownloadDirectory(test);
	
	EXPECT_EQ(test, Settings::getDownloadDirectory());
	
	Settings::setDownloadDirectory("/dtv/usb/sda1/Downloads");
}

/* getIndexFromHash */

/**
 * Trivial test for getIndexFromHash
 */
TEST_F(DownloadManagerTest, getIndexFromHashTrivial) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	
	Download *testDL = new Download("tracker", hash, "test");
	
	DownloadManager::add(testDL);
	
	EXPECT_EQ(0, DownloadManager::getIndexFromHash(hash));
}

/**
 * Try getting a download with a hash that doens't exist
 */
TEST_F(DownloadManagerTest, getIndexFromHashNonexistent) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	EXPECT_THROW(DownloadManager::getIndexFromHash(hash), FileNotFoundException);
}

/* Add Download */

/**
 * Trivial test for addDownload
 */
TEST_F(DownloadManagerTest, addDownloadTrivial) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";

	Download *testDL = new Download("tracker", hash, "test");
	DownloadManager::add(testDL);
	
	int index = DownloadManager::getIndexFromHash(hash);
	Download returnedDL = DownloadManager::getDownloads().at(index);
	testDL->setID(returnedDL.getID());
	
	testDownloadsAreEqual(*testDL, returnedDL);
}

/**
 * Try downloading something that is already being download
 */
TEST_F(DownloadManagerTest, addDownloadExists) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	
	Download *dl1 = new Download("tracker", hash, "test");
	DownloadManager::add(dl1);
	
	Download *dl2 = new Download("tracker", hash, "test");
	
	EXPECT_THROW(DownloadManager::add(dl2), AlreadyDownloadingException);
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
	
}

/* Download First In List */

/**
 * Trivial test for downloadFirstInList
 */
TEST_F(DownloadManagerTest, downloadFirstInListTrivial) {
	
	// Add two different downloads
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	std::string hash2 = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download dl2("track2", hash2, "name2");
	DownloadManager::add(&dl2);
	
	// Check whether the first download started downloading automatically
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	
	// Start the second download
	DownloadManager::startDownload(hash2);
	index = DownloadManager::getIndexFromHash(hash2);
	dl2.setID(DownloadManager::getDownloads().at(index).getID());
	
	// Check whether the second download is not downloading
	testDownloadsAreEqual(dl2, DownloadManager::getActiveDownload());
	
	// Start downloading the first again
	DownloadManager::downloadFirstInList();
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
}

/**
 * Try downloading the first download when already downloading the first
 */
TEST_F(DownloadManagerTest, downloadFirstInListAlreadyDownloading) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	
	DownloadManager::downloadFirstInList();
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
}

/* Clear List */

/**
 * Trivial test for clearList
 */
TEST_F(DownloadManagerTest, clearListTrivial) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download *testDL = new Download("tracker", hash, "test");
	DownloadManager::add(testDL);
	
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
	
	DownloadManager::clearList();
	
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
}

/**
 * Try clearing list when it is already empty
 */
TEST_F(DownloadManagerTest, clearListEmpty) {
	
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
	
	DownloadManager::clearList();
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
}

/* Get Active Download */

/**
 * Trivial test for getActiveDownload
 */
TEST_F(DownloadManagerTest, getActiveDownloadTrivial) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download testDL("track", hash1, "name");
	DownloadManager::add(&testDL);
	int index = DownloadManager::getIndexFromHash(hash1);
	testDL.setID(DownloadManager::getDownloads().at(index).getID());
	
	Download returnedDL = DownloadManager::getActiveDownload();
	
	testDownloadsAreEqual(testDL, returnedDL);
}

/* Start Download */

/**
 * Trivial test for startDownload
 */
TEST_F(DownloadManagerTest, startDownloadTrivial) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	std::string hash2 = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download dl2("track2", hash2, "name2");
	DownloadManager::add(&dl2);
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	
	DownloadManager::startDownload(hash2);
	index = DownloadManager::getIndexFromHash(hash2);
	dl2.setID(DownloadManager::getDownloads().at(index).getID());
	
	testDownloadsAreEqual(dl2, DownloadManager::getActiveDownload());
}

/**
 * Try downloading a file that doens't exist
 */
TEST_F(DownloadManagerTest, startDownloadNonexistent) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	ASSERT_THROW(DownloadManager::startDownload(hash), FileNotFoundException);
	
}

/* Remove From List */

/**
 * Trivial test for removeFromList
 */
TEST_F(DownloadManagerTest, removeFromListTrivial) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	std::string hash2 = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download dl2("track2", hash2, "name2");
	DownloadManager::add(&dl2);
	
	EXPECT_EQ(2, DownloadManager::getDownloads().size());
	
	DownloadManager::removeFromList(hash2);
	
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
	EXPECT_THROW(DownloadManager::getIndexFromHash(hash2), FileNotFoundException);
}

/**
 * Remove the active download
 */
TEST_F(DownloadManagerTest, removeFromListActiveDownload) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	std::string hash2 = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download dl2("track2", hash2, "name2");
	DownloadManager::add(&dl2);
	
	EXPECT_EQ(2, DownloadManager::getDownloads().size());
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	
	DownloadManager::removeFromList(hash1);
	index = DownloadManager::getIndexFromHash(hash2);
	dl2.setID(DownloadManager::getDownloads().at(index).getID());
	
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
	EXPECT_THROW(DownloadManager::getIndexFromHash(hash1), FileNotFoundException);
	testDownloadsAreEqual(dl2, DownloadManager::getActiveDownload());
}

/**
 * try removing nonexistent file
 */
TEST_F(DownloadManagerTest, removeFromListNonexistent) {
	
	std::string hash = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	EXPECT_THROW(DownloadManager::removeFromList(hash), FileNotFoundException);
}

/* Switch Active Download */

/**
 * Tivial test for switchDownload
 */
TEST_F(DownloadManagerTest, switchDownloadTrivial) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	std::string hash2 = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download dl2("track2", hash2, "name2");
	DownloadManager::add(&dl2);
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	
	DownloadManager::switchDownload(hash2);
	index = DownloadManager::getIndexFromHash(hash2);
	dl2.setID(DownloadManager::getDownloads().at(index).getID());
	
	EXPECT_EQ(2, DownloadManager::getDownloads().size());
	testDownloadsAreEqual(dl2, DownloadManager::getActiveDownload());
}

/**
 * Try switching to the same download
 */
TEST_F(DownloadManagerTest, switchDownloadSame) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	Download dl2("track2", "abcd1234abcd1234abcd1234abcd1234abcd1234", "name2");
	dl2.setID(7);
	DownloadManager::add(&dl2);
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	EXPECT_EQ(DOWNLOADING, DownloadManager::getActiveDownload().getStatus());
	
	EXPECT_THROW(DownloadManager::switchDownload(hash1), AlreadyDownloadingException);
	
	EXPECT_EQ(2, DownloadManager::getDownloads().size());
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	EXPECT_EQ(DOWNLOADING, DownloadManager::getActiveDownload().getStatus());
}

/**
 * Switch to nonexistent download.
 */
TEST_F(DownloadManagerTest, switchDownloadNonexistent) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	std::string hash2 = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	EXPECT_THROW(DownloadManager::switchDownload(hash2), FileNotFoundException);
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	EXPECT_EQ(DOWNLOADING, DownloadManager::getActiveDownload().getStatus());
}

/**
 * Trivial test for starting a stream
 */
TEST_F(DownloadManagerTest, startStreamTrivial) {
	
	DownloadManager::startStream("127.0.0.1:9999");
	sleep(1);
	EXPECT_EQ(true, Stream::getInstance()->readStreaming());
}

/**
 * Start a stream while there is an active download
 */
TEST_F(DownloadManagerTest, startStreamWithActiveDownload) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	EXPECT_EQ(DOWNLOADING, DownloadManager::getActiveDownload().getStatus());
	
	DownloadManager::startStream("127.0.0.1:9999");
	sleep(1);
	
	EXPECT_EQ(true, Stream::getInstance()->readStreaming());
	EXPECT_EQ(PAUSED, DownloadManager::getActiveDownload().getStatus());
}

/**
 * Trivial test for stopping a stream
 */
TEST_F(DownloadManagerTest, stopStreamTrivial) {
	
	DownloadManager::startStream("127.0.0.1:9999");
	sleep(1);
	EXPECT_EQ(true, Stream::getInstance()->readStreaming());
	
	DownloadManager::stopStream();
	EXPECT_EQ(false, Stream::getInstance()->readStreaming());
}

/**
 * Stop a stream while there was an active download
 */
TEST_F(DownloadManagerTest, stopStreamWithActiveDownload) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	testDownloadsAreEqual(dl1, DownloadManager::getActiveDownload());
	EXPECT_EQ(DOWNLOADING, DownloadManager::getActiveDownload().getStatus());
	
	DownloadManager::startStream("127.0.0.1:9999");
	sleep(1);
	EXPECT_EQ(true, Stream::getInstance()->readStreaming());
	EXPECT_EQ(PAUSED, DownloadManager::getActiveDownload().getStatus());
	
	DownloadManager::stopStream();
	EXPECT_EQ(false, Stream::getInstance()->readStreaming());
	EXPECT_EQ(DOWNLOADING, DownloadManager::getActiveDownload().getStatus());
}
