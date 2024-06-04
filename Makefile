build:
	gcc -Wall -std=c99 ./main.c `sdl2-config --libs --cflags` -lm -o game

run:
	./game

clean:
	rm game