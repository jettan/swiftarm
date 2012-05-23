#!/bin/bash
args=("$@")

if [ ${args[0]} ]; then
	if [ ${args[0]} == "arm" ]; then
		export CXX="arm-v7a8-linux-gnueabi-g++"
	fi
fi


make
