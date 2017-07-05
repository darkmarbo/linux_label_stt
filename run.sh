#!/bin/sh -x

if(($# < 1));then
    echo "usage $0 in_file "
    exit 0;
fi

rm -rf full && mkdir -p full
in_file=$1
num=`wc -l ${in_file} |awk '{print $1}'`
num=`expr $num / 2`

dos2unix ${in_file}

./Ttslabel ${in_file}  ${num}

## 测试是否一致 
md5sum full/* |awk '{print $1}' > ttt
md5sum full.bak/* |awk '{print $1}' > mmm

md5sum  ttt   mmm


