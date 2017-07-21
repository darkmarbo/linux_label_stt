#coding=utf-8

import os
import sys

if len(sys.argv)<3:
    print("usage: %s file_pinyin file_id  file_pinyin_new "%(sys.argv[0]));
    sys.exit(-1);

fp_in=open(sys.argv[1]);
fp_id=open(sys.argv[2]);
fp_out=open(sys.argv[3],"w");

lines_in=fp_in.readlines();
lines_id=fp_id.readlines();

len_in=len(lines_in)/2;
len_id=len(lines_id);

if len_in != len_id: 
    print("len_infile(%d) != len_id_file(%d)"%(len_in, len_id));
    sys.exit(-2);


for ii in range(0,len_in):
    line=lines_in[2*ii];
    vec_line=line.split("\t");
    if len(vec_line) != 2:
        print("format err:%s"%(line));
        sys.exit(-3);

    cont=vec_line[1];
    id=lines_id[ii][:-1];
    pinyin=lines_in[2*ii+1];
    fp_out.write("%s\t%s"%(id, cont));
    fp_out.write("%s"%(pinyin));

