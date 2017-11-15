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
### arg_list3 = ['x','0','1'];
### str='IS_Qing'
### str_left='*_'
### str_right='/A:*'
def make_single(fp_out, arg_list, str, str_left, str_right):
    for aa in arg_list:
        fp_out.write("QS \"%s==%s\" "%(str, aa));
        fp_out.write("{%s%s%s}\n"%(str_left, aa, str_right));

    fp_out.write("\n\n");


### QS "phone_pos==2" {*@2_*}
### argv_list=['x','1','2','3','4']
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

    arg_list6 = ['x','1','2','3','4','5','6'];
    arg_list7 = ['x','1','2','3','4','5','6','7'];
    arg_list8 = ['x','1','2','3','4','5','6','7','8'];
    arg_list9 = ['x','1','2','3','4','5','6','7','8','9'];
    arg_list12 = ['x','1','2','3','4','5','6','7','8','9','10','11','12'];
    arg_list14 = ['x','1','2','3','4','5','6','7','8','9','10','11','12','13','14'];
    arg_list17 = ['x','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17'];
    arg_list19 = ['x','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19'];
    arg_list24 = ['x','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24'];
    arg_list28 = ['x','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28'];

    ### 1. phone 在 syllable 中的位置 
    make_less_equal(fout, arg_list7, 'phone_pos', '*@', '_*');

    ### 1位. 当前是否是句重音 
    arg_list_accent = ['x','0','1'];
    make_single(fout, arg_list_accent, 'IS_Qing', '*_', '/A:*');

    ### 5位. 前后5个phoneme的重音 
    arg_list_stres = ['x','0','1','2','4'];
    make_single(fout, arg_list_stres, 'LL_DiaoXing', '*/A:', '_*');
    make_single(fout, arg_list_stres, 'L_DiaoXing', '*_', '_*');
    make_single(fout, arg_list_stres, 'C_DiaoXing', '*_', '/B:*');
    make_single(fout, arg_list_stres, 'R_DiaoXing', '*/B:', '-*');
    make_single(fout, arg_list_stres, 'RR_DiaoXing', '*-', '-*');


    ### 8. 音节 在 韵律词|韵律短语|语调短语|句子  的位置
    make_less_equal(fout, arg_list7, 'PhoneInPwWordPosi_Left', '*-', '@*');
    make_less_equal(fout, arg_list7, 'PhoneInPwWordPosi_Right', '*@', '-*');
    make_less_equal(fout, arg_list12, 'PhoneInPpPosi_Left', '*-', '&*');
    make_less_equal(fout, arg_list12, 'PhoneInPpPosi_Right', '*&', '-*');
    make_less_equal(fout, arg_list19, 'PhoneInIpPosi_Left', '*-', '#*');
    make_less_equal(fout, arg_list19, 'PhoneInIpPosi_Right', '*#', '-*');
    make_less_equal(fout, arg_list28, 'PhoneInSentPosi_Left', '*-', '$*');
    make_less_equal(fout, arg_list28, 'PhoneInSentPosi_Right', '*$', '-*');
    ### 4位 韵律词|韵律短语|语调短语|句子  包含音节个数
    ### label 的 sil 需要修改0为x
    make_less_equal(fout, arg_list7, 'PhoneInPwNum', '*-', '!*');
    make_less_equal(fout, arg_list12, 'PhoneInPpNum', '*!', '-*');
    make_less_equal(fout, arg_list19, 'PhoneInIpNum', '*-', ';*');
    make_less_equal(fout, arg_list28, 'PhoneInSentNum', '*;', '-*');



    ### 韵律词 在 *** 的位置 
    make_less_equal(fout, arg_list9, 'PwInPpPosi_Left', '*-', '|*');
    make_less_equal(fout, arg_list9, 'PwInPpPosi_Right', '*|', '/C:*');
    make_less_equal(fout, arg_list14, 'PwInIpPosi_Left', '*/C:', '+*');
    make_less_equal(fout, arg_list14, 'PwInIpPosi_Right', '*+', '+*');
    make_less_equal(fout, arg_list24, 'PwInSentPosi_Left', '*+', '/D:*');
    make_less_equal(fout, arg_list24, 'PwInSentPosi_Right', '*/D:', '_*');
    ### *** 包含韵律词个数 
    make_less_equal(fout, arg_list9, 'PwInPpNum', '*_', '/E:*');
    make_less_equal(fout, arg_list14, 'PwInIpNum', '*/E:', '+*');
    make_less_equal(fout, arg_list24, 'PwInSentNum', '*+', '@*');



    ##  韵律短语  在 ** 的位置 
    make_less_equal(fout, arg_list6, 'PpInIpPosi_Left', '*@', '+*');
    make_less_equal(fout, arg_list6, 'PpInIpPosi_Right', '*+', '&*');
    make_less_equal(fout, arg_list8, 'PpInSentPosi_Left', '*&', '+*');
    make_less_equal(fout, arg_list8, 'PpInSentPosi_Right', '*+', '#*');
    ### ** 包含 韵律短语的个数 
    make_less_equal(fout, arg_list6, 'PpInIpNum', '*#', '+*');
    make_less_equal(fout, arg_list8, 'PpInSentNum', '*+', '/F:*');



    ### 语调短语 在 * 的位置
    make_less_equal(fout, arg_list6, 'IpInSentPosi_Left', '*/F:', '_*');
    make_less_equal(fout, arg_list6, 'IpInSentPosi_Right', '*_', '/G:*');
    ### * 包含语调短语的个数 
    make_less_equal(fout, arg_list6, 'IpInSentNum', '*/G:', '');



    fout.close()        


if __name__ == '__main__':
        if len(sys.argv) != 4:
                print 'python %s monof qsetf treehed' % sys.argv[0]
        else:
                MakeTreeHed(sys.argv[1],sys.argv[2],sys.argv[3])








































