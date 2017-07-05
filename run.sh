#!/bin/sh 

if(($# < 1));then
    echo "usage $0 in_file "
    exit 0;
fi

in_file=$1

rm -rf full && mkdir -p full

dos2unix ${in_file}

python getProcessedFormat.py  ${in_file}  ${in_file}.pro

./Ttslabel ${in_file}.pro 

## 测试是否一致 
md5sum full/* |awk '{print $1}' > ttt
md5sum full.bak/* |awk '{print $1}' > mmm

md5sum  ttt   mmm


