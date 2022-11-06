CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS=-std=gnu++2b -O3 -Wall -pedantic

all: pthread

pthread: pthread.c
	${CC} pthread.c -o pthread

.PHONY: all clean

clean:
	rm -f pthread *.o
