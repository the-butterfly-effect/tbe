#/bin/bash

# Script with gettext support for levels for thebutterflyeffect
# License: GPLv3+
# Author: Alexey Loginov <alexl@mageia.org>, 2015
# Adjusted by Klaas van Gend to match new i18n setup of TBE, January 2016

function create_pot_file {
  echo "Creating POT file..."
  # delete POT if exists
  rm -f i18n/tbe_levels.pot
  # prepare ./tmp directory for work
  rm -rf ./tmp
  mkdir -p ./tmp
  # find all XML files except of levels.xml
  for fname in `find "$lev_dir" -name "*.xml"|grep -v "levels.xml"`
  do
    # write content from all XML files in one file
    cat "$fname" >> tmp/tmp.h
  done
  # use temporary marker N_ for start of strings and temporary marker _N for end of strings, which should be extracted
  cat tmp/tmp.h|grep "<toolboxitem"|grep "name="|sed "s|<toolboxitem.*name=\"|N_|g"|sed "s|\".*|_N|g" > tmp/tmp0.h
  sed -i 's|<title>|N_|g' tmp/tmp.h
  sed -i 's|</title>|_N|g' tmp/tmp.h
  sed -i 's|<description>|N_|g' tmp/tmp.h
  sed -i 's|</description>|_N|g' tmp/tmp.h
  sed -i 's|</property>|_N|g' tmp/tmp.h
  sed -i 's|<property key="page.*">|N_|g' tmp/tmp.h
  sed -i 's|<tooltip>|N_|g' tmp/tmp.h
  sed -i 's|</tooltip>|_N|g' tmp/tmp.h
  sed -i 's|<object.*>N_|N_|g' tmp/tmp.h
  # we need all strings, which were marked with N_, but we don't need empty for translation strings (they were marked N__N)
  cat tmp/tmp.h|grep 'N_'|grep -v 'N__N' >> tmp/tmp0.h
  # change symbol \ on double symbol \ else POT file will have wrong format
  sed -i 's|\\|\\\\|g' tmp/tmp0.h
  # all symbols " should be marked "\ for POT format
  sed -i 's|"|\\"|g' tmp/tmp0.h
  # change temporary marker N_ on N_(" for C++ format for xgettext
  sed -i 's|N_|N_("|g' tmp/tmp0.h
  # change temporary marker _N ") for C++ format for xgettext
  sed -i 's|_N|")|g' tmp/tmp0.h
  # we should think about translators; it's easier for them to translate < instead of &lt;
  sed -i 's|\&lt;|<|g' tmp/tmp0.h
  # we should think about translators; it's easier for them to translate > instead of &gt;
  sed -i 's|\&gt;|>|g' tmp/tmp0.h
  # sort file and leave strings without duplicates
  cat tmp/tmp0.h|sort|uniq > tbe_levels
  # extract all translatable strings into POT file
  xgettext tbe_levels -C --keyword=N_ --from-code=utf-8 -o i18n/tbe_levels.pot
  # prepare ./tmp directory with all temporary files
  rm -rf ./tmp tbe_levels
  echo "Done"
}


function check_dependencies {
  echo "Checking dependencies..."
  # we need only gettext installed, but we check all critical binaries for this script
  a=`which msgfmt`
  if [ "$a" = "" ]
  then
    echo "Error: missing msgfmt."
    exit 1
  fi
  a=`which xgettext`
  if [ "$a" = "" ]
  then
    echo "Error: missing xgettext."
    exit 1
  fi
  a=`which gettext`
  if [ "$a" = "" ]
  then
    echo "Error: missing gettext."
    exit 1
  fi
  echo "Done"
}

echo "Starting tbe_levels_i18n.sh script..."

lev_dir="levels"

check_dependencies
create_pot_file

echo "Script tbe_levels_i18n.sh was finished"
