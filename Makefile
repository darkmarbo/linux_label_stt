
all:
	gcc -c getCharInfo.c
	gcc -c main.c
	gcc -o Ttslabel main.o getCharInfo.o
	rm -rf main.o getCharInfo.o


