main: main.c fft-real-pair.c fft-real-pair.h
	gcc -o main main.c fft-real-pair.c -lwiringPi -lm
