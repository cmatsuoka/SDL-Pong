CC=g++
CFLAGS=-O3 -Wall -Werror
OBJECTS=keyboard.o paddle.o ball.o
#LIBS = -lmingw32 -lSDLmain -lSDL_ttf -lSDL
LIBS = -lSDL_ttf -lSDL -lxmp

all:    pong

pong:  $(OBJECTS) 
	$(CC) main.cpp $(OBJECTS) $(LIBS) $(CFLAGS) -o pong
        
paddle.o: paddle.cpp
	$(CC) -c paddle.cpp $(CFLAGS)
       
ball.o: ball.cpp
	$(CC) -c ball.cpp $(CFLAGS)
	   
keyboard.o: keyboard.cpp
	$(CC) -c keyboard.cpp $(CFLAGS)
	
clean:
	rm -f *.o pong
