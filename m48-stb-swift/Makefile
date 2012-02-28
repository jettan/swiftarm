LIBEVENT_HOME=/home/jettan/swiftarm/lib2

CXX=arm-v7a8-linux-gnueabi-g++
CPPFLAGS+=-O2 -I. -Wall -Wno-sign-compare -Wno-unused -g -I${LIBEVENT_HOME}/include -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
LD=arm-v7a8-linux-gnueabi-ld
LDFLAGS+=-L${LIBEVENT_HOME}/lib -Wl,-rpath,${LIBEVENT_HOME}/lib -levent -lstdc++
LDADD+=-lgcc -lc -lrt

all: swift

swift: swift.o sha1.o compat.o sendrecv.o send_control.o hashtree.o bin.o binmap.o binheap.o channel.o transfer.o httpgw.o statsgw.o cmdgw.o avgspeed.o availability.o
#nat_test.o
	$(CXX) ${CPPFLAGS} -o swift *.o ${LDFLAGS} ${LDADD}

clean:
	rm *.o swift 2>/dev/null

.PHONY: all clean
