#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// A function to print all prime factors of a given number n
long getLargestPrimeFactor(long n, long *primes)
{
    long numberOfPrimeFactors;
    while (n%2 == 0)
    {
        primes[numberOfPrimeFactors] = (long) 2;
        numberOfPrimeFactors++;
        n = n/2;
    }
 
    // n must be odd at this point.  So we can skip one element (Note i = i +2)
    for (long i = 3; i <= sqrt(n); i = i+2)
    {
        // While i divides n, print i and divide n
        while (n%i == 0)
        {
            primes[numberOfPrimeFactors] = i;
            numberOfPrimeFactors++;
            n = n/i;
        }
    }
 
    // This condition is to handle the case whien n is a prime number
    // greater than 2
    if (n > 2) {
        primes[numberOfPrimeFactors] = n;
        numberOfPrimeFactors++;
    }
    return numberOfPrimeFactors;
}