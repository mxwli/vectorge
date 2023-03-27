exec=../bin/run.bin
cc=gcc
cflags=-Wall -MMD
lib=-lraylib -ldl -lm
src=$(wildcard *.c)
obj=$(src:.c=.o)
dep=$(obj:.o=.d)

${exec}: ${obj}
	${cc} ${cflags} ${obj} ${lib} -o ${exec}

-include ${dep}

.PHONY: clean cls

clean:
	rm *.o *.d ${exec}

cls:
	clear

run: ${exec}
	${exec}

vgrind: ${exec}
	valgrind ${exec}


