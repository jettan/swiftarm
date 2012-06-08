GTEST = ../gtest-1.6.0
CURL = ../curl
TICPP = ../..
CPPFLAGS = -I $(SWIFTARM)/server/include/ticpp -I $(SWIFTARM)/server/include/ticpp -I $(SWIFTARM)/server/lib/include/swift -I $(GTEST)/include/ -I $(CURL)/include -Wall
CXX = g++
LIBS = $(GTEST)/lib/libgtest.a $(GTEST)/lib/libgtest_main.a $(CURL)/lib/libcurl.a $(TICPP)/lib/libticpp.a
LDADD=$(SWIFTARM)/server/lib/libswift.a $(SWIFTARM)/server/lib/lib/libevent_pthreads.a $(SWIFTARM)/server/lib/lib/libevent.a -lrt -lstdc++ -lgcc -lutil -lpthread
SOURCE = ../../src

all: testsuite

testsuite: Main.o DownloadTest.o DownloadManagerTest.o HTTPServerTest.o Download.o StreamTest.o DownloadManager.o HttpServer.o Stream.o SearchEngine.o
	$(CXX) $(CPPFLAGS) -o testsuite *.o ${LIBS} ${LDADD}

Main.o: Main.cpp
	$(CXX) $(CPPFLAGS) Main.cpp -c

DownloadTest.o: DownloadTest.cpp
	$(CXX) $(CPPFLAGS) DownloadTest.cpp -c

StreamTest.o: StreamTest.cpp
	$(CXX) $(CPPFLAGS) StreamTest.cpp -c

DownloadManagerTest.o: DownloadManagerTest.cpp
	$(CXX) $(CPPFLAGS) DownloadManagerTest.cpp -c

HTTPServerTest.o: HTTPServerTest.cpp
	$(CXX) $(CPPFLAGS) HTTPServerTest.cpp -c

Download.o: $(SOURCE)/Download.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/Download.cpp -c

Stream.o: $(SOURCE)/Stream.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/Stream.cpp -c

DownloadManager.o: $(SOURCE)/DownloadManager.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/DownloadManager.cpp -c

HttpServer.o: $(SOURCE)/HttpServer.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/HttpServer.cpp -c

SearchEngine.o: $(SOURCE)/SearchEngine.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/SearchEngine.cpp -c

clean:
	rm -f *.o testsuite 2>/dev/null

.PHONY: clean all
