#https://www.gnu.org/software/make/manual/html_node/Quick-Reference.html

#CONSIDER USING AUTOCONF, OR, WRITING ONE SEPARATE MAKEFILE FOR EACH OS

CC=gcc
#CFLAGS=-lSDL

all: ks-client-sdl1.2 ks-server-x11

ks-client-sdl1.2: ok.o ok-sdl1.2.o dasnethelp.o ks-client-sdl1.2.o
	$(CC) -o ../$@ $^ -lSDL

ks-server-x11: ok.o ok-x11.o dasnethelp.o ks-server-x11.o
	$(CC) -o ../$@ $^ -lX11 -lXtst

%.o: %.c
	$(CC) -c $<

.PHONY: clean

clean:
	rm -f *.o ../ks-client-sdl1.2 ../ks-server-x11
