CXX=arm-v7a8-linux-gnueabi-g++
CPPFLAGS+= -static -O2 -I. -Wall -Wno-sign-compare -Wno-unused -g -I $(HOME)/swiftarm/lib/include -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE 
LD=arm-v7a8-linux-gnueabi-ld
LDADD=$(HOME)/swiftarm/lib/lib/libevent.a /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/lib/libstdc++.a /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/libc.a /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/librt.a /opt/armv7a8-toolchain-lite/lib/gcc/arm-v7a8-linux-gnueabi/4.4.1/libgcc.a

all: swift

swift: swift.o sha1.o compat.o sendrecv.o send_control.o hashtree.o bin.o binmap.o binheap.o channel.o transfer.o httpgw.o statsgw.o cmdgw.o avgspeed.o availability.o
#nat_test.o
	$(CXX) ${CPPFLAGS} -o swift *.o  ${LDADD}

clean:
	rm *.o swift 2>/dev/null

.PHONY: all clean
