CFLAGS= -Wall -pedantic -std=gnu99

all: main

main:
	gcc $(CFLAGS) `pkg-config --cflags --libs libnotify` main.c -o build/NightsWatch



