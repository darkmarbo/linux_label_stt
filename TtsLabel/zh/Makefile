
all:
	gcc -c getCharInfo.c
	gcc -c main.c
	gcc -o Ttslabel main.o getCharInfo.o
	rm -rf main.o getCharInfo.o

	rm -rf Ttslabel_bin &&  mkdir -p Ttslabel_bin 
	cp getProcessedFormat.py  Ttslabel  run.sh  text.txt Ttslabel_bin

