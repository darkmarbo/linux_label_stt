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
    #print (i);

    ### 000001 瓜农|要 生存$市民 有需求
    #010001	밤이 되자$황야는|더욱더 두렵게 보인다$
    ln1 = lines[i*2].decode('utf-8');
    list_ln1 = ln1.split("\t");
    if len(list_ln1)<2:
        print("ERROR:format err: %s"%(ln1));
        sys.exit(0);
    id = list_ln1[0];
    ln1 = list_ln1[1];

    ### gua1 nong2 yao4 sheng1 cun2 shi4 min2 you3 xv1 qiu2
    #	ba mi | doe ja | hwang ya neun | deo uk tteo | du ryeop kke | bo in da
    ln2 = lines[i*2+1].decode('utf-8');
    list_ln2 = ln2.split("\t");
    if len(list_ln2)<2:
        print("ERROR:format err: %s"%(ln2));
        sys.exit(0);
    ln2 = list_ln2[1];
	
    ln2 = ln2.replace('\n','');
    ln2 = ln2.replace(' | ',' ');
    ln2 = ln2.replace(' |',' ');
    ln2 = ln2.replace('| ',' ');
    ln2 = ln2.replace('|',' ');
    #ln2,num = re.subn('([^e])r([123456])','\g<1>\g<2> er5',ln2);
    #ln2,num = re.subn('([qwertyuioplkjhgfdsazxcvbnm])er([123456])','\g<1>e\g<2> er5',ln2);
    #ln2,num = re.subn('6','2',ln2);
    #ln2,num = re.subn('([yjqx])v','\g<1>u',ln2);
    fp.write(id+'\t'+ln2+'\n');
	
    ln1=ln1.replace('  \n','\n');
    ln1=ln1.replace(' \n','\n');
    ln1=ln1.replace('\n','');
    ln1=ln1.replace(' ','1');
    ln1=ln1.replace('|','2');
    ln1=ln1.replace('$','3');
    ln1=ln1+'4';
    ln3='';

    for j in range(len(ln1)):
        if ln1[j]=='1' or ln1[j]=='2' or ln1[j]=='3' or ln1[j]=='4':
            ln3 = ln3+ln1[j];
            continue;
        elif j<len(ln1)-1:
            if ln1[j+1]=='1' or ln1[j+1]=='2' or ln1[j+1]=='3' or ln1[j+1]=='4':
                continue;
            else:
                ln3=ln3+'0';
                continue;
    fp.write(ln3.encode('utf-8') + '\n');

fp.close();
    

    
##    lp1=ln2.split('$');
##    for lt1 in lp2:
##        lp2=lt1.split('|');
##        for lt2 in lp2:
##            lp3=lt2.split();
##            for lt3 in lp3
