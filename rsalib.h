
void encryptToFile (char const *textToEncrypt, unsigned long const n, unsigned long const e, const char *fileName);
void decryptFromFileToFile(unsigned long const d, unsigned long const n, const char *fileName, const char *fileNameDest);
unsigned long modpow(unsigned long base,unsigned long exponent,unsigned long modulus);
unsigned long modInverse (unsigned long, unsigned long);
unsigned long gcd (unsigned long, unsigned long);
unsigned long gcdExtended (unsigned long, unsigned long, unsigned long*, unsigned long*);
char *inputString (FILE* fp, size_t size, char terminatingCharacter);
unsigned long getUnsignedLongFromStream(FILE *fp);
int validateN(unsigned long n);
unsigned long getN(unsigned long p, unsigned long q);
unsigned long getR(unsigned long p, unsigned long q);
void getEandD(unsigned long r, unsigned long *e, unsigned long *d);


