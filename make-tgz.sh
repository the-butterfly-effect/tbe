#!/bin/bash

if test -z "$1";
then
  echo "Please provide a version number!";
  exit 1;
fi;

FILELIST=manifest.txt

# files from the base directory:
echo "COPYING" > $FILELIST
echo "AUTHORS" >> $FILELIST
echo "README"  >> $FILELIST
echo "tbe"     >> $FILELIST

# all images in the images directory
# including the README - which should contain all resources
find images  -maxdepth 1 -a \( -name '*.png' -o -name '*.svg' -o -name README \) | sort >>$FILELIST

find levels -name '*.xml' -o -name '*.svg' -o -name '*.png' -o -name 'README' | sort >>$FILELIST

cat $FILELIST 

tar czvf tbe-$1.tgz --files-from=$FILELIST
