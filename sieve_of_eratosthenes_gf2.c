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
#include <inttypes.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "compiler.h"
#include "gf2.h"
#include "prime_list.h"

#include "parameters.h"


/*******************************************************************************************************************//**
* \brief Indicates the program version.
*
* You can use this define to specify the program version.
***********************************************************************************************************************/
#define VERSION ("1.0")


Gf2Polynomial prime;
Gf2Polynomial q;
int           done;
pthread_t     monitorThreadData;


void* monitorThread(void* dummy) {
    time_t startTime = time(NULL);

    do {
        unsigned pulse = 60;
        time_t   elapsedTime;
        double   fraction;

        do {
            sleep(1);
            --pulse;
        } while (!done && pulse > 0);

        elapsedTime = time(NULL) - startTime;
        fraction    = (1.0 * (q - prime)) / (MAXIMUM_PRIME - prime);

        printf("%16d\t%" PRIx64 "\t%" PRIx64 "\t%lf\n", elapsedTime, prime, q, fraction);
    } while (!done);

    pthread_exit(NULL);
}


int main(int argumentCount, char** argumentValues) {
    void* dummyResult;

    done  = 0;
    prime = 3;
    q     = 3;

    initializePrimeList(PRIME_FILE_PREFIX, PRIME_FILE_CREATE_NEW);
    markComposite(0);
    markComposite(1);

    pthread_create(&monitorThreadData, NULL, &monitorThread, NULL);

    do {
        Gf2Polynomial product = gf2Multiply(prime, prime);;

        if (product > MAXIMUM_PRIME) {
            done = 1;
        } else {
            do {
                markComposite(product);
                q += 2;
                product = gf2Multiply(prime, q);
            } while (product <= MAXIMUM_PRIME);
        }

        prime = findNextPrime(prime);
        q = prime;
    } while (!done && prime != 0);

    terminatePrimeList();

    pthread_join(monitorThreadData, &dummyResult);

    return 0;
}
