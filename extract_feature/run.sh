#!/bin/sh +x 

rm -rf cmp && mkdir -p cmp

### 处理整个目录 
#matlab -c license_405329_R2014a.lic  -nodisplay -nodesktop -nosplash -r Run   

### 单独测试 一个文件的处理 
## matlab -c license_405329_R2014a.lic  -nodisplay -nodesktop -nosplash -r "ddd='000002';run_single"

###### 处理wave下的所有wav  对于wave/000001.wav 输入为：000001
ls -1 wave |while read ttt
do
    name=${ttt%.wav}
    echo $name

    while true
    do
        num=`ps aux|grep MATLAB |grep nosplash|wc -l`
        if(($num < 20));then
            matlab -c license_405329_R2014a.lic  -nodisplay -nodesktop \
                -nosplash -r "ddd='${name}';run_single"  &
            break;
        else
            sleep 5
        fi
    done

#matlab -c license_405329_R2014a.lic  -nodisplay -nodesktop \
#        -nosplash -r "ddd='${name}';run_single"  &

done

sleep 60

echo "matlab extract successed!"



