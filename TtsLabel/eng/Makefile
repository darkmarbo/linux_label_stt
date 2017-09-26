
all:
	g++ -c common.cpp
	g++ -c getCharInfo.cpp
	g++ -c main.cpp
	g++ -o Ttslabel main.o common.o getCharInfo.o
	rm -rf *.o

	rm -rf Ttslabel_bin &&  mkdir -p Ttslabel_bin 
	cp getProcessedFormat.py  ko-kr_rr2ph.phmap ko-kr.phone Ttslabel  run.sh  text.txt Ttslabel_bin

