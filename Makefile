CC=gcc

#build sinus_generator
sn_gn: make_wav.o make_sin.o
	$(CC) make_sin.o make_wav.o -lm -o sinus_generator

make_wav.o: make_wav.c
	$(CC) -c make_wav.c -o make_wav.o

main.o: make_sin.c
	$(CC) -c make_sin.c -o make_sin.o


#build fft
fft: fft.o fft_an.o
	$(CC) fft.o fft_an.o -lm -o fft

fft.o: fft.c
	$(CC) -c fft.c -o fft.o

fft_an.o: fft_an.c
	$(CC) -c fft_an.c -o fft_an.o

clean:
	rm *.o