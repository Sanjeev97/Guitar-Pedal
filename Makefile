# Makefile for AudioNoise

all: convert # Explicitly runs the "all" target

convert: convert.c
	gcc -O2 -o convert convert.c -lm

clean:
	rm -f convert