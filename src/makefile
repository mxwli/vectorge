
cc=gcc

../bin/run.bin: main.o
	cc main.o -lraylib -ldl -lm -o ../bin/run.bin

clean:
	rm *.o
	rm ../bin/run.bin

run: ../bin/run.bin
	../bin/run.bin

main.o: main.c main.h
	cc -c main.c -o main.o


