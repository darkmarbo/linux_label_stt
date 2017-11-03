#coding=utf-8


import os
import sys
import re


punc=['，', ',', '\.', ';', '。', '！', '（', '）', '【', '】', '”', '“', '"', '‘', '’', '：', ':', '；', '？', '\?', '、', '》', '《', '……' ]

if len(sys.argv)<3:
    print("usage: %s in_file out_file"%(sys.argv[0]));
    sys.exit(0);



fp_in=open(sys.argv[1]);
fp_out=open(sys.argv[2], "w");

vec_line=fp_in.readlines();

n_line=len(vec_line)/2;
for ii in range(0, n_line):
    #00001	Either way%, you should shoot/very slowly%.
    # /是韵律短语划分  %是语调短语划分 
    line1 = vec_line[2*ii];

    #	IY1 . DH ER0 / W EY1 / Y UW1 / SH UH1 D / SH UW1 T3 / V EH1 . R IY0 / S L OW1 . L IY0
    # .是音节划分   /是word划分  
    line2 = vec_line[2*ii+1];


    ### 删除掉第一行中的标点 
    for cc in punc:
        line1=re.sub(cc, ' ', line1);

    ### /转|   %转$   删除$后面的空格  删除行尾的$
    line1=re.sub('/', '|', line1);
    line1=re.sub('%', '$', line1);

    line1=re.sub('\|  ', '|', line1);
    line1=re.sub('\| ', '|', line1);

    line1=re.sub('\$  ', '$', line1);
    line1=re.sub('\$ ', '$', line1);
    line1=re.sub('\$$', '', line1);

    fp_out.write("%s"%line1);
    fp_out.write("%s"%line2);




fp_in.close();
fp_out.close();
