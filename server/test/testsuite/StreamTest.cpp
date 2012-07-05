#include "Download.h"
#include "Stream.h"
#include "gtest.h"
#include <string>
#include <iostream>

/**
 * This is the class that tests Stream.cpp.
 * The setup sets the tracker.
 * The teardown stops the stream if it is still active.
 */
class StreamTest : public ::testing::Test {
	protected:
	
	virtual ~StreamTest() {}
	
	virtual void SetUp() {
	
		Stream::getInstance()->setTracker("127.0.0.1:9999");
	}
	
	virtual void TearDown() {
		
		if(Stream::getInstance()->readStreaming()) {
			Stream::getInstance()->stop();
		}
	}
	
};

/**
 * Check whether Stream always returns the same instance.
 */
TEST_F(StreamTest, getInstance) {
	
	// Check whether pointer address is equal.
	EXPECT_EQ(Stream::getInstance(), Stream::getInstance());
}

/* Start */

/**
 * Trivial test for start.
 */
TEST_F(StreamTest, startStreamTrivial) {
	
	Stream::getInstance()->start();
	EXPECT_EQ(true, Stream::getInstance()->readStreaming());
}

/**
 * Start stream twice.
 */
TEST_F(StreamTest, startTwice) {
	
	Stream::getInstance()->start();
	Stream::getInstance()->start();
	EXPECT_EQ(true, Stream::getInstance()->readStreaming());
}

/**
 * Start, stop and start the stream again.
 */
TEST_F(StreamTest, startStopStart) {
	
	Stream::getInstance()->start();
	Stream::getInstance()->stop();
	Stream::getInstance()->start();
	EXPECT_EQ(true, Stream::getInstance()->readStreaming());
}

/* Stop */

/**
 * Trivial test for stop.
 */
TEST_F(StreamTest, stopTrivial) {
	
	Stream::getInstance()->start();
	Stream::getInstance()->stop();
	EXPECT_EQ(false, Stream::getInstance()->readStreaming());
}

/**
 * Stop the stream twice.
 */
TEST_F(StreamTest, stopTwice) {
	
	Stream::getInstance()->start();
	Stream::getInstance()->stop();
	Stream::getInstance()->stop();
	EXPECT_EQ(false, Stream::getInstance()->readStreaming());
}

