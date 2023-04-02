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
* \brief Maintains a concise list of primes.
*
* This file defines functions use to maintain and track lists of primes.
***********************************************************************************************************************/

#ifndef PRIME_LIST_H
#define PRIME_LIST_H

#include "gf2.h"


/*******************************************************************************************************************//**
* \brief Specifies how the prime file should be opened.
*
* You can use this enumeration to clearly specify how the prime file should be opened.
***********************************************************************************************************************/
typedef enum PrimeListOpenMode {
    PRIME_FILE_CREATE_NEW,
    PRIME_FILE_OPEN_FOR_READING
} PrimeListOpenMode;

/*******************************************************************************************************************//**
* \brief Initializes the prime list.
*
* You can use this function to initialize the prime list.
*
* \param[in] filePrefix A prefix to assign to the prime list files.
*
* \param[in] openMode You can use this define to specify how the prime file should be opened.
***********************************************************************************************************************/
void initializePrimeList(char const* const filePrefix, PrimeListOpenMode const openMode);

/*******************************************************************************************************************//**
* \brief Cleans up the generated prime list.
*
* You can use this function to do any clean-up to get the prime list coherent when the application terminates.
***********************************************************************************************************************/
void terminatePrimeList(void);

/*******************************************************************************************************************//**
* \brief Marks a value as composite (not prime).
*
* You can use this function to mark an entry as a composite value.
*
* \param[in] value The value to mark as a composite value.
***********************************************************************************************************************/
void markComposite(Gf2Polynomial const value);

/*******************************************************************************************************************//**
* \brief Determines if a value is prime.
*
* You can use this function to determine if a specified value is prime.
*
* \param[in] value The value to be checked.
*
* \return Returns 0 if the value is composite.  Returns a non-zero result if the value is prime.
***********************************************************************************************************************/
int isPrime(Gf2Polynomial const value);

/*******************************************************************************************************************//**
* \brief Locates the next known prime value.
*
* You can use this function to quickly locate the next prime value in the prime list.
*
* \param[in] currentPrime The current value.  The function will find the next prime after this value.
*
* \return Returns the next known prime.  A value of 0 is returned if no new prime could be located.
***********************************************************************************************************************/
Gf2Polynomial findNextPrime(Gf2Polynomial const currentPrime);

#endif
