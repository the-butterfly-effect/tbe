#!/bin/bash

if test -z "$1" -o -z "$2";
then
  echo "$0 <from> <target>"
  echo "where:"
  echo "  <from>   the directory to get files from"
  echo "  <target> the directory to put files into"
  echo "           make sure to use a target name that contains a version number"
  echo "           e.g.   TheButterFlyEffect-m7";
  echo
  echo "Please refer to https://sourceforge.net/apps/trac/tbe/wiki/HowToMakeARelease for more info" 
  echo
  exit 1;
fi;

if test -d "$2";
then
  echo "directory '$2' already exists";
  exit 2;
fi;

# build the target directory and remove all subversion cruft
echo "* Building directory '$2'";
cp -a $1 $2;
find $2 -name '.svn' -exec rm -rf {} \; 2>/dev/null

MANIFEST=manifest.txt

# files from the base directory:
echo "* Building manifest file '$MANIFEST' for the source archive";
echo "$2/COPYING"     > $MANIFEST
echo "$2/AUTHORS"     >> $MANIFEST
echo "$2/README"      >> $MANIFEST
echo "$2/INSTALLING"  >> $MANIFEST
echo "$2/tbe.pro"     >> $MANIFEST
echo "$2/configure"   >> $MANIFEST
echo "$2/make-release.sh" >> $MANIFEST
echo "$2/make-source-archive.sh"   >> $MANIFEST
find $2/i18n -name '*.ts' >> $MANIFEST
find $2/installer >>$MANIFEST
find $2/src       >>$MANIFEST
find $2/3rdParty  >>$MANIFEST
echo "$2/images/illustrations/tbe-icon.png" >>$MANIFEST
echo "$2/images/illustrations/tbe-icon.qrc" >>$MANIFEST

make -C $2/images all >/dev/null;
find $2/images  -maxdepth 1 -type f  >>$MANIFEST
find $2/levels/draft -name '*.xml' -o -name '*.svg' -o -name '*.png' -o -name 'README' >>$MANIFEST
find $2/levels/elce09 -name '*.xml' -o -name '*.svg' -o -name '*.png' -o -name 'README' >>$MANIFEST
echo $2/levels/levels.xml >>$MANIFEST

# for pure joy, let's sort the manifest
echo "* Sorting the entries of the manifest";
sort $MANIFEST >$MANIFEST.tmp
mv $MANIFEST.tmp $MANIFEST

# and now... create the source archive...
echo "* Creating the tarball from the manifest";
tar czf $2.src.tgz --files-from=$MANIFEST

