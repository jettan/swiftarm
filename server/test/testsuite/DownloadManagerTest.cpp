#include "DownloadManager.h"
#include "Download.h"
#include "Utils.h"
#include "gtest.h"
#include <string>

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
			std::cout << "TEST: Removing all files in " << Settings::getDownloadDirectory() << std::endl;
			while ((dirp = readdir(dp)) != NULL) {
				std::string filename(dirp->d_name);
				if (filename.at(0) != '.') {
					remove(filename.c_str());
				}
			}
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

// Trivial
TEST_F(DownloadManagerTest, setDirectoryTrivial) {
	
	std::string test = "/dtv/usb/sda1/Test";
	Settings::setDownloadDirectory(test);
	
	EXPECT_EQ(test, Settings::getDownloadDirectory());
	
	Settings::setDownloadDirectory("/dtv/usb/sda1/Downloads");
}

/* getIndexFromHash */

// Trivial
TEST_F(DownloadManagerTest, getIndexFromHashTrivial) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	
	Download *testDL = new Download("tracker", hash, "test");
	
	DownloadManager::add(testDL);
	
	EXPECT_EQ(0, DownloadManager::getIndexFromHash(hash));
}

// Nonexistent
TEST_F(DownloadManagerTest, getIndexFromHashNonexistent) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	EXPECT_THROW(DownloadManager::getIndexFromHash(hash), FileNotFoundException);
}

/* Add Download */

// Trivial
TEST_F(DownloadManagerTest, addDownloadTrivial) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";

	Download *testDL = new Download("tracker", hash, "test");
	DownloadManager::add(testDL);
	
	int index = DownloadManager::getIndexFromHash(hash);
	Download returnedDL = DownloadManager::getDownloads().at(index);
	testDL->setID(returnedDL.getID());
	
	testDownloadsAreEqual(*testDL, returnedDL);
}

// Already Exists
TEST_F(DownloadManagerTest, addDownloadExists) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	
	Download *dl1 = new Download("tracker", hash, "test");
	DownloadManager::add(dl1);
	
	Download *dl2 = new Download("tracker", hash, "test");
	
	EXPECT_THROW(DownloadManager::add(dl2), AlreadyDownloadingException);
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
	
}

/* Download First In List */

// Trivial
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
	
	std::cout << "Added downloads" << std::endl;
	
	// Check whether the first download started downloading automatically
	testDownloadsAreEqual(DownloadManager::getActiveDownload(), dl1);
	
	// Start the second download
	DownloadManager::startDownload(hash2);
	index = DownloadManager::getIndexFromHash(hash2);
	dl2.setID(DownloadManager::getDownloads().at(index).getID());
	
	// Check whether the second download is not downloading
	testDownloadsAreEqual(DownloadManager::getActiveDownload(), dl2);
	
	// Start downloading the first again
	DownloadManager::downloadFirstInList();
	
	testDownloadsAreEqual(DownloadManager::getActiveDownload(), dl1);
}

// Already Downloading the first
TEST_F(DownloadManagerTest, downloadFirstInListAlreadyDownloading) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	testDownloadsAreEqual(DownloadManager::getActiveDownload(), dl1);
	
	DownloadManager::downloadFirstInList();
	
	testDownloadsAreEqual(DownloadManager::getActiveDownload(), dl1);
}

/* Clear List */

// Trivial
TEST_F(DownloadManagerTest, clearListTrivial) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download *testDL = new Download("tracker", hash, "test");
	DownloadManager::add(testDL);
	
	EXPECT_EQ(1, DownloadManager::getDownloads().size());
	
	DownloadManager::clearList();
	
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
}

// Already Empty
TEST_F(DownloadManagerTest, clearListEmpty) {
	
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
	
	DownloadManager::clearList();
	EXPECT_EQ(0, DownloadManager::getDownloads().size());
}

/* Get Active Download */

//Trivial
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

// Trivial
TEST_F(DownloadManagerTest, startDownloadTrivial) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", hash1, "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash(hash1);
	dl1.setID(DownloadManager::getDownloads().at(index).getID());
	
	std::string hash2 = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	Download dl2("track2", hash2, "name2");
	DownloadManager::add(&dl2);
	
	testDownloadsAreEqual(DownloadManager::getActiveDownload(), dl1);
	
	DownloadManager::startDownload(hash2);
	index = DownloadManager::getIndexFromHash(hash2);
	dl2.setID(DownloadManager::getDownloads().at(index).getID());
	
	testDownloadsAreEqual(DownloadManager::getActiveDownload(), dl2);
}

// Nonexistent
TEST_F(DownloadManagerTest, startDownloadNonexistent) {
	
	std::string hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
	ASSERT_THROW(DownloadManager::startDownload(hash), FileNotFoundException);
	
}

/* Remove From List */

// Trivial
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

// Remove Active Download
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

// Remove Nonexistent File
TEST_F(DownloadManagerTest, removeFromListNonexistent) {
	
	std::string hash = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	EXPECT_THROW(DownloadManager::removeFromList(hash), FileNotFoundException);
}

/* Switch Active Download */

// Tivial
TEST_F(DownloadManagerTest, switchDownloadTrivial) {
	
	std::string hash1 = "1234abcd1234abcd1234abcd1234abcd1234abcd";
	Download dl1("track1", "1234abcd1234abcd1234abcd1234abcd1234abcd", "name1");
	DownloadManager::add(&dl1);
	int index = DownloadManager::getIndexFromHash("1234abcd1234abcd1234abcd1234abcd1234abcd");
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

// Switch to same download
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

// Switch to nonexistent download
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
