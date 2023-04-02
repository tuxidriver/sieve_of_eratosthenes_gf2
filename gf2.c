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
* \brief Implements functions to perform math against polynomials in a GF(2) field.
*
* This file implements functions used to perform math functions against prime polynomials in a GF(2) field.
***********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "compiler.h"
#include "debug.h"
#include "gf2.h"


Gf2Polynomial gf2Multiply(Gf2Polynomial const p1, Gf2Polynomial const p2) {
    Gf2Polynomial product = 0;
    Gf2Polynomial s1      = p1;
    Gf2Polynomial s2      = p2;

    while (s2) {
        if (s2 & 1) {
            product ^= s1;
        }

        s2 >>= 1;
        s1 <<= 1;
    }

    return product;
}


static unsigned mantissaSizeInBits(uint64_t const m) {
    return 64-countLeadingZeros64(m);
}


Gf2Polynomial gf2Divide(Gf2Polynomial const dividend, Gf2Polynomial const divisor, Gf2Polynomial* optionalRemainder) {
    Gf2Polynomial remainder     = dividend;
    Gf2Polynomial quotient      = 0;
    unsigned      divisorSize   = mantissaSizeInBits(divisor);
    unsigned      remainderSize = mantissaSizeInBits(remainder);

    while (remainderSize >= divisorSize) {
        unsigned b = remainderSize - divisorSize;

        quotient |= 1 << b;
        remainder ^= divisor << b;

        remainderSize = mantissaSizeInBits(remainder);
    }

    if (optionalRemainder != NULL) {
        *optionalRemainder = remainder;
    }

    return quotient;
}


Gf2Polynomial gf2Remainder(Gf2Polynomial const dividend, Gf2Polynomial const divisor) {
    Gf2Polynomial remainder     = dividend;
    unsigned      divisorSize   = mantissaSizeInBits(divisor);
    unsigned      remainderSize = mantissaSizeInBits(remainder);

    while (remainderSize >= divisorSize) {
        remainder ^= divisor << (remainderSize - divisorSize);
        remainderSize = mantissaSizeInBits(remainder);
    }

    return remainder;
}
