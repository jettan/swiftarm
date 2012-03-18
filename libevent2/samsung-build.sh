#!/bin/sh -x
#

SRC_DIR="/home/jettan/Documents/swiftarm/libevent2"
INSTALL_DIR="/home/jettan/Documents/swiftarm/lib"


cd $SRC_DIR

export CC="arm-v7a8-linux-gnueabi-gcc"
export CCFLAGS="-static"
export LD="arm-v7a8-linux-gnueabi-ld"
export AR="arm-v7a8-linux-gnueabi-ar"
export RANLIB="arm-v7a8-linux-gnueabi-ranlib"
export STRIP="arm-v7a8-linux-gnueabi-strip"
#export LDFLAGS="-rpath /home/jettan/Documents/swiftarm/libevent2/include/event2"$LDFLAGS
export LDADD="-lgcc -lc -lrt"

./configure --host=arm-v7a8-linux-gnueabi --prefix=$INSTALL_DIR LIBS="-lc -lgcc -lrt"

exit 0
