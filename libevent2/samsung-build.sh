#!/bin/sh -x
#

INSTALL_DIR="/home/jettan/Documents/swiftarm/lib"

export CC="arm-v7a8-linux-gnueabi-gcc"
#export CFLAGS="-static -lgcc -lrt"
export CFLAGS="-static"
export LD="arm-v7a8-linux-gnueabi-ld"
export AR="arm-v7a8-linux-gnueabi-ar"
export RANLIB="arm-v7a8-linux-gnueabi-ranlib"
export STRIP="arm-v7a8-linux-gnueabi-strip"
export LDADD="/opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/libc.a /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/libpthread.a /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/librt.a /opt/armv7a8-toolchain-lite/lib/gcc/arm-v7a8-linux-gnueabi/4.4.1/libgcc.a"
export LIBS="$LDADD"

./configure --host=arm-v7a8-linux-gnueabi --prefix=$INSTALL_DIR --disable-shared
#./configure --prefix=$INSTALL_DIR --disable-shared
exit 0
