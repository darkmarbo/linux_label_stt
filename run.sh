#!/bin/sh 

if(($# < 1));then
    echo "usage $0 in_file "
    echo "例如:  ./run.sh text.txt "
    exit 0;
fi

in_file=$1

rm -rf full && mkdir -p full

dos2unix ${in_file}


######## 为hts中label做准备
awk -F"\t" '{print $2}'   ${in_file}   |\
	sed 's/，//g;s/。//g;s/！//g;s/（//g;s/）//g;s/——//g;s/、//g;s/【//g;s/】//g;s/”//g;s/“//g;s/‘//g;s/’//g;s/：//g;s/；//g;s/？//g;s/、//g;s/》//g;s/《//g;s/……//g;s/-//g'|\
	sed 's/#4//g;s/#1/ /g;s/#2/|/g;s/#3/$/g'\
>  ${in_file}.awk

python getProcessedFormat.py  ${in_file}.awk  ${in_file}.pro

./Ttslabel ${in_file}.pro 

## 测试是否一致 
md5sum full/* |awk '{print $1}' > ttt
md5sum full.bak/* |awk '{print $1}' > mmm

md5sum  ttt   mmm


