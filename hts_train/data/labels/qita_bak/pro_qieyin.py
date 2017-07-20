import sys
import os
import string
import re

if len(sys.argv) < 2:
	print "usage :%s input_file output_file"%(sys.argv[0])
	sys.exit()

fp_in=open(sys.argv[1], 'r')
fp_out=open(sys.argv[2],'w')

shengmu = ('b','c','ch','d','f','g','h','j','k','l','m','n','p','q','r','s','sh','t','uxs','w','x','y','z','zh',); 

lines=fp_in.readlines()
line_old=''
time_st=0
time_end=0
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
		#print ("%d:%s"%(ii,line))
		if line[-1] in ('1','2','3','4','5','6'):
			line=line[:-1]
		if line == 'sp':
			line='sil'

		### r 
		if line[-1] == 'r' and line != 'er' and line != 'r':
			line = line[:-1] 

		### z c s r zh ch sh
		if line == 'iii':
			line = 'ib'; 
		if line == 'ii':
			line = 'if'; 
		if line == 'i' and line_old in ('zh','ch','sh','r'):
			line = 'ib'; 
		if line == 'i' and line_old in ('z','c','s'):
			line = 'if'; 

		## uei->ui	uen->un
		if line == 'uei':
			line = 'ui'; 
		if line == 'uen':
			line = 'un'; 

		###   v ---> ux
		line,num=re.subn('v','ux',line)

		###    as es os
		if line in ('a','ai','an','ang','ao') and line_old not in shengmu:
			#time_mid =  int((time_st + time_end)/2)
			time_mid =  int(time_st + (time_end-time_st)/8)
			fp_out.write("%d %d as\n"%(time_st, time_mid));
			fp_out.write("%d %d %s\n"%(time_mid, time_end, line));
		elif line in ('e','ei','en','eng','er') and line_old not in shengmu:
			time_mid =  int(time_st + (time_end-time_st)/8)
			fp_out.write("%d %d es\n"%(time_st, time_mid));
			fp_out.write("%d %d %s\n"%(time_mid, time_end, line));
		elif line in ('o','ou') and line_old not in shengmu:
			time_mid =  int(time_st + (time_end-time_st)/8)
			fp_out.write("%d %d os\n"%(time_st, time_mid));
			fp_out.write("%d %d %s\n"%(time_mid, time_end, line));

		### y + (u,uan,ue,un) ->>>> uxs
		elif line == 'y':
			line_next = lines[ii+3]
			line_next = line_next[1:len(line_next)-2]
			if line_next[-1] in ('1','2','3','4','5','6'):
				line_next = line_next[:-1]
			if line_next in ('v','van','ve','vn'):
				fp_out.write("%d %d uxs\n"%(time_st, time_end));
			else:
				fp_out.write("%d %d %s\n"%(time_st, time_end, line));
		else:
			fp_out.write("%d %d %s\n"%(time_st, time_end, line));


		line_old = line;

fp_in.close()
fp_out.close()

