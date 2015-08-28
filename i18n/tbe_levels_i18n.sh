#/bin/bash

# Script with gettext support for levels for thebutterflyeffect
# License: GPLv3+
# Author: Alexey Loginov <alexl@mageia.org>, 2015

function prepare_for_parsing {
  echo "Preparing for parsing..."
  # delete temporary level directory
  rm -rf "$lev_dir"_
  # copy level directory into temporary level directory
  cp -af "$lev_dir" "$lev_dir"_
  # find all XML files except of levels.xml
  for fname in `find "$lev_dir"_ -name "*.xml"|grep -v "levels.xml"`
  do
    # we don't need more than one space
    cat "$fname"|tr -s [:blank:] ' ' > "$fname".tmp
    # we don't need tabs in the start of string
    sed -i 's/^[ \t]*//' "$fname".tmp
    # we don't need tabs in the end of string
    sed -i 's/[ \t]*$//' "$fname".tmp
    # delete all EOL
    cat "$fname".tmp|tr -d '\n' > "$fname".tmp0
    rm -f "$fname".tmp
    # restore all EOL
    sed -i 's|<title|\n<title|g' "$fname".tmp0
    sed -i 's|</title>|</title>\n|g' "$fname".tmp0
    sed -i 's|<description|\n<description|g' "$fname".tmp0
    sed -i 's|</description>|</description>\n|g' "$fname".tmp0
    sed -i 's|<property|\n<property|g' "$fname".tmp0
    sed -i 's|</property>|</property>\n|g' "$fname".tmp0
    sed -i 's|<levelinfo|\n<levelinfo|g' "$fname".tmp0
    sed -i 's|</levelinfo>|</levelinfo>\n|g' "$fname".tmp0
    sed -i 's|<author|\n<author|g' "$fname".tmp0
    sed -i 's|</author>|</author>\n|g' "$fname".tmp0
    sed -i 's|<date|\n<date|g' "$fname".tmp0
    sed -i 's|</date>|</date>\n|g' "$fname".tmp0
    sed -i 's|<license|\n<license|g' "$fname".tmp0
    sed -i 's|</license>|</license>\n|g' "$fname".tmp0
    sed -i 's|<object|\n<object|g' "$fname".tmp0
    sed -i 's|</object>|</object>\n|g' "$fname".tmp0
    sed -i 's|<predefined|\n<predefined|g' "$fname".tmp0
    sed -i 's|</predefined>|</predefined>\n|g' "$fname".tmp0
    sed -i 's|<scene|\n<scene|g' "$fname".tmp0
    sed -i 's|</scene>|</scene>\n|g' "$fname".tmp0
    sed -i 's|<goal|\n<goal|g' "$fname".tmp0
    sed -i 's|</goal>|</goal>\n|g' "$fname".tmp0
    sed -i 's|<goals|\n<goals|g' "$fname".tmp0
    sed -i 's|</goals>|</goals>\n|g' "$fname".tmp0
    sed -i 's|<tbe-level|\n<tbe-level|g' "$fname".tmp0
    sed -i 's|</tbe-level>|</tbe-level>\n|g' "$fname".tmp0
    sed -i 's|<toolbox|\n<toolbox|g' "$fname".tmp0
    sed -i 's|</toolbox>|</toolbox>\n|g' "$fname".tmp0
    sed -i 's|<toolboxitem|\n<toolboxitem|g' "$fname".tmp0
    sed -i 's|</toolboxitem>|</toolboxitem>\n|g' "$fname".tmp0
    sed -i 's|<gradientstop|\n<gradientstop|g' "$fname".tmp0
    sed -i 's|</gradientstop>|</gradientstop>\n|g' "$fname".tmp0
    sed -i 's|<!--|\n<!--|g' "$fname".tmp0
    # delete translated names
    sed -i 's|<name lang=.*</name>||g' "$fname".tmp0
    # delete all translated strings
    cat "$fname".tmp0|grep -v "_nl"|grep -v "lang=\"nl\""|grep -v "lang=\"de\""|grep -v "lang=\"fr\"" > "$fname".tmp1
    rm -f "$fname".tmp0
    # delete all empty strings
    sed -i '/^$/d' "$fname".tmp1
    # we don't need symbol >, which written with br
    sed -i 's|br>|br\&gt;|g' "$fname".tmp1
    # we don't need symbol >, which written with a
    sed -i 's|a>|a\&gt;|g' "$fname".tmp1
    # we don't need space before symbol >
    sed -i 's|" >|">|g' "$fname".tmp1
    mv -f "$fname".tmp1 "$fname"
  done
  echo "Done"
}

function create_pot_file {
  echo "Creating POT file..."
  # delete POT if exists
  rm -f i18n/tbe_levels.pot
  # prepare ./tmp directory for work
  rm -rf ./tmp
  mkdir -p ./tmp
  # find all XML files except of levels.xml
  for fname in `find "$lev_dir"_ -name "*.xml"|grep -v "levels.xml"`
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
  sed -i 's|<property key="Description">|N_|g' tmp/tmp.h
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
  cat tmp/tmp0.h|sort|uniq > tmp/tmp.h
  # extract all translatable strings into POT file
  xgettext tmp/tmp.h --keyword=N_ --from-code=utf-8 -o i18n/tbe_levels.pot
  # a little polish for POT file
  sed -i "s|tmp/tmp.h|tbe_levels|g" i18n/tbe_levels.pot
  # prepare ./tmp directory with all temporary files
  rm -rf ./tmp
  echo "Done"
}

function get_orig_str {
  # find all strings with $marker0>
  cat "$fname".tmp|grep "$marker0>"|while read str
  do
    # delete $marker0> and $marker1 from string
    str0=`echo "$str"|sed "s|$marker0>||g"|sed "s|$marker1||g"`
    # write string in temporary file (we add symbols N_ and symbols _N, because string can have start and end spaces)
    if [ ! "$str0" = "" ]
    then
      echo N_"$str0"_N > 1.tmp
    fi
  done
  if [ ! -f "1.tmp" ]
  # use dummy string instead of empty, it's easier to debug
  then
    orig_str="vvv"
  else
  # read temporary file: we have orig_str; we suppose that this string is only one line in file, because we know that $marker0 is only once in XML file
  # and we delete symbols N_ and symbols _N, which were added before
    orig_str=`cat 1.tmp|sed "s|^N_||g"|sed "s|_N$||g"`
  fi
#    echo "orig_str for $language: $orig_str"
  rm -f 1.tmp
}

function get_orig_str_name {
  # create file orig_str_name_full.tmp, which content all strings with marker $marker0 and with name=
  cat "$fname".tmp|grep "$marker0"|grep "name=" > orig_str_name_full.tmp
  # create file orig_str_name.tmp, which content all original for translation strings
  cat orig_str_name_full.tmp|sed "s|$marker0.*name=\"||g"|sed "s|\".*||g" > orig_str_name.tmp
}

function get_orig_str_Description {
  # create file orig_str_Description_full.tmp, which content all strings with marker $marker0
  cat "$fname".tmp|grep "$marker0" > orig_str_Description_full.tmp
  # create file orig_str_Description.tmp, which content all original for translation strings
  cat orig_str_Description_full.tmp|sed "s|$marker0>||g"|sed "s|</property>||g" > orig_str_Description.tmp
}

function put_tr_str {
  # translate $orig_str in $tr_str
  get_tr_str
  # we don't want big size of ready XML file, so we put there only translated strings
  if [ ! "$tr_str" = "$orig_str" ]
  # put translated string into XML file
  then
    if [ "$marker0" = "<title" ] || [ "$marker0" = "<description" ]
    then
      sed -i "s|$marker0>$orig_str$marker1|$marker0>$orig_str$marker1\n$marker0 lang=\"$language\">$tr_str$marker1|g" "$fname".tmp
    fi
    if [ "$marker1" = "</property>" ]
    then
      # create new marker2 from $marker0: we change symbol " in the end of line on _$language"
      marker2=`echo "$marker0"|sed "s|\"$|_$language\"|g"`
      sed -i "s|$marker0>$orig_str$marker1|$marker0>$orig_str$marker1\n$marker2>$tr_str$marker1|g" "$fname".tmp
    fi
  fi
}

function put_tr_str_name {
  # file orig_str_name.tmp can content more than one string for translation
  cat orig_str_name.tmp|while read orig_str
  do
    # translate $orig_str in $tr_str
    get_tr_str
    # we don't want big size of ready XML file, so we put there only translated strings
    if [ ! "$tr_str" = "$orig_str" ]
    then
      # find full string (which content $orig_str) from file orig_str_name_full.tmp
      cat orig_str_name_full.tmp|grep "\"$orig_str\""|while read full_str
      do
        # if we put $tr_str in file already, then we should do nothing again
        put_tr_str=`cat "$fname".tmp|grep "$tr_str"`
        if [ "$put_tr_str" = "" ]
        then
           # change full string on full string with EOL and add translated string
           sed -i "s|$full_str|$full_str\n<name lang=\"$language\">$tr_str</name>|g" "$fname".tmp
        fi
      done
    fi
  done
  # we used files orig_str_name.tmp, orig_str_name_full.tmp and should delete them
  rm -f orig_str_name.tmp
  rm -f orig_str_name_full.tmp
}

function put_tr_str_Description {
  # file orig_str_Description.tmp can content more than one string for translation
  cat orig_str_Description.tmp|while read orig_str
  do
    # translate $orig_str in $tr_str
    get_tr_str
    # we don't want big size of ready XML file, so we put there only translated strings
    if [ ! "$tr_str" = "$orig_str" ]
    then
      # find full string (which content $orig_str) from file orig_str_Description_full.tmp
      cat orig_str_Description_full.tmp|grep "$orig_str"|while read full_str
      do
        # if we put $tr_str in file already, then we should do nothing again
        put_tr_str=`cat "$fname".tmp|grep "$tr_str"`
        if [ "$put_tr_str" = "" ]
        then
           # change full string on full string with EOL and add translated string
           sed -i "s|$full_str|$full_str\n$marker0 lang=\"$language\">$tr_str$marker1|g" "$fname".tmp
        fi
      done
    fi
  done
  # we used files orig_str_Description.tmp, orig_str_Description_full.tmp and should delete them
  rm -f orig_str_Description.tmp
  rm -f orig_str_Description_full.tmp
}

function parsing {
  echo "Parsing..."
  # find all XML files except of levels.xml
  for fname in `find "$lev_dir"_ -name "*.xml"|grep -v "levels.xml"`
  do
    # we work with file "$fname".tmp instead of original "$fname"
    cp -f "$fname" "$fname".tmp
    # we use temporary pattern xxx for &lt;
    sed -i 's|&lt;|xxx|g' "$fname".tmp
    # we use temporary pattern yyy for &gt;
    sed -i 's|&gt;|yyy|g' "$fname".tmp
    # we use temporary pattern kkk for >http
    sed -i 's|>http|kkk|g' "$fname".tmp
    # we use temporary pattern uuu for ordinary symbol \
    sed -i 's|\\|uuu|g' "$fname".tmp
    # find all PO files and determine language from filename
    for language in `find i18n -name *.po|cut -d "_" --fields=3|cut -d "." --fields=1`
      do
        # write marker0 and marker1 and use functions for getting original string and putting translated string with according markers
        marker0="<title"
        marker1="</title>"
        get_orig_str
        put_tr_str

        marker0="<description"
        marker1="</description>"
        get_orig_str
        put_tr_str

        marker0="<property key=\"Description\""
        marker1="</property>"
        get_orig_str_Description
        put_tr_str_Description

        marker0="<property key=\"page1\""
        marker1="</property>"
        get_orig_str
        put_tr_str

        marker0="<property key=\"page2\""
        marker1="</property>"
        get_orig_str
        put_tr_str

        marker0="<property key=\"page3\""
        marker1="</property>"
        get_orig_str
        put_tr_str

        marker0="<property key=\"page4\""
        marker1="</property>"
        get_orig_str
        put_tr_str

        marker0="<property key=\"page5\""
        marker1="</property>"
        get_orig_str
        put_tr_str

        marker0="<property key=\"page6\""
        marker1="</property>"
        get_orig_str
        put_tr_str

        marker0="<property key=\"page7\""
        marker1="</property>"
        get_orig_str
        put_tr_str

        marker0="<toolboxitem"
        # we have no marker1 in this case, can be any
        marker1=""
        get_orig_str_name
        put_tr_str_name
      done
    # restore symbols instead of temporary patterns
    sed -i 's|xxx|\&lt;|g' "$fname".tmp
    sed -i 's|yyy|\&gt;|g' "$fname".tmp
    sed -i 's|uuu|\\|g' "$fname".tmp
    sed -i 's|kkk|>http|g' "$fname".tmp
    # move temporary "$fname".tmp into original "$fname"
    mv -f "$fname".tmp "$fname"
  done
  echo "Done"
}

function post_cleanup {
  echo "Post-cleanup..."
  # delete level directory
  rm -rf "$lev_dir"
  # copy temporary level directory into original level directory
  cp -af "$lev_dir"_ "$lev_dir"
  # delete temporary level directory
  rm -rf "$lev_dir"_
  # we used modified tmp files instead of original PO files
  rm -f i18n/*.tmp
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

function get_tr_str {
  cd i18n
  po_file="tbe_levels_$language.po"
  # prepare PO file only once
  if [ ! -f "$po_file".tmp ]
  then
    cp -f "$po_file" "$po_file".tmp
    # we don't need more than one space
    cat "$po_file".tmp|tr -s [:blank:] ' ' > "$po_file".tmp0
    # we don't need tabs in the start of string
    sed -i 's/^[ \t]*//' "$po_file".tmp0
    # we don't need tabs in the end of string
    sed -i 's/[ \t]*$//' "$po_file".tmp0
    # delete comments
    sed -i "/^#/d" "$po_file".tmp0
    # delete all EOL
    cat "$po_file".tmp0|tr -d '\n' > "$po_file".tmp
    # if string have symbols \" in the end, then use pattern vvv temporary, because we will delete symbols "" soon
    sed -i 's|\\""|vvv|g' "$po_file".tmp
    # restore all EOL
    sed -i 's|msgid|\nmsgid|g' "$po_file".tmp
    # delete symbols ""
    sed -i 's|""||g' "$po_file".tmp
    # return from vvv pattern
    sed -i 's|vvv|\\""|g' "$po_file".tmp
    # delete head
    sed -i 's|msgid msgstr||g' "$po_file".tmp
    # delete symbol \ before symbol "
    sed -i 's|\\"|"|g' "$po_file".tmp
    # we use temporary patterns xxx for <, yyy for >, uuu for double symbol \, kkk for >http,
    # so we change PO file and use modified PO file instead
    cat "$po_file".tmp|sed 's|<br>|xxxbryyy|g'|sed 's|</br>|xxx/bryyy|g'|sed 's|<i>|xxxiyyy|g'|sed 's|</i>|xxx/iyyy|g'|sed 's|\\|uuu|g'|sed 's|<a|xxxa|g'|sed 's|</a>|xxx/ayyy|g'|sed 's|>http|kkk|g' > "$po_file".tmp0
    cp -f "$po_file".tmp0 "$po_file".tmp
    rm -f "$po_file".tmp0
    if [ ! -f "$po_file".tmp ]
    then
      echo "Error: could not create file $po_file.tmp"
      exit 1
    fi
  fi
  # find string, which content $orig_str
  cat "$po_file".tmp|grep "msgid \"$orig_str\"" > "$po_file".tmp0
  # delete part of string, which content original string (between msgid and msgstr)
  sed -i 's|msgid.*msgstr||g' "$po_file".tmp0
  # delete start of string before symbol "
  sed -i 's|^ "||g' "$po_file".tmp0
  # delete end of string after symbol "
  sed -i 's|"$||g' "$po_file".tmp0
  # did we find tr_str?
  tr_str=`cat "$po_file".tmp0`
  # delete temporary file
  rm -f "$po_file".tmp0
  # if we did not find tr_str, then tr_str must be the same with $orig_str
  if [ "$tr_str" = "" ] || [ "$tr_str" = " " ]
  then
    tr_str=$orig_str
  fi
#  echo "orig_str for $language: $orig_str"
#  echo "tr_str for $language: $tr_str"
  cd ..
}

echo "Starting tbe_levels_i18n.sh script..."

lev_dir="levels"

# if this script is located in i18n directory, then we should go to root directory
cd ..

# if script was run without parameters
if [ "$1" = "" ]
then
  check_dependencies
  prepare_for_parsing
  parsing
  post_cleanup
fi

# if script was run with parameter "pot", then create pot file only
if [ "$1" = "pot" ]
then
  check_dependencies
  # copy level directory into temporary original level directory
  cp -af "$lev_dir" "$lev_dir"_orig
  prepare_for_parsing
  create_pot_file
  post_cleanup
  # delete level directory
  rm -rf "$lev_dir"
  # copy original level directory into level directory
  cp -af "$lev_dir"_orig "$lev_dir"
  # delete temporary original level directory
  rm -rf "$lev_dir"_orig
fi

echo "Script tbe_levels_i18n.sh was finished"
