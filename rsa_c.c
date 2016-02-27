#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define BLURB "\nCRYPT:\n\tlate Middle English\n\t(in the sense ‘cavern’):\n\tfrom Latin crypta,\n\tfrom Greek kruptē\n\t‘a vault,’ from kruptos ‘hidden.’\n\n"

void encrypt(char const *textToProcess, long const p, long const q);
void decrypt(const long *encoded, long const p, long const q);
long modpow(long base,long exponent,long modulus);
void clear(void);
long modInverse(long, long);
long gcd(long, long);
long gcdExtended(long, long, long*, long*);
char *inputString(FILE* fp, size_t size, char terminatingCharacter);
long getLongFromFile(FILE *fp);

int main(int argc, char const *argv[]) {
	printf(BLURB);
	long p,q,n,d,r,e,k;
	long publicKey[2];
	long primesForK[2];
	long *encoded;
	char *decoded;
	printf("Please enter the p (needs to be prime): ");
	p = getLongFromFile(stdin);
	if (!p) {
		printf("\007ERROR\n\tp was not read from input properly");
		return 0;
	}
	printf("Please enter the q (needs to be prime): ");
	q = getLongFromFile(stdin);
	if (!q) {
		printf("\007ERROR\n\tq was not read from input properly");
		return 0;
	}
	n = p * q;
	r = (p-1) * (q-1);
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

	printf("\n***public  key is e %ld:n %ld\n", e, n);
	printf(  "***private key is d %ld\n", d);
	char choice[8];
	do {
		printf("Would you like to encrypt or decrypt or quit?\nEnter Choice:_______\b\b\b\b\b\b\b");
		strcpy(choice,inputString(stdin, sizeof(char),'\n'));
		for(int i; choice[i];i++) {
			choice[i] = tolower(choice[i]);
		}
		if (strncmp(choice, "encrypt", strlen(choice)) == 0){
			printf("Please enter text to encrypt, terminate with CTRL+D\n");
			char *textToEncrypt = inputString(stdin,10,'\0');
			encrypt(textToEncrypt, n , e);
			break;
		} else if (strncmp(choice, "decrypt", strlen(choice)) == 0) {
			//printf("Please enter text to decrypt, terminate with CTRL+D");
			decrypt(encoded, 0, 0);
			break;
		}
		if ('q' == tolower(choice[0])) {
			break;
		}
	} while (1);
	printf("\nDone.\n");
	return 0;
}

/*FUNCTION DEFINITIONS*/

void encrypt(char const *textToProcess, long const n, long const e){
	long max = (long) strlen(textToProcess);
	printf("max is %ld, n is %ld, e is %ld", max, n, e);
	long encoded[max];
	long _n = n;
	long _e = e;
	if (!n) {
		printf("please enter n");
		scanf("%ld", &_n);		
	}
	if (!e) {
		printf("please enter e");
		scanf("%ld", &_e);
	}
	long i;
	for (i = 0; i < max; i++) {
		long c = (long) textToProcess[i];
		c = modpow(c, _e, _n);
		encoded[i] = c;
	}
	FILE *fp = fopen("data.enc", "wb");
	fwrite(encoded, sizeof(long), max, fp);
	fclose(fp);
}
void decrypt(const long *encoded, long const d, long const n) {
	long _d = d;
	long _n = n;
	if (!d) {
		printf("\nplease enter your private key, d:");
		scanf("%ld", &_d);
	}
	if (!n) {
		printf("\nplease enter the public key, n:");
		scanf("%ld", &_n);
	}
	FILE *fp = fopen("data.enc", "rb");
	fseek(fp, 0L, SEEK_END);
	long max = ftell(fp) / sizeof(long);
	long buffer[max];
	printf("max is %ld", max);
	fseek(fp, SEEK_SET, 0);
	long bytes = fread(buffer, sizeof(long), max, fp);
	fclose(fp);
	
	char decodedText[max];
	printf("\ndecrypted is:\n");
	for (long i = 0; i < max;i++) {
		long c = buffer[i];
		c = modpow(c, _d, _n);
		decodedText[i] = c;
		printf("%c", decodedText[i]);
	}

	fp = fopen("data.dec", "w");
	fwrite(decodedText, max, 1, fp);
	fclose(fp);
}

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

long getLongFromFile(FILE *fp) {
	char *aString = inputString(fp, 10, '\n');
	long aLong;
	sscanf(aString,"%ld", &aLong);
	return aLong;
};

//http://stackoverflow.com/a/16871702
char *inputString(FILE* fp, size_t size, char terminatingCharacter){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != terminatingCharacter) {
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}
