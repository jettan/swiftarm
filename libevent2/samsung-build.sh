#!/bin/sh -x
#

INSTALL_DIR="/home/jettan/swiftarm/lib"


cd $(HOME)/swiftarm/libevent2

export CC="arm-v7a8-linux-gnueabi-gcc"
export CFLAGS="-static"
export LD="arm-v7a8-linux-gnueabi-ld"
export AR="arm-v7a8-linux-gnueabi-ar"
export RANLIB="arm-v7a8-linux-gnueabi-ranlib"
export STRIP="arm-v7a8-linux-gnueabi-strip"
export LDADD="/opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/libc.a /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/librt.a /opt/armv7a8-toolchain-lite/lib/gcc/arm-v7a8-linux-gnueabi/4.4.1/libgcc.a"
export LIBS="$LDADD"

./configure --host=arm-v7a8-linux-gnueabi --prefix=$INSTALL_DIR --disable-shared

exit 0
