#coding=utf-8
import sys
import os
import string
import re

if len(sys.argv) < 2:
    print "usage :%s input_file output_file"%(sys.argv[0])
    sys.exit()

fp_in=open(sys.argv[1], 'r')
fp_out=open(sys.argv[2],'w')

shengyunmu = ('AA' ,'AE' ,'AH' ,'AO','AW' ,'AY' ,'B' ,'CH' ,'D' ,'DH' ,'EH' ,'ER' ,'EY' ,'F' ,'G' ,'HH' ,'IH' ,'IY' ,'JH' ,'K' ,'L' ,'M' ,'N' ,'NG' ,'OW' ,'OY' ,'P' ,'R' ,'S' ,'SH' ,'T' ,'TH' ,'UH' ,'UW' ,'V' ,'W' ,'Y' ,'Z' ,'ZH', 'sp', 'sil');


lines=fp_in.readlines()
time_st=0
time_end=0

##print("%d"%(len(shengyunmu)));

for ii in range(len(lines)):

    if(ii<12):
        continue

    line=lines[ii]
    line=line[:-1]
    
    #2.909926777929853
    #3.0880553409073177
    #"uang2"

    if(ii%3 == 0):
        time_st = int(float(line)*10000000.0)
        continue;

    if(ii%3 == 1):
        time_end = int(float(line)*10000000.0)
        continue;


    ## 去掉 头尾的 ""
    line=line[1:len(line)-1]
    if line[-1] in ('"'):
        line=line[:-1]

    ### AA03-1 吞音 
    if line[-2:] == "-1":
        line=line[:-2]

    if line[-2:] in ('03', '13','23'):
        line=line[:-2]
    if line[-1] in ('0', '1','2','3','*'):
        line=line[:-1]


    ## test 
    #if line not in shengyunmu:
    #    print("not in shengyun :%s\n"%(line));


    ###   R
    #line,num = re.subn('R','',line)

    ####  P
    if line == 'sp':
        line='sil'

    if line in shengyunmu:
        fp_out.write("%d %d %s\n"%(time_st, time_end, line));
    else:
        print("error :%s"%(line));




fp_in.close()
fp_out.close()

