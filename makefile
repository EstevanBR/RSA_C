CC=gcc
CFLAGS=-c -Wall

all: rsa
	make test

test: rsa
	./rsa < input1.txt
	./rsa < input2.txt

rsa: rsa.o rsalib.o
	$(CC) rsa.o rsalib.o -o rsa

rsa.o: rsa.c
	$(CC) $(CFLAGS) rsa.c

rsalib.o: rsalib.c
	$(CC) $(CFLAGS) rsalib.c

rsac.o: rsac.c rsalib.o
	$(CC) $(CFLAGS) rsac.c rsalib.o

clean:
	rm *.o hello
