#include "Download.h"
#include "gtest.h"
#include <string>
#include <iostream>

/**
 * This is the class for testing Download.cpp
 * The setup creates a new Download with a hard coded tracker, hash and filename
 * The teardown removes any files that get created
 */
class DownloadTest : public ::testing::Test {
	protected:
	
	Download *download;
	std::string tracker;
	std::string root_hash;
	std::string filename;
	int id;
	
	virtual ~DownloadTest() {}
	
	virtual void SetUp(){
		tracker = "tracker";
		root_hash = "abcd1234abcd1234abcd1234abcd1234abcd1234";
		filename = "test";
		download = new Download(tracker, root_hash, filename);
	}
	
	virtual void TearDown() {
			
		DIR *dp;
		struct dirent *dirp;
		
		if((dp = opendir(Settings::getDownloadDirectory().c_str())) == NULL) {
			std::cout << "Failed opening Downloads directory in DownloadTest" << std::endl;
			
		} else {
			while ((dirp = readdir(dp)) != NULL) {
				std::string filename(dirp->d_name);
				if (filename.at(0) != '.') {
					remove(filename.c_str());
				}
			}
		}
	}
};

/* Constructor */

/**
 * Test whether the constructor sets values properly
 */
TEST_F(DownloadTest, constructor) {
	
	EXPECT_EQ(tracker, download->getTrackerAddress());
	EXPECT_EQ(root_hash, download->getRootHash());
	EXPECT_EQ(filename, download->getFilename());
}

/* ID */

/**
 * Trivial test for setID
 */
TEST_F(DownloadTest, setIDTrivial) {
	
	id = 100;
	download->setID(id);
	EXPECT_EQ(id, download->getID());
}

/**
 * Try setting the ID to a negative number
 */
TEST_F(DownloadTest, setIDNegative) {
	
	int id = -5;
	download->setID(id);
	EXPECT_EQ(-1, download->getID());
}

/***********************************************
********* DOWNLOAD STATS TESTS ****************
***********************************************/

/* Download Speed */

/**
 * Trivial test for setDownloadSpeed
 */
TEST_F(DownloadTest, setDownloadSpeedTrivial) {
	
	download->setID(1);
	double downloadSpeed = 10.5;
	download->setDownloadSpeed(downloadSpeed);
	double speed = download->getStatistics().download_speed;
	EXPECT_DOUBLE_EQ(downloadSpeed, speed);
}

/**
 * Try setting the downloadspeed to a negative number
 */
TEST_F(DownloadTest, setDownloadSpeedNegative) {
	
	download->setID(1);
	double downloadSpeed = -1;
	double originalSpeed = download->getStatistics().download_speed;
	download->setDownloadSpeed(downloadSpeed);
	double speed = download->getStatistics().download_speed;
	EXPECT_DOUBLE_EQ(originalSpeed, speed);
}


/* Upload Speed */

/**
 * Trivial test for setUploadSpeed
 */
TEST_F(DownloadTest, setUploadSpeedTrivial) {
	
	download->setID(1);
	double uploadSpeed = 10.5;
	download->setUploadSpeed(uploadSpeed);
	double speed = download->getStatistics().upload_speed;
	EXPECT_DOUBLE_EQ(uploadSpeed, speed);
}

/**
 * Try setting upload speed to a negative number
 */
TEST_F(DownloadTest, setUploadSpeedNegative) {
	
	download->setID(1);
	double uploadSpeed = -1;
	double originalSpeed = download->getStatistics().upload_speed;
	download->setUploadSpeed(uploadSpeed);
	double speed = download->getStatistics().upload_speed;
	EXPECT_DOUBLE_EQ(originalSpeed, speed);
}

/* Percentage */

/**
 * Trivial test for setPercentage
 */
TEST_F(DownloadTest, percentageTrivial) {
	
	download->setID(1);
	double percentage = 45.6;
	download->setProgress(percentage);
	
	EXPECT_DOUBLE_EQ(percentage, download->getStatistics().download_percentage);
}

/**
 * Try setting the percentage to a negative number
 */
TEST_F(DownloadTest, percentageNegative) {
	
	download->setID(1);
	double originalPercentage = download->getStatistics().download_percentage;
	double percentage = -45.6;
	download->setProgress(percentage);
	
	EXPECT_DOUBLE_EQ(originalPercentage, download->getStatistics().download_percentage);
}

/**
 * Try setting the percentage to a number above 100
 */
TEST_F(DownloadTest, percentageOver100) {
	
	download->setID(1);
	double originalPercentage = download->getStatistics().download_percentage;
	double percentage = 154.3;
	download->setProgress(percentage);
	
	EXPECT_DOUBLE_EQ(originalPercentage, download->getStatistics().download_percentage);
}

/* Seeders */

/**
 * Trivial test for setSeeders
 */
TEST_F(DownloadTest, seedersTrivial){
	
	download->setID(1);
	int seeders = 5;
	download->setSeeders(seeders);
	
	EXPECT_EQ(seeders, download->getStatistics().seeders);
}

/**
 * Try to set the seeders to a negative
 */
TEST_F(DownloadTest, seedersNegative){
	
	download->setID(1);
	int originalSeeders = 5;
	download->setSeeders(originalSeeders);
	int seeders = -1;
	download->setSeeders(seeders);
	
	EXPECT_EQ(originalSeeders, download->getStatistics().seeders);
}

/* Peers */

/**
 * Trivial test for setPeers
 */
TEST_F(DownloadTest, peersTrivial){
	
	download->setID(1);
	int peers = 5;
	download->setPeers(peers);
	
	EXPECT_EQ(peers, download->getStatistics().peers);
}

/**
 * Try setting the peers to a negative number
 */
TEST_F(DownloadTest, peersNegative){
	
	download->setID(1);
	int originalPeers = 5;
	download->setPeers(originalPeers);
	int peers = -1;
	download->setPeers(peers);
	
	EXPECT_EQ(originalPeers, download->getStatistics().peers);
}

/***********************************
***** END DOWNLOAD STATS TESTS *****
***********************************/

/* Status */

/**
 * Trivial test for setStatus
 */
TEST_F(DownloadTest, statusTrivial){
	
	download->setID(1);
	download->setStatus(DOWNLOADING);
	EXPECT_EQ(DOWNLOADING, download->getStatus());
}

/**
 * Try setting the status to a status that doesn't exist
 */
TEST_F(DownloadTest, statusWrong){
	
	download->setID(1);
	int status = download->getStatus();
	download->setStatus(-1);
	
	EXPECT_EQ(status, download->getStatus());
}

/* Start */

/**
 * Trivial test for start
 */
TEST_F(DownloadTest, startTrivial){
	
	EXPECT_EQ(READY, download->getStatus());
	download->start();
	EXPECT_EQ(DOWNLOADING, download->getStatus());
}

/**
 * Try starting a download that has already started
 */
TEST_F(DownloadTest, startAlreadyStarted){
	
	download->start();
	download->start();
	EXPECT_EQ(DOWNLOADING, download->getStatus());
	
}

/* Pause */

/**
 * Trivial test for pause
 */
TEST_F(DownloadTest, pauseTrivial){
	
	download->start();
	EXPECT_EQ(DOWNLOADING, download->getStatus());
	download->pause();
	EXPECT_EQ(PAUSED, download->getStatus());
}

/**
 * Try pausing a download that has not started yet
 */
TEST_F(DownloadTest, pauseNeverStarted) {
	
	download->pause();
	EXPECT_EQ(PAUSED, download->getStatus());
}

/**
 * Try pausing a download that is already paused
 */
TEST_F(DownloadTest, pauseAlreadyPaused) {
	
	download->start();
	download->pause();
	download->pause();
	EXPECT_EQ(PAUSED, download->getStatus());
	
}

/* Resume */

/**
 * Trivial test for resume
 */
TEST_F(DownloadTest, resumeTrivial){
	
	download->start();
	download->pause();
	EXPECT_EQ(PAUSED, download->getStatus());
	download->resume();
	EXPECT_EQ(DOWNLOADING, download->getStatus());
}

/**
 * Try resuming a download that is already downlaoding
 */
TEST_F(DownloadTest, resumeAlreadyDownloading) {
	
	download->start();
	download->pause();
	download->resume();
	
	download->resume();
	EXPECT_EQ(DOWNLOADING, download->getStatus());
}

/* Stop */

/**
 * Trivial test for stop
 */
TEST_F(DownloadTest, stopTrivial){
	
	download->start();
	download->stop();
	EXPECT_EQ(STOPPED, download->getStatus());
}

/**
 * Try stopping a download that never started
 */
TEST_F(DownloadTest, stopNotStarted){
	
	download->stop();
	EXPECT_EQ(STOPPED, download->getStatus());
}

/**
 * Stop a download that is paused
 */
TEST_F(DownloadTest, stopWhilePaused) {
	
	download->pause();
	download->stop();
	EXPECT_EQ(STOPPED, download->getStatus());
}

/**
 * Stop a download that is already stopped
 */
TEST_F(DownloadTest, stopAlreadyStopped){
	
	download->stop();
	EXPECT_EQ(STOPPED, download->getStatus());
	
	download->stop();
	EXPECT_EQ(STOPPED, download->getStatus());
}
