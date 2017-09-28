
all:
	g++ -c common.cpp
	g++ -c getCharInfo.cpp
	g++ -c main.cpp
	g++ -o Ttslabel main.o common.o getCharInfo.o
	rm -rf *.o

	rm -rf Ttslabel_bin &&  mkdir -p Ttslabel_bin 
	cp pro_text.py  getProcessedFormat.py  eng.phone Ttslabel  run.sh  Ttslabel_bin

