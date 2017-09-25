
all:
	g++ -c getCharInfo.cpp
	g++ -c main.cpp
	g++ -o Ttslabel main.o getCharInfo.o
	rm -rf main.o getCharInfo.o

	rm -rf Ttslabel_bin &&  mkdir -p Ttslabel_bin 
	cp getProcessedFormat.py  ko-kr_rr2ph.phmap ko-kr.phone Ttslabel  run.sh  text.txt Ttslabel_bin

