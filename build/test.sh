#!/usr/bin/sh

mingw32-make mingw TARGET=test

if [ $? != 0 ]; then
	echo Error occurred.
	exit 1
fi

bin/test
