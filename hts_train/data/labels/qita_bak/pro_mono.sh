#!/bin/sh

#rm -rf mono/*
#cp mono.lu.4000/1* mono/
#cp mono.lu.4000/2* mono/
#cp mono.lu.4000/3* mono/

python pro_mono.py mono

rm -rf mono/*.lab

ls -1 mono|while read line
do
	name=${line%.new}
	echo $line"------>"$name
	#interval
	mv mono/$line mono/$name
done

cat mono/* |awk '{print $3}' |sort|uniq > mono.phone.list

#ii=0
#ls -1 $1|while read line
#do
#	ii=$((ii+1))
#	echo $line"------>"$ii.lab
#	#interval
#	cp $1/$line $1/$ii.lab
#done
