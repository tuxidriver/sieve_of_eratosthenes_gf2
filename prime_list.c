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
#include <assert.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "compiler.h"
#include "gf2.h"

#include "parameters.h"
#include "prime_list.h"


#define NUMBER_PUDDLES_PER_POOL (POOL_SIZE_IN_BYTES / (PUDDLE_SIZE / 8))
#define NUMBER_BITS (MAXIMUM_PRIME >> 1)
#define NUMBER_PUDDLES ((NUMBER_BITS + PUDDLE_SIZE - 1) / PUDDLE_SIZE)
#define NUMBER_POOLS ((NUMBER_PUDDLES + NUMBER_PUDDLES_PER_POOL - 1) / NUMBER_PUDDLES_PER_POOL)
#define CREATE_FLAGS (O_CREAT | O_TRUNC | O_APPEND | O_RDWR)
#define OPEN_FLAGS (O_RDONLY)
#define MODES (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)


#if (PUDDLE_SIZE == 32)

    typedef uint32_t PuddleEntry;

#elif (PUDDLE_SIZE == 64)

    typedef uint64_t PuddleEntry;

#else

    #error "Invalid puddle size."

#endif


PuddleEntry   inMemoryPool[NUMBER_PUDDLES_PER_POOL];
unsigned long inMemoryPoolIndex;
int           inMemoryPoolIsDirty;
char*         primeFilePrefix;
char*         primeFilename;


static void flushInMemoryPool(void) {
    if (inMemoryPoolIsDirty) {
        int     primeFile;
        ssize_t bytesWritten;

        sprintf(primeFilename, "%s%05d", primeFilePrefix, inMemoryPoolIndex);

        primeFile = open(primeFilename, CREATE_FLAGS, MODES);
        assert(primeFile >= 0);

        bytesWritten = write(primeFile, inMemoryPool, sizeof(inMemoryPool));
        assert(bytesWritten == sizeof(inMemoryPool));

        close(primeFile);

        inMemoryPoolIsDirty = 0;
    }
}


static void checkIfCached(unsigned long const newIndex) {
    if (newIndex != inMemoryPoolIndex) {
        int     primeFile;
        ssize_t bytesRead;

        flushInMemoryPool();

        sprintf(primeFilename, "%s%05d", primeFilePrefix, newIndex);

        primeFile = open(primeFilename, OPEN_FLAGS, MODES);
        assert(primeFile >= 0);

        bytesRead = read(primeFile, inMemoryPool, sizeof(inMemoryPool));
        assert(bytesRead == sizeof(inMemoryPool));

        close(primeFile);

        inMemoryPoolIndex = newIndex;
    }
}


void initializePrimeList(char const* const filePrefix, PrimeListOpenMode const openMode) {
    primeFilePrefix = strdup(filePrefix);
    assert(primeFilePrefix != NULL);

    primeFilename = malloc(strlen(primeFilePrefix)+16);
    assert(primeFilename != NULL);

    if (openMode == PRIME_FILE_CREATE_NEW) {
        unsigned long poolIndex;

        memset(inMemoryPool, 0xFF, sizeof(inMemoryPool));
        inMemoryPoolIndex = 0;
        inMemoryPoolIsDirty = 0;

        for (poolIndex=0 ; poolIndex < NUMBER_POOLS ; ++poolIndex) {
            int     primeFile;
            ssize_t bytesWritten;

            sprintf(primeFilename, "%s%05d", primeFilePrefix, poolIndex);
            printf("Creating %s\n", primeFilename);

            primeFile = open(primeFilename, CREATE_FLAGS, MODES);
            assert(primeFile >= 0);

            bytesWritten = write(primeFile, inMemoryPool, sizeof(inMemoryPool));
            assert(bytesWritten == sizeof(inMemoryPool));

            close(primeFile);
        }
    } else {
        inMemoryPoolIndex = (unsigned long) -1;
        inMemoryPoolIsDirty = 0;

        checkIfCached(0);
    }
}


void terminatePrimeList(void) {
    int exitStatus;

    flushInMemoryPool();

    free(primeFilePrefix);
    primeFilePrefix = NULL;

    free(primeFilename);
    primeFilename = NULL;
}


void markComposite(Gf2Polynomial const value) {
    if (value & 1) {
        Gf2Polynomial      v           = value >> 1;
        unsigned long long puddleIndex = v / PUDDLE_SIZE;
        unsigned           offset      = v % PUDDLE_SIZE;
        unsigned long      poolIndex   = puddleIndex / NUMBER_PUDDLES_PER_POOL;
        unsigned long      poolOffset  = puddleIndex % NUMBER_PUDDLES_PER_POOL;
        PuddleEntry        mask        = ~((PuddleEntry) 1 << offset);

        checkIfCached(poolIndex);
        inMemoryPool[poolOffset] &= mask;

        inMemoryPoolIsDirty = 1;
    }
}


int isPrime(Gf2Polynomial const value) {
    if (value & 1) {
        Gf2Polynomial      v           = value >> 1;
        unsigned long long puddleIndex = v / PUDDLE_SIZE;
        unsigned           offset      = v % PUDDLE_SIZE;
        unsigned long      poolIndex   = puddleIndex / NUMBER_PUDDLES_PER_POOL;
        unsigned long      poolOffset  = puddleIndex % NUMBER_PUDDLES_PER_POOL;
        PuddleEntry        mask        = ~((PuddleEntry) 1 << offset);

        checkIfCached(poolIndex);

        return (inMemoryPool[poolOffset] & mask) != 0;
    } else {
        return 0;
    }
}


Gf2Polynomial findNextPrime(Gf2Polynomial const currentPrime) {
    Gf2Polynomial      cp          = currentPrime >> 1;
    unsigned long long puddleIndex = (cp+1) / PUDDLE_SIZE;
    unsigned long      poolIndex   = puddleIndex / NUMBER_PUDDLES_PER_POOL;
    unsigned long      poolOffset  = puddleIndex % NUMBER_PUDDLES_PER_POOL;
    Gf2Polynomial      result      = 0;

    if (puddleIndex < NUMBER_PUDDLES) {
        unsigned        offset = (cp+1) % PUDDLE_SIZE;
        PuddleEntry     mask   = ((PuddleEntry) -1) << offset;
        PuddleEntry     entry;

        checkIfCached(poolIndex);
        entry = inMemoryPool[poolOffset] & mask;

        if (entry == 0) {
            do {
                do {
                    ++poolOffset;
                    if (poolOffset < NUMBER_PUDDLES_PER_POOL) {
                        entry = inMemoryPool[poolOffset];
                    }
                } while (poolOffset < NUMBER_PUDDLES_PER_POOL && entry == 0);

                if (poolOffset >= NUMBER_PUDDLES_PER_POOL) {
                    ++poolIndex;
                    poolOffset = 0;

                    if (poolIndex < NUMBER_POOLS) {
                        checkIfCached(poolIndex);
                        entry = inMemoryPool[0];
                    }
                }
            } while (poolIndex < NUMBER_POOLS && entry == 0);
        }

        if (poolIndex < NUMBER_POOLS) {
            #if (PUDDLE_SIZE == 32)

                offset = countTrailingZeros32(entry);

            #else

                offset = countTrailingZeros64(entry);

            #endif

            result = 2 * (((NUMBER_PUDDLES_PER_POOL * poolIndex) + poolOffset) * PUDDLE_SIZE + offset) + 1;
        }
    }

    return result;
}
