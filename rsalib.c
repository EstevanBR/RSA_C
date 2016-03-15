#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include "rsalib.h"

unsigned long getN(unsigned long p, unsigned long q) {
	return p * q;
}

unsigned long getR(unsigned long p, unsigned long q) {
	return (p-1) * (q-1);
}

bool getEandD(unsigned long r, unsigned long *e, unsigned long *d) {
	unsigned long E;
	unsigned long D;
    bool found = false;
	for (unsigned long i = 3; i < r; i++){
	// e needs to be coprime to r
	// which means vvvv
		if (gcd(i, r) == 1) {
			// if found then set e and break
			E = i;
			// d needs to satisfy <d*e = 1 mod r>
			// mathematically then, d = e^-1 mod r
			// a note, x^-1 is the "inverse" of x
			// so modinverse is the same as x^-1 mod y
            D = modInverse(E, r);
			if (D*E % r == 1) {
                found = true;
				break;
			}
		}
	}
    if (found == false) {
        return false;
    }
    printf("right before e,d assignment");
    (*e)=E;
    (*d)=D;
    return true;
}

void encryptToFile(char const *textToEncrypt, unsigned long const n, unsigned long const e, const char *fileName){
	unsigned long max = (unsigned long) strlen(textToEncrypt)+1;
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
		encoded[i] = modpow(textToEncrypt[i], _e, _n);
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
	printf("attempting to decrypt using _d %lu and _n %lu\n", _d, _n);
	FILE *fp = fopen(fileNameSource, "rb");
	if (fp) {
		fseek(fp, 0L, SEEK_END);
		unsigned long max = ftell(fp) / sizeof(unsigned long);
		unsigned long buffer[max];
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
  unsigned long result = 1;
  while (exponent > 0) {
    if (exponent & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exponent >>= 1;
  }
  return result;
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
