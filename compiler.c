/*-*-c-*-***************************************************************************************************************
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
* \brief File that wraps or emulates common compiler specific features.
*
* This file implements work-arounds or emulation layers for many useful compiler specific features.
***********************************************************************************************************************/

#include <stdlib.h>
#include <stdint.h>

#include "compiler.h"


#if (!defined(__GNUC__))

    unsigned countLeadingZeros32(unsigned long const e) {
        unsigned offset = 0;

        if (e == 0) {
            offset = 32;
        } else {
            unsigned d    = 32/2;
            uint32_t mask = ((uint32_t) 1 << d)-1 << d;

            do {
                if ((e & (mask >> offset)) == 0) {
                    offset += d;
                }

                d >>= 1;
                mask >>= d;
            } while (d != 0);
        }

        return offset;
    }


    unsigned countLeadingZeros64(unsigned long long const e) {
        unsigned offset = 0;

        if (e == 0) {
            offset = 64;
        } else {
            unsigned d    = 64/2;
            uint64_t mask = ((uint64_t) 1 << d)-1 << d;

            do {
                if ((e & (mask >> offset)) == 0) {
                    offset += d;
                }

                d >>= 1;
                mask >>= d;
            } while (d != 0);
        }

        return offset;
    }


    unsigned countTrailingZeros32(unsigned long const e) {
        unsigned offset = 0;

        if (e == 0) {
            offset = 32;
        } else {
            unsigned d    = 32/2;
            uint32_t mask = ((uint32_t) 1 << d)-1;

            do {
                if ((e & (mask << offset)) == 0) {
                    offset += d;
                }

                d >>= 1;
                mask >>= d;
            } while (d != 0);
        }

        return offset;
    }


    unsigned countTrailingZeros32(unsigned long long const e) {
        unsigned offset = 0;

        if (e == 0) {
            offset = 64;
        } else {
            unsigned d    = 64/2;
            uint64_t mask = ((uint64_t) 1 << d)-1;

            do {
                if ((e & (mask << offset)) == 0) {
                    offset += d;
                }

                d >>= 1;
                mask >>= d;
            } while (d != 0);
        }

        return offset;
    }

#endif
