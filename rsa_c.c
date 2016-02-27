#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
//#include "primefac.h"

long* encrypt(char const *textToProcess, long const p, long const q);
char* decrypt(const long *encoded, long const p, long const q);
//long* getFactorsForNumber(long n);
long modpow(long base,long exponent,long modulus);
void clear(void);
void printLong(const long *longToPrint);
long modInverse(long, long);
long gcd(long, long);
long gcdExtended(long, long, long*, long*);


int main(int argc, char const *argv[]) {
	long p,q,n,d,r,e,k;
	long publicKey[2];
	long primesForK[2];
	char textToEncode[255];
	long *encoded;
	char *decoded;
	printf("Please enter the p (needs to be prime): ");
	scanf("%ld", &p);
	if (!p) {
		return 0;
	}
	printf("Please enter the q (needs to be prime): ");
	scanf("%ld", &q);
	if (!q) {
		return 0;
	}
	n = p * q;
	r = (p-1) * (q-1);
	
	/*
	for (long i = 40; i > 0; i--) {
		k = (i * r) + 1;
		if (getLargestPrimeFactor(k, primesForK) == 2) {
			e = primesForK[0];
			d = primesForK[1];
			break;
		}
	}*/

	// get a valid e
	for (long i = 3; i < r; i++){
		// e needs to be coprime to r
		// which means vvvv
		if (gcd(i, r) == 1) {
			// if found then set e and break
			e = i;
			break;
		}
	}

	// d needs to satisfy <d*e = 1 mod r>
	// mathematically then, d = e^-1 mod r
	// a note, x^-1 is the "inverse" of x
	// so modinverse is the same as x^-1 mod y
	d = modInverse(e, r);

	printf("\npublic key is %ld and %ld\n", n, e);
	printf("private key is %ld\n", d);
	printf("Enter the text to encode (ctrl+D) when done: ");
	clear();
	int i;
	while ((textToEncode[i] = getchar())!=EOF) {
		i++;
	}
	textToEncode[strlen(textToEncode)-1] = '\0';
	char newString[strlen(textToEncode)];
	strcpy(newString, textToEncode);
	encoded = encrypt(newString, n , e);
	decoded = decrypt(encoded, d, n);

	printf("Done.\n");
	return 0;
}

long* encrypt(char const *textToProcess, long const n, long const e){
	long i;
	int max = 255;
	static long encoded[255];
	printf("%s\nencrypted is:\n", textToProcess);
	for (i = 0; i<max && textToProcess[i]!='\0' ; i++) {
		long c = (long) textToProcess[i];
		c = modpow(c, e, n);
		encoded[i] = c;
	}
	printf("printLong (1)\n");
	printLong(encoded);
	FILE *fp = fopen("data.enc", "wb");
	fwrite(encoded, sizeof(long), 255, fp);
	fclose(fp);
	return encoded;
}//
char* decrypt(const long *encoded, long const d, long const n) {
	long i;
	int max = 255;//sizeof(encoded) / sizeof(encoded[0]);
	long _d;
	if (d) {
		_d = d;
	} else {
		printf("\nplease enter your private key, d:");
		scanf("%ld", &_d);
	}
	static char decodedText[255];
	
	printf("printLong (2)\n");
	printLong(encoded);
	printf("\ngetting encoded from file\n");
	FILE *fp = fopen("data.enc", "rb");
	long buffer[255];
	fseek(fp, SEEK_SET, 0);
	long bytes = fread(buffer, sizeof(long), 255, fp);
	fclose(fp);
	printf("read %ld bytes\n", bytes);
	printLong(buffer);

	printf("\ndecrypted is:\n");
	for (i = 0; i<max && buffer[i]!=EOF ; i++) {
		long c = buffer[i];
		c = modpow(c, _d, n);
		decodedText[i] = c;
		printf("%c", decodedText[i]);
	}
	printf("\n");

	fp = fopen("data.dec", "w");
	fwrite(decodedText, 255, 1, fp);
	fclose(fp);
	return decodedText;
}

void printLong(const long longToPrint[255]) {
	for (int i=0;i < 255 && longToPrint[i]!=EOF;i++) {
	    printf("%ld ",longToPrint[i]);
	}

};
/*
long *getFactorsForNumber(long n) {
	long i;
	printf("Factors of %ld are:\n", n);
	for(i=1;i<=n;++i) {
		if(n%i!=0) {
			printf("%ld%c",i, i % 10 ? '\n':' ');
		}
	}
	return 0;
}*/

//template <typename T>
long modpow(long base, long exponent, long modulus) {
  base %= modulus;
  long result = 1;
  while (exponent > 0) {
    if (exponent & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exponent >>= 1;
  }
  return result;
}

void clear(void) {
	while ( getchar() != '\n' );
}

long modInverse(long a, long m)
{
    long x, y;
    long g = gcdExtended(a, m, &x, &y);
    if (g != 1)
        return -1;
    else
    {
        // m is added to handle negative x
        long res = (x % m + m) % m;
        return res;
    }
    
}
 
// C function for extended Euclidean Algorithm
long gcdExtended(long a, long b, long *x, long *y)
{
    // Base Case
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }
 
    long x1, y1; // To store results of recursive call
    long g = gcdExtended(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b/a) * x1;
    *y = x1;
 
    return g;
}

long gcd( long a, long b )
{
  long c;
  while ( a != 0 ) {
     c = a;
     a = b%a;
     b = c;
  }
  return b;
}
