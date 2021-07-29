Assembler: dataImage.o  firstPass.o inputAnalyze.o  labelTableLinkedList.o
	gcc -g dataImage.o firstPass.o inputAnalyze.o labelTableLinkedList.o -o Assembler -lm

dataImage.o: dataImage.c dataImage.h defaults.h
	gcc -c dataImage.c -Wall -pedantic -ansi -o dataImage.o

firstPass.o: firstPass.c firstPass.h defaults.h inputAnalyze.h labelTableLinkedList.h dataImage.h 
	gcc -c firstPass.c -Wall -pedantic -ansi -o firstPass.o

inputAnalyze.o: inputAnalyze.c inputAnalyze.h defaults.h
	gcc -c inputAnalyze.c -Wall -pedantic -ansi -o inputAnalyze.o

labelTableLinkedList.o: labelTableLinkedList.c labelTableLinkedList.h defaults.h
	gcc -c labelTableLinkedList.c -Wall -pedantic -ansi -o labelTableLinkedList.o
