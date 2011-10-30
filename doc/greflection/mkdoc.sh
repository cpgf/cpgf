#!/bin/sh

GEN="perl ../../scripts/gendoc.pl"
SRC_PATH=source
OUT_PATH=output

if [ ! -d $OUT_PATH ]; then
	mkdir $OUT_PATH
fi

for input in $SRC_PATH/*.txt
do
	$GEN -o=$OUT_PATH $input
	if [ $? != 0 ]; then
		echo Error when process file $input.
		exit 1
	fi
done
