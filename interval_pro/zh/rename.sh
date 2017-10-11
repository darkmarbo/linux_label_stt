#!/bin/sh


#####  从标注文件中提取出原始文本
##awk -F"\t" '{if(NR%2==1){print $2}}'  000001-008397.txt  \
##        | sed 's/#[1234]//g' > 000001-008397.txt.sentence 
#
#

##### 将原始wav 重命名成顺序的 
##dir_2=${1}_rename
##mkdir -p $dir_2
##
##ii=000001
##ls -1 $1 |while read file
##do
##    tmp=`printf %06d $ii`
##    new_name=${tmp}.wav
##    echo "$file --> $new_name"
##    cp $1/$file  ${dir_2}/${new_name}
##
##    ii=`expr $ii + 1`
##done

#### 将原始full 重命名成 000001.lab 
dir_2=${1}_rename
rm -rf ${dir_2} && mkdir -p $dir_2

ii=000001
ls -1 $1 |sort -n | while read file
do
    tmp=`printf %06d $ii`
    new_name=${tmp}.lab
    echo "$file --> $new_name"
    cp $1/$file  ${dir_2}/${new_name}

    ii=`expr $ii + 1`
done


##### 重命名000001.wav 为 000001.raw
#rm -rf $2 && mkdir $2
#ls -1 $1 | sort -n|while read line
#do
#	name=${line%.wav}
#
#	new_name=${name}.raw
#
#    sox $1/${line}  $2/${new_name}
#
#	echo $line"   ------>   "$new_name
#
#done


#### 重命名 1.lab 为 rs_rul_100001.lab 
#rm -rf $2 && mkdir $2
#ii=100000
#ls -1 $1|sort -n|while read line
#do
#	ii=$((ii+1))
#	new_name="rs_rjl_"${ii}.lab
#	echo $line"   ------>   "$new_name
#	cp $1/$line $2/$new_name
#done

##### 重命名 000001.interval 为 rs_rjl_100001.lab 
#rm -rf $2 && mkdir $2
###  000089.interval
###  rs_rjl_100193.lab
#ii=100000
#ls -1 $1|sort -n|while read line
#do
#	ii=$((ii+1))
#	new_name="rs_rjl_"${ii}.lab
#	echo $line"   ------>   "$new_name
#	cp $1/$line $2/$new_name
#done

#ii=0
#ls -1 $1|sort -n|while read line
#do
#	ii=$((ii+1))
#	name=${ii}.wav
#	echo $line"   ------>   "$name
#	sox $1/$line $2/${name} remix 2
#	new_name="rs_ccc_"${ii}.raw
#
#	echo $name"   ------>   "$new_name
#	sox $2/${name}  $2/${new_name}
#done



#cat $1|while read line
#do
#	cp mono.rename/$line   mono.rename.500/
#done



#### 修改语音名字  跟 pinyin文本对应上 提供给晓婷 
#rm -rf $2 && mkdir $2
#ls -1 $1|sort -n|while read line
#do
#	name=${line#100}
#    name="000"${name}
#    echo ${name}
#	sox $1/${line} -r 16000 $2/${name}
#done




