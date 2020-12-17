CC = gcc
CFLAGS = -g -Wall -I/home/jmc/include
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
EXE = app

$(EXE): main.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

clean:
	rm *.o $(EXE)

.PHONY: clean
