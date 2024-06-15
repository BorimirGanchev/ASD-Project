build:
	gcc -o algorithm algorithm.c
	gcc -Wall -std=c99 ./main.c `sdl2-config --libs --cflags` -lm -o main

run:
	./algorithm
	./game

clean:
	rm game