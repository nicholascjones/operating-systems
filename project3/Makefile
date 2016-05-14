
CFLAGS = -Wall

all: mandel mandelmovie

mandel: mandel.o bitmap.o
	gcc mandel.o bitmap.o -o mandel -lm -pthread

mandelmovie: mandelmovie.o
	gcc mandelmovie.o -o mandelmovie -lm -pthread

mandel.o: mandel.c
	gcc -Wall -g -c mandel.c -o mandel.o -lm -pthread

mandelmovie.o: mandelmovie.c
	gcc -Wall -g -c mandelmovie.c -o mandelmovie.o -lm -pthread

bitmap.o: bitmap.c
	gcc -Wall -g -c bitmap.c -o bitmap.o

clean:
	rm -f mandel.o bitmap.o mandel
