
cc=gcc
# cflags=-fdiagnostics-color=always

../bin/run.bin: main.o node.o advvec.o game.o entity.o editor.o
	clear
	cc main.o node.o game.o advvec.o entity.o editor.o -lraylib -ldl -lm -o ../bin/run.bin ${cflags}

clean:
	rm *.o
	rm ../bin/run.bin

cls:
	clear

run: ../bin/run.bin
	../bin/run.bin

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
