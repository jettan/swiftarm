#!/bin/sh -x
#
# FourierTest/build.sh
# Compiles libevent2 for Android
# Make sure you have NDK_ROOT defined in .bashrc or .bash_profile

NDK_ROOT="/arno/build/android-ndk-r7"
INSTALL_DIR="/arno/co/ttuki/android/jni/libevent2"
SRC_DIR="/arno/co/ttuki/android/libevent-2.0.15-arno-http"

cd $SRC_DIR

export PATH="$NDK_ROOT/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/:$PATH"
export SYS_ROOT="$NDK_ROOT/platforms/android-14/arch-arm/"
export CC="arm-linux-androideabi-gcc --sysroot=$SYS_ROOT"
export LD="arm-linux-androideabi-ld"
export AR="arm-linux-androideabi-ar"
export RANLIB="arm-linux-androideabi-ranlib"
export STRIP="arm-linux-androideabi-strip"

mkdir -p $INSTALL_DIR
./configure --host=arm-eabi --build=i686-unknown-linux-gnu --prefix=$INSTALL_DIR LIBS="-lc -lgcc"

make
make install

exit 0

