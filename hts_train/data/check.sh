#!/bin/sh

cat $1|while read line
do
	name=${line}.cmp
	if [ "cmp/$name" ]
	then
		cp cmp/$name cmp.new/
		echo "$name"
	fi
done
