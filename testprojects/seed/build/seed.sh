#!/bin/bash

if [ $OSTYPE == "cygwin" ]; then
	mingw32-make mingw
else
	make linux
fi

if [ $? != 0 ]; then
	echo Error occurred.
	exit 1
fi

cd ./bin
./seed
cd ../

