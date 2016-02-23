#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "primefac.h"

long* encrypt(char const textToProcess[], long const p, long const q);
char* decrypt(const long encoded[], long const p, long const q);
long* getFactorsForNumber(long n);
long modInverse(long a, long m);
long gcd(long a, long b);
long gcdExtended(long a, long b, long *x, long *y);
long modpow(long base,long exponent,long modulus);
char power(long n, long e);
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
	//printf("Please enter the p: (needs to be prime)");
	//scanf("%ld", &p);
	p = 37;
	if (!p) {
		return 0;
	}
	//printf("Please enter the q: (needs to be prime)");
	//scanf("%ld", &q);
	q = 89;
	if (!q) {
		return 0;
	}
	n = p * q;
	r = (p-1) * (q-1);
	/*
	  p   = 37
	  q   = 39

      e   = 101
      d   = 941
      
      N   = 3293
      r   = 3168
      
      e*d = 95041
e*d mod r = 1
e and N are relatively prime
d and N are relatively prime
e and r are relatively prime
d and r are relatively prime



	*/
	printf("p is %ld, q is %ld, r (phi of n): %ld\n", p, q, r);
	for (long i = 40; i > 0; i--) {
		k = (i * r) + 1;
		printf("for k %ld\n", k);
		if (getLargestPrimeFactor(k, primesForK) == 2) {
			e = primesForK[0];
			d = primesForK[1];
			printf("\nk is %ld e is %ld d is %ld\n", k, e, d);
			break;
		}
	}

	//try putting all of that in 2 lines^
	//d = modInverse(e , r);

	printf("private key is %ld\n", d);
	printf("public key is %ld and %ld\n", n, e);
	printf("Enter the text to encode: ");
	scanf("%s", textToEncode);
	encoded = encrypt(textToEncode, n , e);
	decoded = decrypt(encoded, d, n);

	printf("Done.\n");
	return 0;
}

long* encrypt(char const textToProcess[], long const n, long const e){
	long i;
	long max = strlen(textToProcess);
	printf("%s strlen is %ld\n", textToProcess, max);
	static long encodedText[255];
	for (i = 0; i<max ; i++) {
		long c = textToProcess[i];
		c = modpow(c, e, n);
		encodedText[i] = c;
		printf("%d encrypted is %ld\n", textToProcess[i], encodedText[i]);
	}
	return encodedText;
}
char* decrypt(const long encoded[255], long const d, long const n) {
	long i;
	long max = 255;
	static char decodedText[255];
	for (i = 0; i<max, encoded[i]!=NULL ; i++) {
		long c = encoded[i];
		decodedText[i] = c % n;
		printf("%ld decrypted is %d\n", encoded[i], decodedText[i]);
	}
	return decodedText;
}

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

// Function to find modulo inverse of a
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
    long gcd = gcdExtended(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b/a) * x1;
    *y = x1;
 
    return gcd;
}

long gcd ( long a, long b )
{
  long c;
  while ( a != 0 ) {
     c = a;
     a = b%a;
     b = c;
  }
  return b;
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
char power(long n, long e) {
	// if (e == 0) {
	// 	return 0;
	// }
	int i = 0;
	printf("%ld to the power of %ld is:", n, e);
	for (i = 0; i < e; i++) {
		n *= n;
	}
	printf("%ld\n", n);
	return (char) n;
}



