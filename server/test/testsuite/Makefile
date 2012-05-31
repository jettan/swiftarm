GTEST = ../gtest-1.6.0
CPPFLAGS = -I $(SWIFTARM)/server/include -I $(SWIFTARM)/server/lib/include -I $(GTEST)/include/ -Wall
CXX = g++
GTESTLIB = $(GTEST)/lib/libgtest.a $(GTEST)/lib/libgtest_main.a
LDADD=$(SWIFTARM)/server/lib/libswift.a $(SWIFTARM)/server/lib/lib/libevent_pthreads.a $(SWIFTARM)/server/lib/lib/libevent.a -lrt -lstdc++ -lgcc -lutil -lpthread
SOURCE = ../../src


all: testsuite

testsuite: Main.o DownloadTest.o DownloadManagerTest.o HTTPServerTest.o Download.o DownloadManager.o HttpServer.o
	$(CXX) $(CPPFLAGS) -o testsuite *.o ${GTESTLIB} ${LDADD}

Main.o: Main.cpp
	$(CXX) $(CPPFLAGS) Main.cpp -c

DownloadTest.o: DownloadTest.cpp
	$(CXX) $(CPPFLAGS) DownloadTest.cpp -c

DownloadManagerTest.o: DownloadManagerTest.cpp
	$(CXX) $(CPPFLAGS) DownloadManagerTest.cpp -c

HTTPServerTest.o: HTTPServerTest.cpp
	$(CXX) $(CPPFLAGS) HTTPServerTest.cpp -c

Download.o: $(SOURCE)/Download.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/Download.cpp -c

DownloadManager.o: $(SOURCE)/DownloadManager.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/DownloadManager.cpp -c

HttpServer.o: $(SOURCE)/HttpServer.cpp
	$(CXX) $(CPPFLAGS) $(SOURCE)/HttpServer.cpp -c

clean:
	rm -f *.o testsuite 2>/dev/null

.PHONY: clean all
