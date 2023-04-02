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
* \brief Parameters used for the Sieve Of Eratosthenes algorithm.
*
* You can use this header file to specify parameters for the Sieve Of Eratosthenes algorithm.
***********************************************************************************************************************/

#ifndef PARAMETERS_H
#define PARAMETERS_H

/*******************************************************************************************************************//**
* \brief Indicates the default value for the --verbose switch.
*
* You can use this define to indicate whether or not verbose output should be enabled by default.
***********************************************************************************************************************/
#define VERBOSE (0)

/*******************************************************************************************************************//**
* \brief Indicates the default maximum prime value that will be searched for.
*
* You can use this define to specify the maximum prime value that will be assumed unless otherwise specified on the
* command line.
***********************************************************************************************************************/
//#define MAXIMUM_PRIME (0x100000000000ULL)-1
#define MAXIMUM_PRIME (1ULL << 34)-1
//#define MAXIMUM_PRIME (0x10000ULL)-1

/*******************************************************************************************************************//**
* \brief Indicates the number of primes tracked per puddle.
*
* You can use this define to determine the number of primes tracked per puddle.  A puddle represents a single storage
* element in memory that will be atomically updated.
***********************************************************************************************************************/
#define PUDDLE_SIZE (32)

/*******************************************************************************************************************//**
* \brief Indicates the maximum in memory allocation.
*
* You can use this define to determine the maximimum memory allocation that this application should assume.  Note that
* this size is approximate and may be off due to memory alignment constraints.
***********************************************************************************************************************/
#define POOL_SIZE_IN_BYTES (1024*1024*1024)
//#define POOL_SIZE_IN_BYTES (1024)

/*******************************************************************************************************************//**
* \brief Indicates the file prefix to use for the prime files.
*
* You can use this define to specify the prefix for files containing information on the primes.
***********************************************************************************************************************/
#define PRIME_FILE_PREFIX ("primes.")

#endif
