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


int main(int argumentCount, char** argumentValues) {
    Gf2Polynomial prime = 2;

    initializePrimeList(PRIME_FILE_PREFIX, PRIME_FILE_OPEN_FOR_READING);

    do {
        printf("%" PRIx64 "\n",prime);
        prime = findNextPrime(prime);
    } while (prime != 0);

    terminatePrimeList();

    return 0;
}
