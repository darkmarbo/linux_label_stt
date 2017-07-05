#!/bin/sh -x


############ 去标点 
#awk -F"\t" '{if(NR%2==1){print $2}}' yunlv.txt |\
#	sed 's/，//g;s/。//g;s/！//g;s/（//g;s/）//g;s/——//g;s/、//g;s/【//g;s/】//g;s/”//g;s/“//g;s/‘//g;s/’//g;s/：//g;s/；//g;s/？//g;s/、//g;s/》//g;s/《//g;s/……//g;s/-//g'|\
#	sed 's/#4//g;s/#1/ /g;s/#2/|/g;s/#3/$/g'\
#> yunlv.txt.awk
#
######### 为hts中label做准备
#awk -F"\t" '{print $2}' yunlv.txt |\
#	sed 's/，//g;s/。//g;s/！//g;s/（//g;s/）//g;s/——//g;s/、//g;s/【//g;s/】//g;s/”//g;s/“//g;s/‘//g;s/’//g;s/：//g;s/；//g;s/？//g;s/、//g;s/》//g;s/《//g;s/……//g;s/-//g'|\
#	sed 's/#4//g;s/#1/ /g;s/#2/|/g;s/#3/$/g'\
#> yunlv.txt.for-hts-label.awk
#
#awk -F"\t" '{print $2}' cixing.txt|\
#	sed 's/，//g;s/。//g;s/！//g;s/（//g;s/）//g;s/——//g;s/、//g;s/【//g;s/】//g;s/”//g;s/“//g;s/‘//g;s/’//g;s/：//g;s/；//g;s/？//g;s/、//g;s/》//g;s/《//g;s/-//g;s/……//g'\
#> cixing.txt.awk

#python3.2 step1_del_err.py cixing.txt.awk yunlv.txt.awk
#python3.2 step2_pw_txt.py 
#python3.2 step3_pp_txt.py
#python3.2 step4_ip_txt.py



#### 其他测试使用
######## 为hts中label做准备
awk -F"\t" '{print $2}'   text_all.txt   |\
	sed 's/，//g;s/。//g;s/！//g;s/（//g;s/）//g;s/——//g;s/、//g;s/【//g;s/】//g;s/”//g;s/“//g;s/‘//g;s/’//g;s/：//g;s/；//g;s/？//g;s/、//g;s/》//g;s/《//g;s/……//g;s/-//g'|\
	sed 's/#4//g;s/#1/ /g;s/#2/|/g;s/#3/$/g'\
> text_all.txt.for-hts-label.awk
