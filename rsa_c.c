#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "primefac.h"

long* encrypt(char const textToProcess[], long const p, long const q);
char* decrypt(const long encoded[], long const p, long const q);
long* getFactorsForNumber(long n);
long modpow(long base,long exponent,long modulus);
void clear(void);
void printLong(const long *longToPrint);
int main(int argc, char const *argv[])

{
	long p,q,n,d,r,e,k;
	long publicKey[2];
	long primesForK[2];
	/*
p = prime 1;
q = prime 2;
n = p * q;
r = phi of n

e = exponent, greater than 1, less than phi of n, and not a factor of p or q
d = private key
public key = n and e
	*/
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
	
	for (long i = 40; i > 0; i--) {
		k = (i * r) + 1;
		if (getLargestPrimeFactor(k, primesForK) == 2) {
			e = primesForK[0];
			d = primesForK[1];
			break;
		}
	}
	printf("public key is %ld and %ld\n", n, e);
	printf("private key is %ld\n", d);
	printf("Enter the text to encode (ctrl+D) when done: ");
	clear();
	int i;
	while ((textToEncode[i] = getchar())!=EOF) {
		i++;
	}
	textToEncode[strlen(textToEncode)-1] = '\0';
	encoded = encrypt(textToEncode, n , e);
	decoded = decrypt(encoded, d, n);

	printf("Done.\n");
	return 0;
}

long* encrypt(char const textToProcess[255], long const n, long const e){
	long i;
	long max = strlen(textToProcess);
	static long encoded[255];
	printf("%s\nencrypted is:\n", textToProcess);
	for (i = 0; i<max ; i++) {
		long c = textToProcess[i];
		c = modpow(c, e, n);
		encoded[i] = c;
	}
	printLong(encoded);
	return encoded;
}
char* decrypt(const long encoded[255], long const d, long const n) {
	long i;
	long max = 255;
	long _d;
	if (d) {
		_d = d;
	} else {
		printf("\nplease enter your private key, d:");
		scanf("%ld", &_d);
	}
	static char decodedText[255];
	
	printLong(encoded);
	printf("\ndecrypted is:\n");
	for (i = 0; i<max && encoded[i]!='\0' ; i++) {
		long c = encoded[i];
		c = modpow(c, _d, n);
		decodedText[i] = c;
		printf("%c", decodedText[i]);
	}
	printf("\n");
	return decodedText;
}

void printLong(const long longToPrint[255]) {
	for (int i=0;i < 255 && longToPrint[i]!='\0';i++) {
	    printf("%ld",longToPrint[i]);
	}

};

long *getFactorsForNumber(long n) {
	long i;
	printf("Factors of %ld are:\n", n);
	for(i=1;i<=n;++i) {
		if(n%i!=0) {
			printf("%ld%c",i, i % 10 ? '\n':' ');
		}
	}
	return 0;
}

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