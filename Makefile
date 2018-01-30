CC=g++
SDIR=../src
BDIR=../bin
ODIR=../bin/obj
EXE=make_wav

all: make_wav

make_wav: make_wav.o main.o
	$(CC) $(ODIR)/main.o $(ODIR)/make_wav.o -o $(BDIR)/make_wav

make_wav.o: make_wav.c
	$(CC) -c make_wav.c -o $(ODIR)/make_wav.o

main.o: make_sin.c
	$(CC) -c main.cpp -o $(ODIR)/main.o

clean:
	rm $(ODIR)/*.o $(BDIR)/make_wav test.wav