
cc=gcc

../bin/run.bin: main.o node.o
	cc main.o node.o -lraylib -ldl -lm -o ../bin/run.bin

clean:
	rm *.o
	rm ../bin/run.bin

run: ../bin/run.bin
	../bin/run.bin

main.o: main.c main.h
	cc -c main.c -o main.o

node.o: node.c node.h
	cc -c node.c -o node.o
