
cppflags_for_gpp= -O2 -I . -I $(SWIFTARM)/server/include -I $(SWIFTARM)/server/lib/include
cppflags_for_arm= -O2 -I . -I $(SWIFTARM)/server/include -I $(SWIFTARM)/server/lib-arm/include -I /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/include -I /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/include

ifeq ($(CXX),g++)
	CPPFLAGS=$(cppflags_for_gpp)
	LDADD=$(SWIFTARM)/server/lib/lib/libevent_pthreads.a $(SWIFTARM)/server/lib/lib/libevent.a $(SWIFTARM)/server/lib/libswift64.a -lrt -lstdc++ -lgcc -lutil -lpthread
else
	CPPFLAGS=$(cppflags_for_arm)
	LD=arm-v7a8-linux-gnueabi-ld
	LDADD=$(SWIFTARM)/server/lib-arm/lib/libevent_pthreads.a $(SWIFTARM)/server/lib-arm/lib/libevent.a $(SWIFTARM)/server/lib-arm/libswift.a -lrt -lstdc++ -lgcc -lutil -lpthread
endif

CPPFLAGS +=-Wall -Wno-sign-compare -Wno-unused -g -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE 

all: main

main: Main.o HttpServer.o Download.o DownloadManager.o
	$(CXX) ${CPPFLAGS} -o ws *.o ${LDADD}

Main.o: Main.cpp
	$(CXX) $(CPPFLAGS) Main.cpp -c

Download.o: Download.cpp ../include/Download.h
	$(CXX) $(CPPFLAGS) Download.cpp -c

DownloadManager.o: DownloadManager.cpp ../include/DownloadManager.h
	$(CXX) $(CPPFLAGS) DownloadManager.cpp -c

HttpServer.o: HttpServer.cpp ../include/HttpServer.h
	$(CXX) $(CPPFLAGS) HttpServer.cpp -c

clean:
	rm -f *.o server 2>/dev/null

.PHONY: clean all
