#!/bin/sh

#ii=0
#ls -1 $1|while read line
#do
#	name=${line%.lab}
#	echo $line"------>"$name.cmp
#	cp $1/$line $1/$name.cmp
#done

ii=0
ls -1 $1|while read line
do
	ii=$((ii+1))
	echo $line"------>"$ii.cmp
	cp $1/$line $1/$ii.cmp
done
