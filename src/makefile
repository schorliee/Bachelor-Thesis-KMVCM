CC = gcc -std=c99

all: main run

main: main.c
	${CC} kmv.c countmin.c mergesort.c merge_double.c murmurhash.c help_main.c main.c -o main 

run: 
	./main

clean:
	rm main
	rm -rf min
	rm -rf max
	rm -rf median
	rm -rf mean
