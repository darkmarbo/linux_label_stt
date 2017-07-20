
test:
	rm -f tmp
	for lab in labels/full/*.lab
	do	
		echo $${lab}
		bs=`basename ${lab}`
		#echo $bs
		bs_1=`basename ${lab} .lab`.cmp
		#echo $bs_1
	
		if [ -s $${lab} -a -s labels/mono/`basename $${lab}` -a -s cmp/`basename $${lab} .lab`.cmp ]
		then 
			sed -e "s/.* //g" $${lab} >> tmp; 
		fi 
	done
	sort -u tmp > lists/full.list

