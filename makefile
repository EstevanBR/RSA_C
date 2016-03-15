CC=gcc
CFLAGS=-c -Wall

all: rsa
	make test

test: rsa
	./rsa -e    < input1.txt
	./rsa --enc < input1.txt
	./rsa -d    < input2.txt
	./rsa --dec < input2.txt

rsa: rsa.o rsalib.o
	$(CC) rsa.o rsalib.o -o rsa

rsa.o: rsa.c
	$(CC) $(CFLAGS) rsa.c

rsalib.o: rsalib.c
	$(CC) $(CFLAGS) rsalib.c

rsac.o: rsac.c rsalib.o
	$(CC) $(CFLAGS) rsac.c rsalib.o

clean:
	rm *.o rsa
