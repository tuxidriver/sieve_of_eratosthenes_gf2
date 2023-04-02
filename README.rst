Sieve Of Eratosthenes - GF(2)
=============================
You can use this small program to generate a list of prime polynomials in a
GF(2) field.  The program is designed to cache data on disk/SSD during
operation and to perform the calculations across multiple concurrent threads.

You configure the program by setting values in the file ``parameters.h``.

When done, you can use the ``list_primes_gf2`` program to list the resulting
primes.

The MSB represents the coeffient of the highest order term.  The LSB
represents the coefficient of the lowest order term (x^0).
