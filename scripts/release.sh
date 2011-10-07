#!/usr/bin/sh

if [ $# -ne 1 ]; then
	echo 1>&2 Usage: $0 destpath
	exit 1
fi

SHOME=$(dirname "$0")
SRC=$SHOME/..
DEST=$1/cpgf
echo copy from $SRC to $DEST

if [ -d $DEST ]; then
	echo $DEST exists
	exit 1
fi

mkdir $DEST

copy_file()
{
	SUB_PATH=$1
	FILES=$2

	if [ ! -d $DEST/$SUB_PATH ]; then
		mkdir -p $DEST/$SUB_PATH
	fi

	cp -p $SRC/$SUB_PATH/$FILES $DEST/$SUB_PATH
}

head_file()
{
	SUB_PATH=$1
	FILES=$2

	if [ ! -d $DEST/$SUB_PATH ]; then
		mkdir -p $DEST/$SUB_PATH
	fi

	cp -p $SRC/$SUB_PATH/$FILES $DEST/$SUB_PATH

	perl $SHOME/file_prefix.pl $SHOME/licensehead_apache.txt $DEST/$SUB_PATH/*.h
	perl $SHOME/file_prefix.pl $SHOME/licensehead_apache.txt $DEST/$SUB_PATH/*.cpp
}

copy_file "build" "*"
mkdir $DEST/lib

head_file "include/cpgf" "*"
head_file "include/cpgf/scriptbind" "*"

head_file "src" "*"
head_file "src/pinclude" "*"
head_file "src/scriptbind" "*"

copy_file "samples" "*"
copy_file "samples/callback" "*"
copy_file "samples/reflection" "*"
copy_file "samples/scriptbind" "*"
copy_file "samples/scriptbind/luabind" "*"

copy_file "test" "*"
copy_file "test/benchmark/callback" "*"
copy_file "test/callback" "*"
copy_file "test/misc" "*"
copy_file "test/reflection" "*"
copy_file "test/scriptbind" "*"
copy_file "test/scriptbind/luabind" "*"
