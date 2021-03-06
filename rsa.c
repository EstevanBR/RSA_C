#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include "rsalib.h"
#define BLURB "\n\nCRYPT:\n\tlate Middle English\n\t(in the sense ‘cavern’):\n\tfrom Latin crypta,\n\tfrom Greek kruptē\n\t‘a vault,’ from kruptos ‘hidden.’\n\n\n"


int test(void);

int main(int argc, char const *argv[]) {
	test();
	for (int i = 1; i < argc; i++) {
		printf("%d: %s\n", i, argv[i]);
	}
	printf(BLURB);

	static unsigned long p,q,n,d,r,e;
	static unsigned long *encoded;
	static char *decoded;

	printf("Would you like to encrypt or decrypt or quit?\nEnter Choice:_______\b\b\b\b\b\b\b");
	char choice[8];
	strcpy(choice,inputString(stdin, sizeof(char),'\n'));
	for(int i; choice[i];i++) {
		choice[i] = tolower (choice[i]);
	}
	if (choice[0] == 'e') {
		do {
			if (!n) {
				printf("Enter n (0 if you don't have n): ");
				n = getUnsignedLongFromStream(stdin);
			}
			if (!e) {
				printf("Enter e (0 if you don't have e): ");
				e = getUnsignedLongFromStream(stdin);
			}
			
			if (!validateN(n)) {
				printf("\nn is too big... Pick smaller primes (p * q needs to be less than %lu\n\007",(unsigned long) sqrt(ULONG_MAX));	
			}
			printf("Please enter the p (needs to be prime): ");
			p = getUnsignedLongFromStream(stdin);
			if (!p) {
				printf("ERROR p was not read from input properly\007");
				break;
			}
			printf("Please enter the q (needs to be prime): ");
			q = getUnsignedLongFromStream(stdin);
			if (!q) {
				printf("ERROR q was not read from input properly\007");
				break;
			}
			n = getN(p, q);
			r = getR(p, q);
		} while (!validateN(n));
		bool found = getEandD(r, &e, &d);
		if (found == false){
			return -1;
		} 
		//printf("confirm d*e = 1 (mod r), where d = %lu,\ne = %lu\nd*e mod r =%lu", d, e, (d*e) % r);
		printf("\n*** Public key e is                      %lu\n"
			     "*** Public key n is                      %lu\n"
			     "*** Private key d is (do not distribute) %lu\n", e, n, d);
		printf("Please enter text to encrypt, terminate with CTRL+D\n");
		char *textToEncrypt = inputString(stdin,10,'\0');
		encryptToFile(textToEncrypt, n , e, "data.enc");
		printf("\n");
		return 0;
	}
	if (choice[0] == 'd') {
		decryptFromFileToFile(d, n, "data.enc", "data.dec");
		return 0;
	}
	if (choice[0] == 'q') {
		return 0;
	}
	printf("\nDone.\n");
	return 0;
}

/*FUNCTION DEFINITIONS*/


int test(void) {
	unsigned long p,q,n,d,r,e;
	char choice[8] = "encrypt";
	p = 151;
	q = 233;
	n = p*q;
	r = (p-1)*(q-1);
	printf("%lu", r);
	getEandD(r, &e, &d);
	if (e != 7) {
		printf("from test, e is %lu\n", e);
		return -1;
	}
	if (d != 9943) {
		printf("from test, d is %lu\n", d);
		return -1;
	}
	return 0;
}
