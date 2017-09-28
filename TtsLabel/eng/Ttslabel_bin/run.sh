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



