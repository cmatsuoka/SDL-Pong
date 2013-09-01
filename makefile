CC=g++
CFLAGS=-O3 -Wall -Werror
OBJECTS=main.o keyboard.o paddle.o ball.o
#LIBS = -lmingw32 -lSDLmain -lSDL_ttf -lSDL
LIBS = -lSDL_ttf -lSDL -lxmp

.cpp.o:
	$(CC) -c $(CFLAGS) -o $*.o $<

all:    pong

pong:  $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)
        
clean:
	rm -f *.o pong
