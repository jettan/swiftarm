#!/bin/sh -x
#

SRC_DIR="/home/jettan/swiftarm/libevent2"
INSTALL_DIR="/home/jettan/swiftarm/lib2"


cd $SRC_DIR

export CC="arm-v7a8-linux-gnueabi-gcc"
export CCFLAGS="-static"
export LD="arm-v7a8-linux-gnueabi-ld"
export AR="arm-v7a8-linux-gnueabi-ar"
export RANLIB="arm-v7a8-linux-gnueabi-ranlib"
export STRIP="arm-v7a8-linux-gnueabi-strip"
export LDFLAGS="-rpath /home/jettan/swiftarm/libevent2/include/event2"$LDFLAGS
export LDADD="-lgcc -lc -lrt"

./configure --prefix=$INSTALL_DIR LIBS="-lc -lgcc -lrt"

make
make install

exit 0
