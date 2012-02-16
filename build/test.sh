#!/bin/bash

if [ $OSTYPE == "cygwin" ]; then
	mingw32-make mingw TARGET=test
else
	make linux TARGET=test
fi

if [ $? != 0 ]; then
	echo Error occurred.
	exit 1
fi

#find temp_cache/ -name \*.gcda | xargs rm

cd ./bin
./test
cd ../

