#!/bin/sh 

if(($# < 1));then
    echo "usage $0 in_file "
    echo "例如:  ./run.sh text.txt "
    exit 0;
fi

in_file=$1

rm -rf full && mkdir -p full

dos2unix ${in_file}


#python pro_text.py  $in_file  ${in_file}.awk

#python getProcessedFormat.py  ${in_file}.awk  ${in_file}.pro

./Ttslabel ${in_file}.pro 









### 测试是否一致 
#md5sum full/* |awk '{print $1}' > ttt
#md5sum full.bak/*|sort -n -t/ -k2 |awk '{print $1}' > mmm
#
#md5sum  ttt   mmm

#### 检测lable序列是否符合规则 
###  nu^sil-b+a=m@1_0/A:0_0_0/B:0-0-1@2-1&4-1#4-1$16-2!4-4;16-1|2/C:1+2+1/D:6_2/E:2+6@1+1&1+3#1+
###    3/F:1_2/G:2
#echo "检测所有full/* 是否符合规则 "
#
#grep -v '\w.*\^\w.*-\w.*+\w.*=\w.*@\w.*_\w.*\/A:\w.*_\w.*_\w.*\/B:\w.*-\w.*-\w.*@\w.*-\w.*&\w.*-\w.*#\w.*-\w.*\$\w.*-\w.*!\w.*-\w.*;\w.*-\w.*\|\w.*\/C:\w.*+\w.*+\w.*\/D:\w.*_\w.*\/E:\w.*+\w.*@\w.*+\w.*&\w.*+\w.*#\w.*+\w.*\/F:\w.*_\w.*\/G:\w.*$' full/*
#
#


