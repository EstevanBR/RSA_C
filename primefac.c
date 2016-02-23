#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// A function to print all prime factors of a given number n
long getLargestPrimeFactor(long n, long *primes)
{
    //primes = malloc(2*sizeof(long));
    long numberOfPrimeFactors;
    // Print the number of 2s that divide n
    while (n%2 == 0)
    {
        primes[numberOfPrimeFactors] = (long) 2;
        printf("prime: %ld numberOfPrimeFactors: %ld\n", (long) 2, numberOfPrimeFactors++);
        n = n/2;
    }
 
    // n must be odd at this point.  So we can skip one element (Note i = i +2)
    for (long i = 3; i <= sqrt(n); i = i+2)
    {
        // While i divides n, print i and divide n
        while (n%i == 0)
        {
            primes[numberOfPrimeFactors] = i;
            printf("prime: %ld numberOfPrimeFactors: %ld\n", i, numberOfPrimeFactors++);
            n = n/i;
        }
    }
 
    // This condition is to handle the case whien n is a prime number
    // greater than 2
    if (n > 2) {
        primes[numberOfPrimeFactors] = n;
        printf("prime: %ld numberOfPrimeFactors: %ld \n", n, numberOfPrimeFactors++);
    }
    printf("\n***numberOfPrimeFactors: %ld***\n", numberOfPrimeFactors);
    printf("***primes[0]:%ldprimes[1]:%ld***\n\n", primes[0], primes[1]);
    return numberOfPrimeFactors;
}