#!/bin/bash
args=("$@")
export system=`uname -m`
echo $system
if [ ${args[0]} ]; then
	if [ ${args[0]} == "arm" ]; then
		export CXX="arm-v7a8-linux-gnueabi-g++"
	fi
fi


make
