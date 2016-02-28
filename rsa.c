#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#define BLURB "\n\nCRYPT:\n\tlate Middle English\n\t(in the sense ‘cavern’):\n\tfrom Latin crypta,\n\tfrom Greek kruptē\n\t‘a vault,’ from kruptos ‘hidden.’\n\n\n"

void encryptToFile (char const *textToProcess, unsigned long const p, unsigned long const q, const char *fileName);
void decryptFromFileToFile(unsigned long const p, unsigned long const q, const char *fileName, const char *fileNameDest);
unsigned long modpow(unsigned long base,unsigned long exponent,unsigned long modulus);
void clear(void);
unsigned long modInverse (unsigned long, unsigned long);
unsigned long gcd (unsigned long, unsigned long);
unsigned long gcdExtended (unsigned long, unsigned long, unsigned long*, unsigned long*);
char *inputString (FILE* fp, size_t size, char terminatingCharacter);
unsigned long getUnsignedLongFromStream(FILE *fp);
int validateN(unsigned long n);

int main(int argc, char const *argv[]) {
	for (int i = 1; i < argc; i++) {
		printf("%d: %s\n", i, argv[i]);
	}
	printf(BLURB);
	static unsigned long p,q,n,d,r,e,k;
	static unsigned long publicKey[2];
	static unsigned long primesForK[2];
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

void encryptToFile(char const *textToEncrypt, unsigned long const n, unsigned long const e, const char *fileName){
	unsigned long max = (unsigned long) strlen(textToEncrypt)+1;
	//printf("\nmax is %lu, n is %lu, e is %lu", max, n, e);
	unsigned long encoded[max];
	unsigned long _n = n;
	unsigned long _e = e;
	if (!n) {
		printf("Please enter n: ");
		_n = getUnsignedLongFromStream(stdin);
	}
	if (!e) {
		printf("Please enter e: ");
		_e = getUnsignedLongFromStream(stdin);
	}
	unsigned long i;
	for (i = 0; i < max; i++) {
		//printf("\nencoding %c / %d as", textToEncrypt[i], textToEncrypt[i]);
		encoded[i] = modpow(textToEncrypt[i], _e, _n);
		//printf("\n		 %c / %lu", (char) encoded[i], encoded[i]);
	}
	FILE *fp = fopen(fileName, "wb");
	fwrite(encoded, sizeof(unsigned long), max, fp);
	fclose(fp);
}
void decryptFromFileToFile(unsigned long const d, unsigned long const n, const char *fileNameSource, const char *fileNameDest) {
	unsigned long _d = d;
	unsigned long _n = n;
	if (!d) {
		printf("Please the your private key, d: ");
		_d = getUnsignedLongFromStream(stdin);
	}
	if (!n) {
		printf("Please enter the public key, n: ");
		_n = getUnsignedLongFromStream(stdin);
	}
	FILE *fp = fopen(fileNameSource, "rb");
	if (fp) {
		fseek(fp, 0L, SEEK_END);
		unsigned long max = ftell(fp) / sizeof(unsigned long);
		unsigned long buffer[max];
		//printf("max is %lu", max);
		fseek(fp, SEEK_SET, 0);
		unsigned long bytes = fread(buffer, sizeof(unsigned long), max, fp);
		fclose(fp);
		
		char decodedText[max];
		for (unsigned long i = 0; i < max;i++) {
			decodedText[i] = modpow(buffer[i], _d, _n);
		}
		printf("\n# BEGIN DECODED TEXT #\n%s\n# END DECODED TEXT #\n", decodedText);
		fp = fopen(fileNameDest, "w");
		if (fp) {
			fwrite(decodedText, 1, max, fp);
			fclose(fp);
		} else {
			printf("ERROR  FAILED TO OPEN DESTINATION FILE\007");
			return;
		}
	} else {
		printf("ERROR FAILED TO OPEN SOURCE FILE\007");
		return;
	}
}

unsigned long modpow(unsigned long base, unsigned long exponent, unsigned long modulus) {
  base %= modulus;
  //printf("\n\n%-10s%10lu", "modulus", modulus);
  unsigned long result = 1;
  while (exponent > 0) {
  	//printf("\n%-10s%10lu,%-10s%10lu,%-10s%c", "base", base, "exponent",exponent,"result", (char) result);
    if (exponent & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exponent >>= 1;
  }
  return result;
}

void clear(void) {
	while ( getchar() != '\n' );
}

unsigned long modInverse(unsigned long a, unsigned long m)
{
    unsigned long x, y;
    unsigned long g = gcdExtended(a, m, &x, &y);
    if (g != 1)
        return -1;
    else
    {
        // m is added to handle negative x
        unsigned long res = (x % m + m) % m;
        return res;
    }
    
}
 
// C function for extended Euclidean Algorithm
unsigned long gcdExtended(unsigned long a, unsigned long b, unsigned long *x, unsigned long *y)
{
    // Base Case
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }
 
    unsigned long x1, y1; // To store results of recursive call
    unsigned long g = gcdExtended(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b/a) * x1;
    *y = x1;
 
    return g;
}

unsigned long gcd( unsigned long a, unsigned long b )
{
  unsigned long c;
  while ( a != 0 ) {
     c = a;
     a = b%a;
     b = c;
  }
  return b;
}

unsigned long getUnsignedLongFromStream(FILE *fp) {
	char *aString = inputString(fp, 10, '\n');
	unsigned long anUnsignedLong;
	sscanf(aString,"%lu", &anUnsignedLong);
	return anUnsignedLong;
};

//http://stackoverflow.com/a/16871702
char *inputString(FILE* fp, size_t size, char terminatingCharacter){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    if (terminatingCharacter != '\0') {
	    while(EOF!=(ch=fgetc(fp)) && ch != terminatingCharacter) {
	        str[len++]=ch;
	        if(len==size){
	            str = realloc(str, sizeof(char)*(size+=16));
	            if(!str)return str;
	        }
	    }
	} else {
		while(EOF!=(ch=fgetc(fp))) {
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        	}
    	}
	}
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

int validateN(unsigned long n) {
	return (n < (unsigned long) sqrt(ULONG_MAX));
};