#!/usr/bin/sh

if [ $# -ne 2 ]; then
	echo 1>&2 Usage: $0 destpath version
	exit 1
fi

SHOME=$(dirname "$0")
SRC=$SHOME/..
DEST=$1/cpgf
VERSION=$2
ZIPNAME=cpgf_$VERSION.zip

echo copy from $SRC to $DEST

if [ -d $DEST ]; then
	echo $DEST exists
	exit 1
fi

mkdir $1
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

copy_file "" "readme.txt"
copy_file "" "license"
copy_file "" "changelog"

copy_file "build" "*"
mkdir $DEST/lib

head_file "include/cpgf" "*"
head_file "include/cpgf/private" "*"
head_file "include/cpgf/scriptbind" "*"
head_file "include/cpgf/metadata" "*"
head_file "include/cpgf/metadata/stl" "*"
head_file "include/cpgf/metadata/stl/private" "*"

head_file "src" "*"
head_file "src/pinclude" "*"
head_file "src/scriptbind" "*"

head_file "samples" "*"
head_file "samples/callback" "*"
head_file "samples/reflection" "*"
head_file "samples/scriptbind" "*"
head_file "samples/scriptbind/luabind" "*"

head_file "test" "*"
head_file "test/benchmark/callback" "*"
head_file "test/callback" "*"
head_file "test/misc" "*"
head_file "test/reflection" "*"
head_file "test/scriptbind" "*"
head_file "test/scriptbind/general" "*"
head_file "test/scriptbind/luabind" "*"
head_file "test/scriptbind/v8bind" "*"
head_file "test/scriptbind/metadata" "*"
head_file "test/scriptbind/metadata/stl" "*"

cd $1

if [ $? != 0 ]; then
	echo "Can't change directory for zip."
	exit 1
fi


zip -rq $ZIPNAME cpgf
cd ..

