BUILD_TARGET=$1
if [ -z "$BUILD_TARGET" ]; then
#BUILD_TARGET=test_serialization
#BUILD_TARGET=test_scriptbind
#BUILD_TARGET=test_reflection
#BUILD_TARGET=test_misc
#BUILD_TARGET=test_metagen
#BUILD_TARGET=test_variant
#BUILD_TARGET=tutorials
#BUILD_TARGET=test_metadata_stl
#BUILD_TARGET=test_accessor
BUILD_TARGET=test_tween
fi

echo BULD TARGET $BUILD_TARGET

if [ $OSTYPE == "cygwin" ]; then
	mingw32-make mingw TARGET=$BUILD_TARGET
else
	make linux TARGET=$BUILD_TARGET
fi

if [ $? != 0 ]; then
	echo Error occurred.
	exit $?
fi

cd ./bin
./$BUILD_TARGET

exit $?
