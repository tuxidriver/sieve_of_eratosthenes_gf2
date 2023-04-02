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
* \brief Support functions to assist with debugging.
*
* This module defines thin wrappers around a small number of central ANSI-C functions.  You can use the functions to
* check for issues related to the heap.
***********************************************************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <string.h>

/*******************************************************************************************************************//**
* \def MALLOC
*
* \brief You can use this macro as a wrapper for the stdio.h malloc function.
*
* You can use this macro as a wrapper for the stdio.h malloc function.  The wrapper provides support to track allocation
* and freeing of data structures in order to identify memory leaks.
*
* \param[in] _size The size of the region to be allocated, in bytes.
*
* \return Returns a pointer to the allocated member.  A NULL pointer is returned on error.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \def CALLOC
*
* \brief You can use this macro as a wrapper for the stdio.h calloc function.
*
* You can use this macro as a wrapper for the stdio.h calloc function.  The wrapper provides support to track allocation
* and freeing of data structures in order to identify memory leaks.
*
* \param[in] _numberMembers The number of members to be allocated.
*
* \param[in] _memberSize    The size of each member, in bytes.
*
* \return Returns a pointer to the allocated member.  A NULL pointer is returned on error.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \def REALLOC
*
* \brief You can use this macro as a wrapper for the stdio.h realloc function.
*
* You can use this macro as a wrapper for the stdio.h realloc function.  The wrapper provides support to track
* allocation and freeing of data structures in order to identify memory leaks.
*
* \param[in] _pointer The pointer to the memory region to be resized.
*
* \param[in] _size    The new size for the memory region, in bytes.
*
* \return Returns a pointer to the new location for the member.  A NULL pointer is returned on error.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \def FREE
*
* \brief You can use this macro as a wrapper for the stdio.h free function.
*
* You can use this macro as a wrapper for the stdio.h free function.  The wrapper provides support to track allocation
* and freeing of data structures in order to identify memory leaks.
*
* \param[in] _pointer The pointer to the memory region to be released.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \def STRDUP
*
* \brief You can use this macro as a wrapper for the string.h strdup function.
*
* You can use this macro as a wrapper for the string.h strdup function.  The wrapper provides support to track
* allocation and freeing of data structures in order to identify memory leaks.
*
* \param[in] _string The string to be duplicated.
*
* \return Returns a pointer to the duplicated string.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* \def DEBUG_INITIALIZE
*
* \brief You can use this macro as a wrapper for the string.h strdup function.
*
* You can use this macro as a wrapper for the string.h strdup function.  The wrapper provides support to track
* allocation and freeing of data structures in order to identify memory leaks.
*
* \param[in] _string The string to be duplicated.
*
* \return Returns a pointer to the duplicated string.
***********************************************************************************************************************/

#ifdef DEBUG_TRACE_MALLOC

    #define MALLOC(_size)                      (debugMalloc((_size),__FILE__,__LINE__))
    #define CALLOC(_numberMembers,_memberSize) (debugCalloc((_numberMembers),(_memberSize),__FILE__,__LINE__))
    #define REALLOC(_pointer,_size)            (debugRealloc((_pointer),(_size),__FILE__,__LINE__))

    #define FREE(_pointer)                     (debugFree((_pointer),__FILE__,__LINE__))

    #define STRDUP(_string)                    (debugStrDup((_string),__FILE__,__LINE__))

    void  debugInitialize();
    void* debugMalloc(size_t const size, char const* const file, unsigned long const line);
    void* debugCalloc(
                      size_t const        numberMembers,
                      size_t const        memberSize,
                      char const* const   file,
                      unsigned long const line
                     );
    void* debugRealloc(void* const pointer, size_t const size, char const* const file, unsigned long const line);
    void  debugFree(void* const pointer, char const* const file, unsigned long const line);
    char* debugStrdup(char const* string, char const* const file, unsigned long const line);

#else

    #define MALLOC(_size)                      malloc(_size)
    #define CALLOC(_numberMembers,_memberSize) calloc((_numberMembers),(_memberSize))
    #define REALLOC(_pointer,_size)            realloc(_pointer,_size)

    #define FREE(_pointer)                     free(_pointer)

    #define STRDUP(_string)                    strdup(_string)

#endif

#endif
