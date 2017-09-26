#coding=utf-8
import sys
import os
import string
import re

if len(sys.argv)<3:
    print("usage: %s in_file out_file"%(sys.argv[0]));
    sys.exit(0);

fp = open(sys.argv[1],'r');
lines = fp.readlines();
fp.close();

fp = open(sys.argv[2],'w');
num_line = int(len(lines)/2);

for i in range(num_line):

    #010001	밤이 되자$황야는|더욱더 두렵게 보인다$
    #	ba mi | doe ja | hwang ya neun | deo uk tteo | du ryeop kke | bo in da

    #00001   Either way$you should shoot|very slowly
    #  IY1 . DH ER0 / W EY1 / Y UW1 / SH UH1 D / SH UW1 T3 / V EH1 . R IY0 / S L OW1 . L IY0

    ln1 = lines[i*2].decode('utf-8');
    list_ln1 = ln1.split("\t");
    if len(list_ln1)<2:
        print("ERROR:format err: %s"%(ln1));
        sys.exit(0);
    id = list_ln1[0];
    ln1 = list_ln1[1];

    ln1=ln1.replace('  \n','\n');
    ln1=ln1.replace(' \n','\n');
    ln1=ln1.replace('\n','');
    ln1=ln1.replace('   ',' ');
    ln1=ln1.replace('  ',' ');


    ### 记录 韵律词 短语 语调短语的123
    prs=[];
    for cc in ln1:
        if cc == ' ':
            prs.append('1');
        elif cc == '|':
            prs.append('2');
        elif cc == '$':
            prs.append('3');

    prs.append('4');
    #print(prs);


    ln2 = lines[i*2+1].decode('utf-8');
    list_ln2 = ln2.split("\t");
    if len(list_ln2)<2:
        print("ERROR:format err: %s"%(ln2));
        sys.exit(0);
    ln2 = list_ln2[1];
	
    ln2 = ln2.replace('\n','');
    vec_syl=[];
    ## 按照/划分  得到韵律词 看看跟 prs 对应否 
    list_ln2=ln2.split("/");
    if len(prs) != len(list_ln2):
        print("ERROR prs ", prs);
        print("ERROR ln2 ", list_ln2);
        continue;

    ## 010001	ba mi doe ja hwang ya neun deo uk tteo du ryeop kke bo in da
    ## 0103002001001004

    ### 输出第一行  syllable 
    fp.write(id+'\t');
    for ii in range(len(list_ln2)):
        ## 每个word  :  IY1 . DH ER0
        word = list_ln2[ii];
        vec_syl = word.split(".");
        for kk in range(0, len(vec_syl)):
            ## 每个音节: IY1 和 DH ER0
            syl = vec_syl[kk]
            syl=syl.strip();
            syl=re.sub('  ', ' ', syl);
            syl=re.sub(' ', '_', syl);

            if ii == len(list_ln2)-1 and kk == len(vec_syl)-1 :
                fp.write(syl);
            else:
                fp.write(syl+' ');


    fp.write('\n');


    ### 输出第2行  01234 
    for ii in range(len(list_ln2)):
        ## 每个word  :  IY1 . DH ER0
        word = list_ln2[ii];
        vec_syl = word.split(".");
        for jj in range(len(vec_syl)-1):
            ## 每个音节: IY1 和 DH ER0
            fp.write('0');


        fp.write(prs[ii]);


    fp.write('\n');


fp.close();
    

    
