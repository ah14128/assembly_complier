FLAGS = -std=c99 -Wall

all: zxpre.exe zxenc.exe

zxpre.exe: zx256.h zxpremain.c zx256.c
	gcc $(FLAGS) zxpremain.c zx256.c -o zxpre.exe

zxenc.exe: zx256.h zxencmain.c zx256.c
	gcc $(FLAGS) zxencmain.c zx256.c -o zxenc.exe


test: zxpre.exe 
	python tests/runtests.py