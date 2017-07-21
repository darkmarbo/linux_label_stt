#!/bin/sh 

if(($#<2));then
    echo "usage: $0 file_txt  dir_wav "
    echo "file_txt 格式: id_num+TAB+content"
    echo "dir_wav 格式: id_num.wav 与文本id_num完全对应的"
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

file_tn=${file}.ok.tn_res.txt
file_pros_zip=${file_tn}.zh-pros_res.zip
file_pros=${file}_pros.txt
file_pinyin=${file}_pinyin.txt

ip_A="root@10.10.10.151"
dir_A=`pwd`

ip_19="yanfa@10.10.10.19"
dir_19="/home/yanfa/yanqiwei/proj/ProsSP_Final/ProsSP_Script"

ip_27="root@10.10.10.27"
dir_27="/home/szm/ssh"
cmd_tn="/home/yanfa/yanqiwei/proj/TN_Tools/TN_Toolkit_CPP/TN_Tools.sh"
cmd_prosody="/home/yanfa/yanqiwei/proj/ProsSP_Final/ProsSP_Script/zh_pros_predict.sh"

ip_192="root@10.10.10.192"
dir_192="/home/temp"
cmd_pinyin="perl  /home/yanfa/yfhao/mars/dict/util/make_raw_ph.pl "

ip_211="forcealignment@10.10.10.211"
dir_211="/home/forcealignment/zh-cn-common"
#### 内部包含 project/tts_8000/wav/1/  

### 带id 的txt 预处理
mv -f ${file}  ${file}.bak
awk -F"\t" '{print $1}' ${file}.bak  > ${file}.id
awk -F"\t" '{print $2}' ${file}.bak  > ${file}
size_id=`wc -c ${file}.id | awk '{print $1}'`
size_txt=`wc -c ${file} | awk '{print $1}'`
if((${size_txt}<${size_id}));then
    echo "format err:${file}"
    rm -rf lock;
    exit 1
fi

#################################  1   简体中文-TN  序号55 
### 把目标机器上的 临时目录清空 

scp -P 22 ${dir_A}/${file}  ${ip_27}:${dir_27} 

ssh  -p 22 ${ip_27}  "

    cd ${dir_27};
    ${cmd_tn} ${dir_27}/${file} zh_cn  zh_cn ;

"

rm -rf ${dir_A}/${file_tn}
scp -P 22 ${ip_27}:${dir_27}/${file_tn}  ${dir_A}/ 
if [ -f ${dir_A}/${file_tn} ];then
    echo "${ip_27} TN successed! "
else
    echo "${ip_27} 文本正则化出错 请联系管理员  "
    rm -rf lock;
    exit
fi
mv -f ${file}.bak  ${file}

#################################   2    简体中文-韵律标注  序号55 
### 把目标机器上的 临时目录清空 
## 上传 file_tn 到   ip_27的dir_27目录下 
scp -P 22 ${dir_A}/${file_tn}  ${ip_27}:${dir_27} 

ssh  -p 22 ${ip_27}  "

    cd ${dir_27};
    ${cmd_prosody}  ${dir_27}/${file_tn}  no  no;  

"

### 下载结果 解压  判断结果是否存在  
scp -P 22 ${ip_27}:${dir_27}/${file_pros_zip}  ${dir_A}/ 
rm -rf tmp && mkdir -p tmp
unzip ${file_pros_zip}  -d tmp
mv -f tmp/${file_tn}  ${file_pros} 
rm -rf ${file_pros_zip}

if [ -f ${dir_A}/${file_pros} ];then
    echo "${ip_27} prosody successed! "
else
    echo "${ip_27} 韵律预测出错 请联系管理员 "
    rm -rf lock;
    exit
fi


#################################    3   发音预测   序号17  
scp -P 22 ${dir_A}/${file_pros}  ${ip_192}:${dir_192} 

ssh  -p 22 ${ip_192}  "

    cd ${dir_192};
    ${cmd_pinyin} -i ${file_pros}  -o ${file_pinyin} -l zh-cn_pinyin  -m lingua -w 1

"
scp -P 22 ${ip_192}:${dir_192}/${file}_pinyin*  ${dir_A}/ 
if [ -f ${dir_A}/${file_pinyin} ];then
    echo "${ip_192} pinyin successed! "
else
    echo "${ip_192} 注音出错 请联系管理员"
    rm -rf lock;
    exit
fi

### 将 id 拼回来 
python merge_id.py  ${file_pinyin}  ${file}.id  ${file_pinyin}.merge
mv -f ${file_pinyin}.merge   ${file_pinyin}


#################################   4  拆分音素  自动对齐 
python py2phone.py ${file_pinyin}  ${file}_yinsu.txt  

dd=`date +%H%M%S`
name_211="${dir_211}/project/${dd}"   ### 211 上的目录名字 
script_211="${name_211}/script"
wav_211="${name_211}/wav"
result=${name_211}/interval

rm -rf hehe_utf8.txt && cp ${file}_yinsu.txt   hehe_utf8.txt

ssh  -p 22 ${ip_211}  "mkdir  -p ${script_211}; mkdir -p ${wav_211};"

scp -P 22 -r ${dir_wav}      ${ip_211}:${wav_211} 
scp -P 22 -r ${dir_A}/hehe_utf8.txt  ${ip_211}:${script_211} 
ssh  -p 22 ${ip_211}  "
    cd ${dir_211}; 
    iconv -f utf-8 -t utf-16 ${script_211}/hehe_utf8.txt  > ${script_211}/hehe.txt;
    rm -rf ${script_211}/hehe_utf8.txt
    mv  ${wav_211}/${dir_wav}   ${wav_211}/hehe
"
echo "发送 wav和script 完毕！"

cmd_qieyin="force_alignment.py ${dd} project/${dd}/script  project/${dd}/wav  "
ssh  -p 22 ${ip_211}  "
    cd ${dir_211};
    export PATH="/usr/java/jdk1.8.0_60/bin:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin:/home/forcealignment/bin";
    echo ${cmd_qieyin};
    python  ${cmd_qieyin} ;
"
## python test.py;

rm -rf interval && scp -r ${ip_211}:${result}  ./
#md5sum interval/hehe/* | awk '{print $1}' > ttt




rm -rf lock;

echo "front_pro 执行完毕!"






