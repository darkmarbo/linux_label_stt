
import os
import sys
import string

if len(sys.argv) < 2:
	print("usage: %s input_dir"%(sys.argv[0]));
dir_name=sys.argv[1]

file_list=os.listdir(sys.argv[1]);
for file_name in file_list:
	dir_file = dir_name+'/'+file_name 	
	new_dir_file = dir_name+'/'+file_name+'.new' 
	print dir_file
	fp=open(dir_file,'r')
	fp_out=open(new_dir_file,'w')
	lines=fp.readlines()
	for line in lines:
		line_split=line.split()
		if len(line_split) != 3:
			print ("%s:%s"%(new_dir_file,line))
		phone=line_split[0]
		### r 
		if phone[-1] == 'r' and phone != 'er' and phone != 'r':
			phone = phone[:-1] 
		st = line_split[1].split('.')[0];
		end = line_split[2].split('.')[0];
		fp_out.write(st + ' ' + end + ' ' + phone + '\n');
	fp.close()
	fp_out.close()
	
	
