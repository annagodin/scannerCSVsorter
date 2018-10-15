all: scannerCSVsorter.c mergesort.o

scannerCSVsorter: scannerCSVsorter.o mergesort.o
	gcc -Wall -o scannerCSVsorter scannerCSVsorter.o mergesort.o
mergesort.o: mergesort.c
	gcc -c mergesort.c
clean:
	rm -f scannerCSVsorter mergesort.o
