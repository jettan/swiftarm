GTEST = ../gtest-1.6.0
CPPFLAGS = -I $(SWIFTARM)/server/include -I $(SWIFTARM)/server/lib/include -I $(GTEST)/include/ -Wall
CXX = g++
GTESTLIB = $(GTEST)/lib/libgtest.a $(GTEST)/lib/libgtest_main.a
LDADD=$(SWIFTARM)/server/lib/libswift64.a $(SWIFTARM)/server/lib/lib64/libevent_pthreads.a $(SWIFTARM)/server/lib/lib64/libevent.a -lrt -lstdc++ -lgcc -lutil -lpthread

all: testsuite

testsuite: Main.o DownloadTest.o DownloadManagerTest.o HTTPServerTest.o
	$(CXX) $(CPPFLAGS) -o testsuite *.o ${GTESTLIB} ${LDADD}

Main.o: Main.cpp
	$(CXX) $(CPPFLAGS) Main.cpp -c

DownloadTest.o: DownloadTest.cpp
	$(CXX) $(CPPFLAGS) DownloadTest.cpp -c

DownloadManagerTest.o: DownloadManagerTest.cpp
	$(CXX) $(CPPFLAGS) DownloadManagerTest.cpp -c

HTTPServerTest.o: HTTPServerTest.cpp
	$(CXX) $(CPPFLAGS) HTTPServerTest.cpp -c

