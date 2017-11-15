#!/bin/sh 

if(($# < 1));then
    echo "usage $0 in_dir "
    echo "例如:  ./run.sh dir "
    exit 0;
fi

dir=$1
tmp_awk=tmp.awk
tmp_pro=tmp.pro

rm -rf full && mkdir -p full

ls -1 $dir | while read line 
do
    echo $line
    dos2unix ${dir}/${line}


    python pro_text.py  ${dir}/${line}  ${tmp_awk} 
    python getProcessedFormat.py  ${tmp_awk}  ${tmp_pro}

    ./Ttslabel ${tmp_pro} 

done

rm -rf $tmp_pro  $tmp_awk



#### 检测所有lab  是否有格式错误
### IY^DH-ER+W=EY@2_0/A:1_4_0/B:4-1-2@1-2&2-2#2-2
### $9-2!3-3;10-1|2/C:1+2+1/D:7_2/E:2+7@1+1&1+3#1+3/F:1_2/G:2

grep "^[\^]" full/* 
grep "[\^][-]" full/* 
grep "[-][+]" full/* 
grep "[+][=]" full/* 
grep "[=][@]" full/* 



