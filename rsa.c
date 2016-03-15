#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include "rsalib.h"
#define BLURB "\n\nCRYPT:\n\tlate Middle English\n\t(in the sense ‘cavern’):\n\tfrom Latin crypta,\n\tfrom Greek kruptē\n\t‘a vault,’ from kruptos ‘hidden.’\n\n\n"

// void encryptToFile (char const *textToProcess, unsigned long const p, unsigned long const q, const char *fileName);
// void decryptFromFileToFile(unsigned long const p, unsigned long const q, const char *fileName, const char *fileNameDest);
// unsigned long modpow(unsigned long base,unsigned long exponent,unsigned long modulus);
// void clear(void);
// unsigned long modInverse (unsigned long, unsigned long);
// unsigned long gcd (unsigned long, unsigned long);
// unsigned long gcdExtended (unsigned long, unsigned long, unsigned long*, unsigned long*);
// char *inputString (FILE* fp, size_t size, char terminatingCharacter);
// unsigned long getUnsignedLongFromStream(FILE *fp);
// int validateN(unsigned long n);

int main(int argc, char const *argv[]) {
	for (int i = 1; i < argc; i++) {
		printf("%d: %s\n", i, argv[i]);
	}
	printf(BLURB);

	static unsigned long p,q,n,d,r,e,k;
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
			n = p * q;
			r = (p-1) * (q-1);
		} while (!validateN(n));
	// get a valid e
		for (unsigned long i = 3; i < r; i++){
			// e needs to be coprime to r
			// which means vvvv
			if (gcd(i, r) == 1) {
				// if found then set e and break
				e = i;
				// d needs to satisfy <d*e = 1 mod r>
				// mathematically then, d = e^-1 mod r
				// a note, x^-1 is the "inverse" of x
				// so modinverse is the same as x^-1 mod y
				d = modInverse(e, r);
				if (d*e % r == 1) {
					break;
				}
			}
		}
	//printf("confirm d*e = 1 (mod r), where d = %lu,\ne = %lu\nd*e mod r =%lu", d, e, (d*e) % r);
	printf(	"\n*** Public  key e is %lu\n"
			  "*** Public  key n is %lu\n"
			  "*** Private key is d %lu (do not distribute)\n", e, n, d);
			printf("Please enter text to encrypt, terminate with CTRL+D\n");
			char *textToEncrypt = inputString(stdin,10,'\0');
			encryptToFile(textToEncrypt, n , e, "data.enc");
			printf("\n");
			return -1;
		}
		if (choice[0] == 'd') {
			decryptFromFileToFile(d, n, "data.enc", "data.dec");
			return -1;
		}
		if (choice[0] == 'q') {
			return -1;
		}
	printf("\nDone.\n");
	return 0;
}

/*FUNCTION DEFINITIONS*/

