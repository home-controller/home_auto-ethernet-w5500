#!/bin/bash

d_dir=/home/jmnc2/Documents/programming/Arduino/home_auto/ethernet_relay/
f_cpp=$d_dir'*.cpp'
f_h=$d_dir'*.h'

LOCAL_DIR=/home/jmnc2/Documents/PlatformIO/Projects/relay_controll/src/
#delete broken links.
find $LOCAL_DIR -xtype l -delete

for f in $f_cpp $f_h $d_dir'LICENSE' $d_dir'README.md'
do
#  echo "Processing $f file..."
  fl=$(basename "$f")
  if [[ -f "$fl" ]]; then
	if [[ ! -L "$fl" ]]; then
  		echo "Local version of file not a simbolic link: $fl. Deleting..."
  		rm $LOCAL_DIR$fl
  	fi
  fi
  if [[ ! -f "$fl" ]]; then
    c="ln -s '$f' '$LOCAL_DIR$fl'"
    echo "Creating link for: $fl"
    echo $c
    eval $c
  else
    echo "File already in local dir. $fl"  
  fi

  # take action on each file. $f store current file name
#  cat $f
#  echo $f
done