#!/bin/sh 

if(($# < 1));then
    echo "usage $0 in_file "
    exit 0;
fi

rm -rf full && mkdir -p full
in_file=$1

dos2unix ${in_file}

./Ttslabel ${in_file}  

## 测试是否一致 
md5sum full/* |awk '{print $1}' > ttt
md5sum full.bak/* |awk '{print $1}' > mmm

md5sum  ttt   mmm


