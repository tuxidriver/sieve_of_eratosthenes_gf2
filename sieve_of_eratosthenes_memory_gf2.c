/*-*-c++-*-*************************************************************************************************************
* Copyright (C) 2015 by Paul H. Smith
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.  If not, see
* <http://www.gnu.org/licenses/>.
********************************************************************************************************************//**
* \file
* \brief Locates prime polynomials in a GF(2) field.
*
* This program locates prime polynomials in a GF(2) field.  For performance reasons, the program is designed to operate
* across multiple concurrent threads.
***********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "compiler.h"
#include "gf2.h"

#include "parameters.h"


/*******************************************************************************************************************//**
* \brief Indicates the program version.
*
* You can use this define to specify the program version.
***********************************************************************************************************************/
#define VERSION ("1.0")

/*******************************************************************************************************************//**
* \brief Indicates the number of primes tracked per pool.
*
* You can use this define to determine the number of primes tracked per pool.
***********************************************************************************************************************/
#define POOL_SIZE (32)

/*******************************************************************************************************************//**
* \brief Indicates the number of prime list pools.
*
* You can use this define to determine the number of pools in the prime list.
***********************************************************************************************************************/
#define NUMBER_POOLS ((MAXIMUM_PRIME+POOL_SIZE-1ULL)/POOL_SIZE)

#if (POOL_SIZE == 32)

    typedef uint32_t PoolEntry;

#else

    typedef uint64_t PoolEntry;

#endif

PoolEntry primeList[NUMBER_POOLS];

/*******************************************************************************************************************//**
* \brief Initializes the prime list.
*
* You can use this function to initialize the prime list.
***********************************************************************************************************************/
static void initializePrimeList(void) {
    memset(primeList, 0xFF, sizeof(primeList));
}

/*******************************************************************************************************************//**
* \brief Marks a value as composite (not prime).
*
* You can use this function to mark an entry as a composite value.
*
* \param[in] value The value to mark as a composite value.
***********************************************************************************************************************/
static void markComposite(Gf2Polynomial const value) {
    unsigned long index  = value / POOL_SIZE;
    unsigned      offset = value % POOL_SIZE;
    PoolEntry     mask   = ~((PoolEntry) 1 << offset);

    primeList[index] &= mask;
}

/*******************************************************************************************************************//**
* \brief Locates the next known prime value.
*
* You can use this function to quickly locate the next prime value in the prime list.
*
* \param[in] iterator The iterator being used to traverse the prime list.
*
* \return Returns the next known prime.  A value of 0 is returned if no new prime could be located.
***********************************************************************************************************************/
static Gf2Polynomial findNextPrime(Gf2Polynomial const currentPrime) {
    unsigned long index  = (currentPrime+1) / POOL_SIZE;
    Gf2Polynomial result = 0;

    if (index < NUMBER_POOLS) {
        unsigned      offset = (currentPrime+1) % POOL_SIZE;
        PoolEntry     mask   = ((PoolEntry) -1) << offset;
        PoolEntry     entry  = primeList[index] & mask;

        if (entry == 0) {
            do {
                ++index;
            } while (index < NUMBER_POOLS && primeList[index] == 0);

            if (index < NUMBER_POOLS) {
                entry = primeList[index];
            }
        }

        if (index < NUMBER_POOLS) {
            #if (POOL_SIZE == 32)

                offset = countTrailingZeros32(entry);

            #else

                offset = countTrailingZeros64(entry);

            #endif

            result = POOL_SIZE*index + offset;
        }
    }

    return result;
}


int main(int argumentCount, char** argumentValues) {
    int           done = 0;
    Gf2Polynomial prime = 2;
    Gf2Polynomial product;

    initializePrimeList();
    markComposite(0);
    markComposite(1);

    do {
        product = gf2Multiply(prime, prime);

        if (product > MAXIMUM_PRIME) {
            done = 1;
        } else {
            Gf2Polynomial q = prime;
            do {
                markComposite(product);
                ++q;
                product = gf2Multiply(prime, q);
            } while (product <= MAXIMUM_PRIME);
        }

        printf("* 0x%016LLX\n",prime);

        prime = findNextPrime(prime);
    } while (!done && prime != 0);

    while (prime != 0) {
        printf("- 0x%016LLX\n",prime);
        prime = findNextPrime(prime);
    }

    return 0;
}
