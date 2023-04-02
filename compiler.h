/*-*-c++-*-*************************************************************************************************************
* Copyright (C) 2015 by Paul H. Smith
*
*  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
*  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
*  version.
*
*  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*  details.
*
*  You should have received a copy of the GNU General Public License along with this program.  If not, see
*  <http://www.gnu.org/licenses/>.
*
* $Id$
********************************************************************************************************************//**
* \file
* \brief Provides abstraction for compiler specific features.
*
* You can use this header to provide some abstraction for compiler specific features.
***********************************************************************************************************************/

#ifndef COMPILER_H
#define COMPILER_H

/*******************************************************************************************************************//**
* \def INLINE
*
* \brief Indicates an inline function
*
* You can use the INLINE macro in place of compiler specific __inline, __inline__, or the C99 inline keyword to support
* inline functions.  The macro maps to the associated compiler function as needed.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \fn static unsigned countLeadingZeros32(unsigned long const v)
*
* \brief Determines the number of leading zeros in a number.
*
* You can use this function to calculate the number of leading zeros in a an arbitrary 32-bit number.
*
* \param[in] v The value to calculate the number of leading zeros for.
*
* \return Returns the number of leading zeros.  The value 0xFFFFFFFF will return 0.  The value 0x00000001 will return
*         31.  A value of 0 returns 32.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \fn static unsigned countLeadingZeros64(unsigned long long const v)
*
* \brief Determines the number of leading zeros in a number.
*
* You can use this function to calculate the number of leading zeros in a an arbitrary 64-bit number.
*
* \param[in] v The value to calculate the number of leading zeros for.
*
* \return Returns the number of leading zeros.  The value 0xFFFFFFFFFFFFFFFF will return 0.  The value
*         0x0000000000000001 will return 63.  A value of 0 returns 64.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \fn static unsigned countTrailingZeros32(unsigned long const v)
*
* \brief Determines the number of trailing zeros in a number.
*
* You can use this function to calculate the number of trailing zeros in a an arbitrary 32-bit number.
*
* \param[in] v The value to calculate the number of trailing zeros for.
*
* \return Returns the number of trailing zeros.  The value 0xFFFFFFFF will return 0.  The value 0x80000000 will return
*         31.  A value of 0 returns 32.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \fn static unsigned countTrailingZeros64(unsigned long long const v)
*
* \brief Determines the number of trailing zeros in a number.
*
* You can use this function to calculate the number of trailing zeros in a an arbitrary 64-bit number.
*
* \param[in] v The value to calculate the number of trailing zeros for.
*
* \return Returns the number of trailing zeros.  The value 0xFFFFFFFFFFFFFFFF will return 0.  The value
*         0x8000000000000000 will return 63.  A value of 0 returns 64.
***********************************************************************************************************************/

#if (defined(__GNUC__))

    #define INLINE __inline__ static

    INLINE unsigned countLeadingZeros32(unsigned long const v) {
        return v == 0 ? 32 : __builtin_clzl(v);
    }

    INLINE unsigned countLeadingZeros64(unsigned long long const v) {
        return v == 0 ? 64 : __builtin_clzll(v);
    }

    INLINE unsigned countTrailingZeros32(unsigned long const v) {
        return v == 0 ? 32 : __builtin_ctz(v);
    }

    INLINE unsigned countTrailingZeros64(unsigned long long const v) {
        return v == 0 ? 64 : __builtin_ctzl(v);
    }

#else

    unsigned countLeadingZeros32(unsigned long const v);
    unsigned countLeadingZeros64(unsigned long long const v);
    unsigned countTrailingZeros32(unsigned long const v);
    unsigned countTrailingZeros64(unsigned long long const v);

#endif

#endif
