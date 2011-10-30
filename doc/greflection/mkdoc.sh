#!/usr/bin/sh

GEN="perl ../../scripts/gendoc.pl"
SRC_PATH=source
OUT_PATH=output

if [ ! -d $OUT_PATH ]; then
	mkdir $OUT_PATH
fi

for input in $SRC_PATH/*.txt
do
	$GEN -o=$OUT_PATH $input
done
