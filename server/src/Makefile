cppflags_for_gpp= -I $(SWIFTARM)/server/include -I $(SWIFTARM)/server/lib/include -I /usr/include/python2.7
cppflags_for_x86_64= -O2 -I . -I $(SWIFTARM)/server/include -I $(SWIFTARM)/server/lib/include64 -I /usr/include/python2.7
cppflags_for_arm= -O2 -I . -I $(SWIFTARM)/server/include -I $(SWIFTARM)/server/lib-arm/include -I /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/include -I /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/include -I $(SWIFTARM)/server/include/python2.6

ifeq ($(CXX),g++)
	ifeq ($(system),x86_64)
		CPPFLAGS=$(cppflags_for_x86_64)
		LDADD=$(SWIFTARM)/server/lib/swift64.a $(SWIFTARM)/server/lib/lib64/libevent_pthreads.a $(SWIFTARM)/server/lib/lib64/libevent.a $(SWIFTARM)/server/lib/lib64/libticpp.a -lrt -lstdc++ -lgcc -lutil -lpthread -lpython2.7
	else
		CPPFLAGS=$(cppflags_for_gpp)
		LDADD=$(SWIFTARM)/server/lib/swift.a $(SWIFTARM)/server/lib/lib/libevent_pthreads.a $(SWIFTARM)/server/lib/lib/libevent.a $(SWIFTARM)/server/lib/libticpp.a -lrt -lstdc++ -lgcc -lutil -lpthread -lpython2.7
	endif
else
	CPPFLAGS=$(cppflags_for_arm)
	LD=arm-v7a8-linux-gnueabi-ld
	LDADD=$(SWIFTARM)/server/lib-arm/swift.a $(SWIFTARM)/server/lib-arm/lib/libevent_pthreads.a $(SWIFTARM)/server/lib-arm/lib/libevent.a $(SWIFTARM)/server/lib-arm/libticpp.a -lpython2.6 -lz -lrt -lstdc++ -lgcc -lutil -lpthread
endif

LDPATH+= /lib/libs
CPPFLAGS += -I $(SWIFTARM)/server/include/ticpp -I $(SWIFTARM)/server/include/swift -Wall -Wno-sign-compare -Wno-unused -g -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE 

all: main

main: Main.o Download.o Stream.o Settings.o DownloadManager.o HttpServer.o SearchEngine.o
	$(CXX) ${CPPFLAGS} -o ws *.o ${LDADD}

Main.o: Main.cpp
	$(CXX) $(CPPFLAGS) Main.cpp -c

Settings.o: Settings.cpp
	$(CXX) $(CPPFLAGS) Settings.cpp -c

Stream.o: Stream.cpp
	$(CXX) $(CPPFLAGS) Stream.cpp -c

Download.o: Download.cpp
	$(CXX) $(CPPFLAGS) Download.cpp -c

DownloadManager.o: DownloadManager.cpp
	$(CXX) $(CPPFLAGS) DownloadManager.cpp -c

HttpServer.o: HttpServer.cpp
	$(CXX) $(CPPFLAGS) HttpServer.cpp -c

SearchEngine.o: SearchEngine.cpp
	$(CXX) $(CPPFLAGS) SearchEngine.cpp -c

clean:
	rm -f *.o *.pyo *.pyc ws 2>/dev/null

.PHONY: clean all
