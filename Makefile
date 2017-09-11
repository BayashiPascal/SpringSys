OPTIONS_DEBUG=-ggdb -g3 -Wall
OPTIONS_RELEASE=-O3 
OPTIONS=$(OPTIONS_RELEASE)
INCPATH=/home/bayashi/Coding/Include
LIBPATH=/home/bayashi/Coding/Include

all : main

main: main.o springsys.o $(LIBPATH)/tgapaint.o $(LIBPATH)/gset.o Makefile 
	gcc $(OPTIONS) main.o springsys.o $(LIBPATH)/tgapaint.o $(LIBPATH)/gset.o -o main -lm

main.o : main.c springsys.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c main.c

springsys.o : springsys.c springsys.h $(INCPATH)/gset.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c springsys.c

clean : 
	rm -rf *.o main

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main

video:
	avconv -r 25 -i ./Frames/%02d.tga -b:v 2048k video.mp4

