#coding=utf-8

import os
import sys

yindiao=['1','2','3','4','5']
dict_py={};

def load_dict(file):
    print("load dict:%s"%(file));
    fp = open(file);
    for line in fp:
        line = line[:-1]

        vec_line = line.split('\t');
        if len(vec_line)<2:
            print("format err: %s"%(line));
            return -1;
        pinyin=vec_line[0]
        phone = vec_line[1]
        #print("test:%s"%(pinyin));
        if not dict_py.has_key(pinyin):
            dict_py[pinyin] = phone;


    fp.close();
    return 0;
    

print("开始....")

if len(sys.argv)<3:
    print("usage: %s in_file out_file "%(sys.argv[0]));
    sys.exit(0);

ret = load_dict("phone-snda-RS-TS-71.txt");
if ret != 0:
    print("load dict err!");
    sys.exit(-1);

fp_in = open(sys.argv[1]);
fp_out = open(sys.argv[2],"w");


num=0;
for line in fp_in:
    line = line[:-1]
    num += 1;
    if num % 2 == 1:
        fp_out.write("%s\n"%(line));
        continue;

    
    vec_line = line.split('\t');
    if len(vec_line)<2:
        print("input format err:%s"%(line));
        sys.exit(-1);

    line = vec_line[1];
    vec_line = line.split(' ');
    ii=0;
    for pinyin in vec_line:
        yd = pinyin[-1];
        pinyin = pinyin[:-1]
        if yd not in yindiao:
            print("pinyin format err:%s"%(pinyin));
            sys.exit(-2);
        
        if not dict_py.has_key(pinyin):
            ### 儿化音 
            if pinyin[-1] == 'r' and dict_py.has_key(pinyin[:-1]):
                phone1 = dict_py[pinyin[:-1]];
                if ii == 0:
                    fp_out.write("\t%sr%s"%(phone1,yd));
                else:
                    fp_out.write(" %sr%s"%(phone1,yd));

                continue;

            print("error:pinyin=%s not in dict_py!"%(pinyin));
            sys.exit(-3);

        phone = dict_py[pinyin];
        if ii == 0:
            fp_out.write("\t%s%s"%(phone,yd));
        else:
            fp_out.write(" %s%s"%(phone,yd));

        ii += 1;

    fp_out.write("\n");


fp_in.close();
fp_out.close();


