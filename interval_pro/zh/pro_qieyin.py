
import sys
import os
import string
import re

if len(sys.argv) < 2:
	print "usage :%s input_file output_file"%(sys.argv[0])
	sys.exit()

fp_in=open(sys.argv[1], 'r')
fp_out=open(sys.argv[2],'w')

shengyunmu = ('aa','oo','ee','ii','uu','a','ai','an','ang','ao','as','b','c','ch','d','e','ei','en','eng','er','es','f','g','h','i','ia','ian','iang','iao','ib','ie','if','in','ing','iong','iu','j','k','l','m','n','o','ong','os','ou','p','q','r','s','sh','t','u','ua','uai','uan','uang','ui','un','uo','ux','uxan','uxe','uxn','uxs','w','x','y','z','zh','sil');

shengmu = ('aa','oo','ee','ii','uu','b','c','ch','d','f','g','h','j','k','l','m','n','p','q','r','s','sh','t','uxs','w','x','y','z','zh'); 

lines=fp_in.readlines()
line_old=''
time_st=0
time_end=0
time_old_st=0
time_old_end=0

#print("%d"%(len(shengyunmu)));

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
	if(ii%3 == 1):
		time_end = int(float(line)*10000000.0)
	if(ii%3 == 2):
		line=line[1:len(line)-1]
		if line[-1] in ('1','2','3','4','5','6','"'):
			line=line[:-1]
		if line[-1] in ('1','2','3','4','5','6','"'):
			line=line[:-1]
		if line[-1] in ('1','2','3','4','5','6','"'):
			line=line[:-1]
		#print ("%d:%s"%(ii,line))
		line_bak = line;

        ## test 
		#if line not in shengyunmu:
		#	print("not in shengyun :%s\n"%(line));

		if line in ('bc','pc','dc','tc','gc','kc','zc','cc','zhc','chc','jc','qc'):
			line_old = line_bak;
			time_old_st = time_st;
			time_old_end = time_end;
			continue;

		#if line in ('b','p','d','t','g','k','z','c','zh','ch','j','q'):
		#	if line_old == "%sc"%(line):
		#		#print ("test2:%s\t%s"%(line,line_old));
		#		fp_out.write("%d %d %s\n"%(time_old_st, time_end, line));
		#	else:
		#		print("!!!! c err[now,old][%s:%s]!!!!!!!!\n"%(line,line_old));

		#	line_old = line_bak;
		#	time_old_st = time_st;
		#	time_old_end = time_end;
		#	continue;

		if line == 'y' or line == 'ii' or line == 'vv':
			line_next = lines[ii+3]
			line_next = line_next[1:len(line_next)-2]
			if line_next[-1] in ('1','2','3','4','5','6','"'):
				line_next = line_next[:-1]
			#if line_next in ('v','ivan','ive','ivn'): ## xijinping 20170328
			if line_next in ('v','ivan','ive','ivn','ux','uxan','uxe','uxn','van','ve','vn') or line_next[:-1] in ('v','ivan','ive','ivn','ux','uxan','uxe','uxn','van','ve','vn') :
				fp_out.write("%d %d uxs\n"%(time_st, time_end));
			else:
				fp_out.write("%d %d y\n"%(time_st, time_end));

			line_old = line_bak;
			time_old_st = time_st;
			time_old_end = time_end;
			continue;

		if line in ('v','ivan','ive','ivn','van','ve','vn') and line_old == 'y':
			line,num = re.subn('iv','ux',line);
			line,num = re.subn('v','ux',line);
			#print("test1:%s"%(line));
			fp_out.write("%d %d %s\n"%(time_st, time_end, line));
			line_old = line_bak;
			time_old_st = time_st;
			time_old_end = time_end;
			continue;

		######    as es os
		if line in ('a','ai','an','ang','ao') and line_old not in shengmu:
			#print("test_shengmu:%s"%(line_old));
			time_mid =  int(time_st + (time_end-time_st)/8)
			fp_out.write("%d %d as\n"%(time_st, time_mid));
			fp_out.write("%d %d %s\n"%(time_mid, time_end, line));

			line_old = line_bak;
			time_old_st = time_st;
			time_old_end = time_end;
			continue;
		if line in ('e','ei','en','eng','er') and line_old not in shengmu:
			#print("test_shengmu:%s"%(line_old));
			time_mid =  int(time_st + (time_end-time_st)/8)
			fp_out.write("%d %d es\n"%(time_st, time_mid));
			fp_out.write("%d %d %s\n"%(time_mid, time_end, line));

			line_old = line_bak;
			time_old_st = time_st;
			time_old_end = time_end;
			continue;
		if line in ('o','ou') and line_old not in shengmu:
			#print("test_shengmu:%s"%(line_old));
			time_mid =  int(time_st + (time_end-time_st)/8)
			fp_out.write("%d %d os\n"%(time_st, time_mid));
			fp_out.write("%d %d %s\n"%(time_mid, time_end, line));

			line_old = line_bak;
			time_old_st = time_st;
			time_old_end = time_end;
			continue;
		if line == 'ng':
			#print("test_shengmu:%s"%(line_old));
			time_mid =  int(time_st + (time_end-time_st)/8)
			fp_out.write("%d %d es\n"%(time_st, time_mid));
			fp_out.write("%d %d en\n"%(time_mid, time_end));

			line_old = line_bak;
			time_old_st = time_st;
			time_old_end = time_end;
			continue;

		###   R
		line,num = re.subn('R','',line)

		####  P
		if line == 'P' or line == 'sp':
			line='sil'

		#### z c s r zh ch sh
		if line == 'ih':
			line = 'ib'; 
		if line == 'i' and line_old in ('zh','ch','sh','r'):
			line = 'ib'; 
		if line == 'i' and line_old in ('z','c','s'):
			line = 'if'; 

		####   v ---> ux 
		if line == 'iv':
			print("test!!!!!!!!!!vvvvvvvvvvvvv\n");
		line,num = re.subn('iv','ux',line);
		line,num = re.subn('v','ux',line)

		if line == 'io':
			line = 'o'; 
		if line == 'iou':
			line = 'iu'; 

		### uei->ui	uen->un
		if line == 'uei':
			line = 'ui'; 
		if line == 'uen':
			line = 'un'; 
		if line == 'ueng':
			line = 'eng'; 

        ## aa ee oo
		if line == 'aa':
			line = 'as'; 
		if line == 'ee':
			line = 'es'; 
		if line == 'oo':
			line = 'os'; 
		if line == 'uu':
			line = 'w'; 
		if line == 'ix':
			line = 'ib'; 
		if line == 'iy':
			line = 'if'; 

		if line[-1:] == 'r' and line not in shengyunmu:
			line = line[:-1]
		
		if line in shengyunmu:
			fp_out.write("%d %d %s\n"%(time_st, time_end, line));
		else:
			print("error :%s\n"%(line));


		line_old = line_bak;
		time_old_st = time_st;
		time_old_end = time_end;



fp_in.close()
fp_out.close()

