#-*- coding: utf-8 -*-
#!/usr/bin/python

"""
2013-3-8
This program is used to create question list
"""

import os
import re
import sys
import pdb
import struct
import getopt
import ConfigParser


erroMsg = '===> [FATAL ERROR]:'
warnMsg = '===> [WARNING]:'

_DEBUG=True#False

def debugme(dbflag):
    if dbflag:
        pdb.set_trace()
    return


def Usage():
    print """ create_hed.py """


### QS "IS_Qing==1" {*_1/A:*}
### arg_list3 = ['xx','0','1'];
### str='IS_Qing'
### str_left='*_'
### str_right='/A:*'
def make_single(fp_out, arg_list, str, str_left, str_right):
    for aa in arg_list:
        fp_out.write("QS \"%s==%s\" "%(str, aa));
        fp_out.write("{%s%s%s}\n"%(str_left, aa, str_right));

    fp_out.write("\n\n");


### QS "phone_pos==2" {*@2_*}
### argv_list=['xx','1','2','3','4']
### str='phone_pos'
### str_left='*@'
### str_right='_*'
def make_less_equal(fp_out, arg_list, str, str_left, str_right):
    
    for aa in arg_list:
        fp_out.write("QS \"%s==%s\" "%(str, aa));
        fp_out.write("{%s%s%s}\n"%(str_left, aa, str_right));

    fp_out.write("\n");

    for ii in range(1, len(arg_list)):
        ### QS "Seg_Fw<=3"  {*@x_*,*@1_*,*@2_*,*@3_*}
        fp_out.write("QS \"%s<=%s\" {"%(str, arg_list[ii]));
        for jj in range(0,ii+1):
            aa = arg_list[jj];
            if jj == 0:
                fp_out.write("%s%s%s"%(str_left, aa, str_right));
            else:
                fp_out.write(",%s%s%s"%(str_left, aa, str_right));
        fp_out.write("}\n");

    fp_out.write("\n\n");

def MakeTreeHed(monof,qsetf, treehed):
        
    """ Make tree.hed file for HHEd """

    monomap = {}
    
    for line in open(monof).xreadlines():
        line = line.strip()
        if not line:
            continue
        ## qset excluding sp phone
        if line.lower()=='sp':
            continue
        else:
            monomap[line] = 1

    fout = open(treehed, 'wt')
    

    ## LL-Vowel aa^*                       
    for line in open(qsetf).xreadlines():
                
        line = line.strip()
        if not line:
            continue
        ## skip comment
        if line.startswith('#'):
            continue

        ## make question list
        tmp = line.split()

        ## monophone question
        if len(tmp)<2:
            sys.stderr.write('%s, Invalid quesion format: %s' %(erroMsg, line))
            sys.exit(-1)
        else:
            fout.write('QS "LL-%s" {' %tmp[0])
            flag_tmp = 1;
            for leftp in tmp[1:]:
                if leftp in monomap:
                    if flag_tmp == 1:
                        fout.write('%s^*' %leftp)
                        flag_tmp = 0;
                    else:
                        fout.write(',%s^*' %leftp)
                else:
                    print '%s\t%s' %(leftp,line)
                                        
            fout.write('}\n')


    ## L-Vowel    *^aa-*
    fout.write('\n')
    tmp = []
    for line in open(qsetf).xreadlines():
                                
        line = line.strip()
        if not line:
            continue
        ## skip comment
        if line.startswith('#'):
            continue

        ## make question list
        tmp = line.split()

        ## monophone question
        if len(tmp)<2:
            sys.stderr.write('%s, Invalid quesion format: %s' %(erroMsg, line))
            sys.exit(-1)
        else:
            fout.write('QS "L-%s" {' %tmp[0])
            flag_tmp = 1;
            for rightp in tmp[1:]:
                if rightp in monomap:
                    if flag_tmp == 1:
                        fout.write('*^%s-*' %rightp)
                        flag_tmp = 0;
                    else:
                        fout.write(',*^%s-*' %rightp)
                else:
                    print line
            fout.write('}\n')

    fout.write('\n')
    tmp = []
    ## C-Vowel    *-aa+*
    for line in open(qsetf).xreadlines():
                                
        line = line.strip()
        if not line:
            continue
        ## skip comment
        if line.startswith('#'):
            continue

        ## make question list
        tmp = line.split()

        ## monophone question
        if len(tmp)<2:
            sys.stderr.write('%s, Invalid quesion format: %s' %(erroMsg, line))
            sys.exit(-1)
        else:
            fout.write('QS "C-%s" {' %tmp[0])
            flag_tmp = 1;
            for rightp in tmp[1:]:
                if rightp in monomap:
                    if flag_tmp == 1:
                        fout.write('*-%s+*' %rightp)
                        flag_tmp = 0;
                    else:
                        fout.write(',*-%s+*' %rightp)
                else:
                    print line
            fout.write('}\n')

    fout.write('\n')
    tmp = []
    for line in open(qsetf).xreadlines():
                                
        line = line.strip()
        if not line:
            continue
        ## skip comment
        if line.startswith('#'):
            continue

        ## make question list
        tmp = line.split()

        ## monophone question
        if len(tmp)<2:
            sys.stderr.write('%s, Invalid quesion format: %s' %(erroMsg, line))
            sys.exit(-1)
        else:
                        ## R-Vowel    *+aa=*
            fout.write('QS "R-%s" {' %tmp[0])
            flag_tmp = 1;
            for rightp in tmp[1:]:
                if rightp in monomap:
                    if flag_tmp == 1:
                        fout.write('*+%s=*' %rightp)
                        flag_tmp = 0;
                    else:
                        fout.write(',*+%s=*' %rightp)
                else:
                    print line
            fout.write('}\n')

    fout.write('\n')
    tmp = []
    for line in open(qsetf).xreadlines():
                                
        line = line.strip()
        if not line:
            continue
        ## skip comment
        if line.startswith('#'):
            continue

        ## make question list
        tmp = line.split()

        ## monophone question
        if len(tmp)<2:
            sys.stderr.write('%s, Invalid quesion format: %s' %(erroMsg, line))
            sys.exit(-1)
        else:
                        ## RR-Vowel    *=aa@*
            fout.write('QS "RR-%s" {' %tmp[0])
            flag_tmp = 1;
            for rightp in tmp[1:]:
                if rightp in monomap:
                    if flag_tmp == 1:
                        fout.write('*=%s@*' %rightp)
                        flag_tmp = 0;
                    else:
                        fout.write(',*=%s@*' %rightp)
                else:
                    print line
            fout.write('}\n')

    fout.write('\n\n')

    arg_list6 = ['xx','1','2','3','4','5','6'];
    arg_list7 = ['xx','1','2','3','4','5','6','7'];
    arg_list8 = ['xx','1','2','3','4','5','6','7','8'];
    arg_list9 = ['xx','1','2','3','4','5','6','7','8','9'];
    arg_list12 = ['xx','1','2','3','4','5','6','7','8','9','10','11','12'];
    arg_list14 = ['xx','1','2','3','4','5','6','7','8','9','10','11','12','13','14'];
    arg_list17 = ['xx','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17'];
    arg_list19 = ['xx','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19'];
    arg_list24 = ['xx','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24'];

    arg_list7noxx = ['1','2','3','4','5','6','7'];
    arg_list24noxx = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24'];
    arg_list28noxx = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28'];
    arg_list40noxx = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28','29','30','31','32','33','34','35','36','37','38','39','40'];


    ### Phone=Ph    Syllable=Sy  word=Pw  Phrase=Ip utterance=Ut 
    ### 1. 前中后 syllable 是否有 stress
    arg_list_stres = ['xx','0','1'];
    make_single(fout, arg_list_stres, 'l_stress', '*@', '_*');
    make_single(fout, arg_list_stres, 'c_stress', '*_', '/A:*');
    make_single(fout, arg_list_stres, 'r_stress', '*/A:', '_*');


    ## 2.  前中后 syll包含ph个数
    make_less_equal(fout, arg_list6, 'L_NumPhInSy', '*_', '_*');
    make_less_equal(fout, arg_list6, 'C_NumPhInSy', '*_', '/B:*');
    make_less_equal(fout, arg_list6, 'R_NumPhInSy', '*/B:', '-*');
    ## 3. Phone在syllable的位置
    make_less_equal(fout, arg_list6, 'PhInSyPos_Left', '*-', '-*');
    make_less_equal(fout, arg_list6, 'PhInSyPos_Right', '*-', '@*');


    ## 4. word 中 syll 个数 左右位置 
    make_less_equal(fout, arg_list7, 'SyInPwNum', '*@', '-*');
    make_less_equal(fout, arg_list7, 'SyInPwPos_Left', '*-', '&*');
    make_less_equal(fout, arg_list7, 'SyInPwPos_Right', '*&', '-*');


    ## 5. phrase 中 syll个数 左右位置 
    make_less_equal(fout, arg_list24, 'SyInIPNum', '*-', '#*');
    make_less_equal(fout, arg_list24, 'SyInIpPos_Left', '*#', '-*');
    make_less_equal(fout, arg_list24, 'SyInIpPos_Right', '*-', '$*');
    ## 6. phrase 中 word个数 左右位置 
    make_less_equal(fout, arg_list19, 'PwInIPNum', '*$', '-*');
    make_less_equal(fout, arg_list19, 'PwInIpPos_Left', '*-', '!*');
    make_less_equal(fout, arg_list19, 'PwInIpPos_Right', '*!', '-*');


    ## 7. utterance 中 syll  word  phrase 个数
    ##  没有xx 的对应项目 
    make_less_equal(fout, arg_list40noxx, 'SyInUtNum', '*-', ';*');
    make_less_equal(fout, arg_list28noxx, 'PwInUtNum', '*;', '-*');
    make_less_equal(fout, arg_list7noxx, 'IpInUtNum', '*-', '|*');
    ## 8. utterance 中 phrase 位置 
    make_less_equal(fout, arg_list7, 'IpInUtPos_Left', '*|', '/C:*');
    make_less_equal(fout, arg_list7, 'IpInUtPos_Right', '*/C:', '');





    fout.close()        


if __name__ == '__main__':
        if len(sys.argv) != 4:
                print 'python %s monof qsetf treehed' % sys.argv[0]
        else:
                MakeTreeHed(sys.argv[1],sys.argv[2],sys.argv[3])








































