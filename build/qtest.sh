#BUILD_TARGET=test_serialization
BUILD_TARGET=test_scriptbind
#BUILD_TARGET=test_reflection
#BUILD_TARGET=test_misc
#BUILD_TARGET=test_metagen
#BUILD_TARGET=tutorials

if [ $OSTYPE == "cygwin" ]; then
	mingw32-make mingw TARGET=$BUILD_TARGET
else
	make linux TARGET=$BUILD_TARGET
fi

if [ $? != 0 ]; then
	echo Error occurred.
	exit 1
fi

cd ./bin
./$BUILD_TARGET
cd ../

