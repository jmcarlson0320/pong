CC = gcc
CFLAGS = -g -Wall -I/home/jmc/include
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
EXE = app

$(EXE): main.o player.o ball.o collisions.o
	$(CC) -o $@ $^ $(LIBS)

main.o: defs.h
player.o: defs.h
ball.o: defs.h
collisions.o: defs.h

clean:
	rm *.o $(EXE)

.PHONY: clean
