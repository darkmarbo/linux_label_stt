#!/bin/sh 

if(($#<2));then
    echo "usage: $0 file_txt  dir_wav "
    echo "其中，file_txt 为UTF8格式的原始文本 一行为一句话 与wav一一对应"
    echo "dir_wav 为wav语音所在的目录名字 内部全是.wav的语音"
    exit 
fi

while(true)
do
    if [ -f lock ];then
        echo "当前流程有人使用，等待即可!  date:`date +%H:%M:%S`"
        sleep 1;
    else
        touch lock;
        echo "当前流程无人使用，我们现在开始......"
        break;
    fi
done



################################# 0 变量定义 
file=$1
dir_wav=$2
file_py=${file}_pinyin.txt

### 子程序目录 
dir_front=front_pro
dir_int=interval_pro
dir_label=Ttslabel_bin
dir_cmp=extract_feature
dir_hts=hts_train
dir_pwd=`pwd`
### 生成结果分别为   full  interval/mono  cmp 


if [ ! -f ${file} -o ! -d ${dir_wav} ];then
    echo "输入文件:$file 或者 ${dir_wav} 不存在!"
    rm -rf lock;
    exit
fi




################################# 1 前端处理 + 切音  
### 结果: interval  ${file_py}
### 需要配置 19  192  27  211 的ssh等效性连接 
rm -rf ${dir_front}/${file}   ${dir_front}/${dir_wav}
mv ${file} ${dir_wav}   ${dir_front}
rm -rf interval  ${file_py}

cd ${dir_front}
    ./run.sh  ${file} ${dir_wav}
    mv interval/hehe ../interval
    mv  ${file}   ${dir_wav}  ../
    mv   ${file_py}  ../
cd -

################################## 2 interval 处理 重命名
### 结果: mono
rm -rf ${dir_int}/interval
mv interval  ${dir_int}
rm -rf mono

cd ${dir_int}
   ./run.sh interval 
   mv  interval_pro_rename  ../mono
   mv interval   ../
cd -

################################  3  label 序列生成 
### 结果: full
rm -rf ${dir_label}/${file_py}
mv ${file_py}  ${dir_label}
rm -rf full

cd ${dir_label}
    ./run.sh   ${file_py} 
    mv full ../
    mv ${file_py}  ../
cd -


################################   4  提取特征  
rm -rf ${dir_cmp}/wave  ${dir_cmp}/cmp
mv  ${dir_wav}   ${dir_cmp}/wave 
rm -rf cmp

cd ${dir_cmp}
    ./run.sh 
    mv cmp ../
    mv wave ../${dir_wav}

cd -

#################################     5   训练 hts
ls -1 cmp | awk -F"." '{print $1}' > tmp111
ls -1 full | awk -F"." '{print $1}' > tmp222
ls -1 mono | awk -F"." '{print $1}' > tmp333
md5sum    tmp111  tmp222  tmp333
wc -l  tmp111  tmp222  tmp333
rm -rf     tmp111  tmp222  tmp333

#### 拷贝数据 
rm -rf ${dir_hts}/data/cmp  && cp -r cmp ${dir_hts}/data/
rm -rf ${dir_hts}/data/labels/full  && cp -r full ${dir_hts}/data/labels/full
rm -rf ${dir_hts}/data/labels/mono  && cp -r mono ${dir_hts}/data/labels/mono

### 修改 data/Makefile 
path_mod=path_mod
path_hts="${dir_pwd}/${dir_hts}"
rm -rf ${dir_hts}/data/Makefile && cp ${dir_hts}/data/Makefile.ori  ${dir_hts}/data/Makefile
sed -i "s#$path_mod#$path_hts#g"  ${dir_hts}/data/Makefile

### scripts/Config.pm
rm -rf ${dir_hts}/scripts/Config.pm  && cp ${dir_hts}/scripts/Config.pm.ori  ${dir_hts}/scripts/Config.pm
sed -i "s#$path_mod#$path_hts#g"  ${dir_hts}/scripts/Config.pm

###  data/labels/gen
rm -rf ${dir_hts}/data/labels/gen/*
ls -1  ${dir_hts}/data/labels/full/* | head -10 |while read file
do
    cp ${file}  ${dir_hts}/data/labels/gen/
done

### 运行  data 下 make labels
cd ${dir_hts}/data && make labels && cd - 

cd ${dir_hts}
    perl scripts/Training.pl  scripts/Config.pm
cd -



### 清理中间结果 
rm -rf interval  ${file_py} 
rm -rf lock;

echo "${0}  执行完毕!"






