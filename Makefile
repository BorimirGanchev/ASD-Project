build:
	gcc -o algorithm algorithm.c
	gcc -Wall -std=c99 ./main.c `sdl2-config --libs --cflags` -lm -o main

run:
	./algorithm
	./main

clean:
	rm main
	rm algorithm