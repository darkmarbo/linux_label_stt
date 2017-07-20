
import sys
import os
import string


cixing=sys.argv[1]
yunlv=sys.argv[2]

fp=open(cixing,'r',-1,'utf-8')
line_cixing=fp.readlines();
fp.close();

fp=open(yunlv,'r',-1,'utf-8')
line_yunlv=fp.readlines();
fp.close();

num_cixing=len(line_cixing)
num_yunlv=len(line_yunlv)
if num_cixing != num_yunlv:
	print ("num_cixing(%d) != num_yunlv(%d)"%(num_cixing,num_yunlv))
	sys.exit -1

fp_ps1=open('ps1.txt','w',-1,'utf-8')
fp_ps2=open('ps2.txt','w',-1,'utf-8')
for ii in range(num_cixing):
	ln=line_cixing[ii];
	lp=ln.split();
	#一两/m 蒜/n 几乎/d 相当/d 于/p 一斤/m 黄瓜/n 价/n 
	k=0;
	for i in range(len(lp)):
		wd=lp[i];
		if wd.find('/')<0:
			k=1;
		if k==0:
			fp_ps1.write(line_yunlv[ii])
			fp_ps1.write(ln);
		else:
			# 未标注词性的句子
			fp_ps2.write(ln)

fp_ps1.close();
fp_ps2.close()


