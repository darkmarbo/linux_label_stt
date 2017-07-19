#!/bin/sh

## pro_qieyin_v0.py:
##      标准处理流程
## pro_qieyin_puqiang.py:
##      针对普强的数据 修改的处理流程
##      处理8387句的结果 是一样的 所以可以直接使用 _puqiang的版本 
## interval_pro.bak.ok:
##      处理结果备份 
## interval: 测试集
##      /home/szm/tts/data_all/auto_tag/data_57_8000/interval 中的 自动切音来的 




if(($#<1))
then
	echo "usage: $0 input_dir "
	echo "重命名时  按照序号 000001.lab 格式"
	exit 0
fi

dir=${1}
dir_pro=${dir}_pro
dir_rename=${dir_pro}_rename

## interval 格式处理  
rm -rf ${dir_pro} && mkdir ${dir_pro}
rm -rf err_list
ls -1 ${dir} | while read line
do
    #echo "${dir}/$line -----> ${dir_pro}/$line"
	python pro_qieyin.py ${dir}/$line ${dir_pro}/$line >> err_list
done
cat err_list | awk -F":" '{print $2}' |sort |uniq > phone_err_list
rm -rf err_list

### 判断 md5sum 是否一致 
cat ${dir_pro}/* |awk '{print $3}'|sort|uniq > ${dir_pro}.yinsu
md5sum mono.list
md5sum ${dir_pro}.yinsu

md5sum ${dir_pro}/* | awk '{print $1}' > pro.md5sum.new 
md5sum interval_pro.bak.ok/* | awk '{print $1}' > pro.md5sum.ok 
md5sum pro.md5sum.new  pro.md5sum.ok 
rm -rf  pro.md5sum.new  pro.md5sum.ok 

#### 将interval 重命名成 000001.lab 
rm -rf ${dir_rename} && mkdir -p ${dir_rename}

ii=000001
ls -1 ${dir_pro} |sort -n | while read file
do
    tmp=`printf %06d $ii`
    new_name=${tmp}.lab
    #echo "$file --> $new_name"
    cp ${dir_pro}/$file  ${dir_rename}/${new_name}

    ii=`expr $ii + 1`
done

echo "interval 处理完毕！"



