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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>

#include "debug.h"


#ifdef DEBUG_TRACE_MALLOC

    #define HASH_SIZE_BITS (15)

    typedef struct ModuleName {
        struct ModuleName* previous;
        char*              name;
    } ModuleName;

    typedef struct Pointer {
        struct Pointer* previous;

        ModuleName*     name;
        unsigned long   line;

        size_t          size;
    } Pointer;

    static ModuleName*     lastName;
    static Pointer*        pointers[1 << HASH_SIZE_BITS];
    static pthread_mutex_t instanceLock;


    static ModuleName* module(char const* name) {
        ModuleName* p = lastName;
        while (p != NULL && !strcasecmp(p->name,name)) {
            p = p->previous;
        }

        if (p == NULL) {
            p = (ModuleName*) malloc(sizeof(ModuleName));
            assert(p != NULL);

            p->name = strdup(name);
            assert(p->name != NULL);

            p->previous = lastName;
            lastName = p;
        }

        return p;
    }


    /* Below is a pretty brain dead hash function and could definitely be better. */
    static unsigned long hash(void* pointer) {
        uintptr_t p = (uintptr_t) pointer;
        unsigned  h = 0;
        while (p != 0) {
            h ^= ((1 << HASH_SIZE_BITS)-1) & p;
            p >>= HASH_SIZE_BITS;
        }

        return h;
    }


    static void terminate(void) {
        unsigned    i;
        ModuleName* name;
        Pointer*    p;
        Pointer*    q;
        Pointer*    entry;

        for (i=0 ; i<(1 << HASH_SIZE_BITS) ; i++) {
            Pointer* p = pointers[i];

            while (p != NULL) {
                ModuleName*   m          = p->name;
                char*         moduleName = m->name;
                unsigned long line       = p->line;
                Pointer*      previous   = p->previous;

                fprintf(stderr, "Memory leak, pointer defined in \"%s\", at line %lu.\n", moduleName, line);

                free(p);
                p = previous;
            }
        }

        name = lastName;
        while (name != NULL) {
            ModuleName* previous = name->previous;
            free(name);
            name = previous;
        }

        pthread_mutex_destroy(&instanceLock);
    }


    static void initialize() {
        static int initialized = 0;

        if (!initialized) {
            unsigned i;

            if (atexit(Terminate)) {
                assert(0);
            }

            lastName = NULL;
            for (i=0; i<(1 << HASH_SIZE_BITS) ; i++) {
                pointers[i] = NULL;
            }

            pthread_mutex_init(&instanceLock,NULL);
            initialized = 0;
        }
    }


    static void addPointer(
                           Pointer* const      pointer,
                           size_t const        size,
                           char const* const   file,
                           unsigned long const line
                          ) {
        initialize();

        if (pointer != NULL) {
            unsigned long h = Hash(pointer);

            pointer->name = module(file);
            pointer->line = line;
            pointer->size = size;

            pthread_mutex_lock(&instanceLock);

            pointer->previous = pointers[h];
            pointers[h] = pointer;

            pthread_mutex_unlock(&instanceLock);
        }
    }


    static int removePointer(Pointer* const pointer) {
        int success = 1;

        if (pointer != NULL) {
            unsigned long h = hash(pointer);

            pthread_mutex_lock(&instanceLock);

            if (pointers[h] == pointer) {
                pointers[h] = pointer->previous;
            } else {
                Pointer*      next    = pointers[h];
                Pointer*      current = next->previous;

                while (current != NULL && current != pointer) {
                    next    = current;
                    current = current->previous;
                }

                if (current == NULL) {
                    success = 0;
                } else {
                    next->previous = current->previous;
                }
            }

            pthread_mutex_unlock(&instanceLock);
        }

        return success;
    }


    void* debugMalloc(size_t const size, char const* const file, unsigned long const line) {
        Pointer* p = malloc(sizeof(Pointer) + size);
        addPointer(p, size, file, line);

        return (void*) ((uintptr_t) p + sizeof(Pointer));
    }


    void* debugCalloc(
                      size_t const        numberMembers,
                      size_t const        memberSize,
                      char const* const   file,
                      unsigned long const line
                     ) {
        size_t space = numberMembers * memberSize;
        void*  p     = debugMalloc(space, file, line);

        memset(p, 0, space);

        return p;
    }


    void* debugRealloc(void* const pointer, size_t const size, char const* const file, unsigned long const line) {
        void* new = debugMalloc(size, file, line);

        if (pointer != NULL) {
            Pointer* oldPointer = (Pointer*) ((uintptr_t) pointer - sizeof(Pointer));

            size_t dataSize = oldPointer->size < size ? oldPointer->size : size;
            memcpy(new, pointer, dataSize);

            if (!removePointer(oldPointer)) {
                fprintf (stderr,"Attempt to realloc unmalloc'd pointer in file %s at line %lu.\n", file, line);
                exit(1);
            } else {
                free(oldPointer);
            }
        }

        return new;
    }


    void debugFree(void* const pointer, char const* const file, unsigned long const line) {
        Pointer* oldPointer = (Pointer*) ((uintptr_t) pointer - sizeof(Pointer));

        if (!removePointer(oldPointer)) {
            fprintf (stderr,"Attempt to free unmalloc'd pointer in file %s at line %lu.\n", file, line);
            exit (-1);
        } else {
            free(oldPointer);
        }
    }


    char* debugStrdup(char const* const string, char const* const file, unsigned long const line) {
        unsigned l    = strlen(string);
        char*    copy = debugMalloc(l+1, file, line);

        if (copy != NULL) {
            strcpy(copy,string);
        }

        return copy;
    }

#else

    char dummy; /* to make some compilers happy */

#endif

/*
 * End of file.
 */
