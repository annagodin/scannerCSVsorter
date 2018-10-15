all: sorter.c mergesort.o searchDir.o

sorter: sorter.o mergesort.o searchDir.o
	gcc -Wall -o sorter sorter.o mergesort.o searchDir.o
mergesort.o: mergesort.c
	gcc -c mergesort.c
clean:
	rm -f sorter mergesort.o searchDir.o
