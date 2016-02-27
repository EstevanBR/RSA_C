#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define BLURB "\nCRYPT:\n\tlate Middle English\n\t(in the sense ‘cavern’):\n\tfrom Latin crypta,\n\tfrom Greek kruptē\n\t‘a vault,’ from kruptos ‘hidden.’\n\n"

void encryptToFile (char const *textToProcess, unsigned long const p, unsigned long const q, const char *fileName);
void decryptFromFileToFile(unsigned long const p, unsigned long const q, const char *fileName, const char *fileNameDest);
unsigned long modpow(unsigned long base,unsigned long exponent,unsigned long modulus);
void clear(void);
unsigned long modInverse(unsigned long, unsigned long);
unsigned long gcd(unsigned long, unsigned long);
unsigned long gcdExtended(unsigned long, unsigned long, unsigned long*, unsigned long*);
char *inputString(FILE* fp, size_t size, char terminatingCharacter);
unsigned long getUnsignedLongFromStream(FILE *fp);

int main(int argc, char const *argv[]) {
	for (int i = 1; i < argc; i++) {
		printf("%d: %s ", i, argv[i]);
	}
	printf(BLURB);
	static unsigned long p,q,n,d,r,e,k;
	static unsigned long publicKey[2];
	static unsigned long primesForK[2];
	static unsigned long *encoded;
	static char *decoded;

	printf("Please enter the p (needs to be prime): ");
	p = getUnsignedLongFromStream(stdin);
	if (!p) {
		printf("ERROR p was not read from input properly\007");
		return -1;
	}
	printf("Please enter the q (needs to be prime): ");
	q = getUnsignedLongFromStream(stdin);
	if (!q) {
		printf("ERROR q was not read from input properly\007");
		return -1;
	}
	n = p * q;
	r = (p-1) * (q-1);
	// get a valid e
	for (unsigned long i = 3; i < r; i++){
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

	printf("\n***public  key is e %lu\nn %lu\n", e, n);
	printf(  "***private key is d %lu\n", d);
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
			encryptToFile(textToEncrypt, n , e, "data.enc");
			break;
		} else if (strncmp(choice, "decrypt", strlen(choice)) == 0) {
			decryptFromFileToFile(d, n, "data.enc", "data.dec");
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

void encryptToFile(char const *textToProcess, unsigned long const n, unsigned long const e, const char *fileName){
	unsigned long max = (unsigned long) strlen(textToProcess);
	//printf("\nmax is %lu, n is %lu, e is %lu", max, n, e);
	unsigned long encoded[max];
	unsigned long _n = n;
	unsigned long _e = e;
	if (!n) {
		printf("please enter n");
		scanf("%lu", &_n);		
	}
	if (!e) {
		printf("please enter e");
		scanf("%lu", &_e);
	}
	unsigned long i;
	for (i = 0; i < max; i++) {
		unsigned long c = (unsigned long) textToProcess[i];
		c = modpow(c, _e, _n);
		encoded[i] = c;
	}
	FILE *fp = fopen(fileName, "wb");
	fwrite(encoded, sizeof(unsigned long), max, fp);
	fclose(fp);
}
void decryptFromFileToFile(unsigned long const d, unsigned long const n, const char *fileNameSource, const char *fileNameDest) {
	unsigned long _d = d;
	unsigned long _n = n;
	if (!d) {
		printf("\nplease enter your private key, d:");
		scanf("%lu", &_d);
	}
	if (!n) {
		printf("\nplease enter the public key, n:");
		scanf("%lu", &_n);
	}
	FILE *fp = fopen(fileNameSource, "rb");
	if (fp) {
		fseek(fp, 0L, SEEK_END);
		unsigned long max = ftell(fp) / sizeof(unsigned long);
		unsigned long buffer[max];
		printf("max is %lu", max);
		fseek(fp, SEEK_SET, 0);
		unsigned long bytes = fread(buffer, sizeof(unsigned long), max, fp);
		fclose(fp);
		
		char decodedText[max];
		printf("\ndecrypted is:\n");
		for (unsigned long i = 0; i < max;i++) {
			unsigned long c = buffer[i];
			c = modpow(c, _d, _n);
			decodedText[i] = c;
			printf("%c", decodedText[i]);
		}
		fp = fopen(fileNameDest, "w");
		if (fp) {
			fwrite(decodedText, max, 1, fp);
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
