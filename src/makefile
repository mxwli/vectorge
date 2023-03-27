exec=../bin/run.bin
cc=gcc
cflags=-Wall -MMD
lib=-lraylib -ldl -lm
src=$(wildcard *.c)
obj=$(SRC:.c=.o)
dep=$(obj:.o=.d)


${exec}: ${obj}
	${cc} ${cflags} ${obj} ${lib} -o ${exec}

-include ${dep}

.PHONY: clean cls

clean:
	rm ${obj}
	rm ${exec}

cls:
	clear

run: ${exec}
	${exec}

vgrind: ${exec}
	valgrind ${exec}




../bin/run.bin: main.o node.o advvec.o game.o entity.o editor.o fx.o
	clear
	cc main.o node.o game.o advvec.o entity.o editor.o fx.o -lraylib -ldl -lm -o ../bin/run.bin ${cflags}

clean:
	rm *.o
	rm ../bin/run.bin

cls:
	clear

run: ../bin/run.bin
	../bin/run.bin

vgrind: ../bin/run.bin
	valgrind ../bin/run.bin

main.o: main.c main.h
	cc -c main.c -o main.o ${cflags}

node.o: node.c node.h
	cc -c node.c -o node.o ${cflags}

advvec.o: advvec.c advvec.h
	cc -c advvec.c -o advvec.o ${cflags}

game.o: game.c game.h
	cc -c game.c -o game.o ${cflags}

entity.o: entity.c entity.h
	cc -c entity.c entity.o ${cflags}

editor.o: editor.c editor.h
	cc -c editor.c editor.o ${cflags}

fx.o: fx.c fx.h
	cc -c fx.c fx.o ${cflags}
