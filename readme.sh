

当前目录下运行 ./make_tools.sh  会生成pipeline 环境 


===============================================================
需要配置的环境:
    配置相关机器 19 192 27 211 的等效性连接
    确定interval文件的格式 是否能覆盖
    安装 matlab 配置环境变量  修改run_single.m
    安装hts训练相关工具   修改相关训练配置 


===============================================================
front_pro:  interval
    原始文本 000.txt 经过TN 分词 韵律 注音 音素拆分  切音 
    得到: 000.txt_pinyin.txt  和 interval 

TtsLabel:   label
    输入 000.txt_pinyin.txt ,得到 label 序列 


extract_feature:    cmp
    输入wav目录  使用matlab 并行提取特征 

interval_pro:
    interval 后处理 


===============================================================

