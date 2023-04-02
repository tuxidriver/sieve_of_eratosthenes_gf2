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
* \brief Defines a basic command line parser.
*
* This module defines a simple command line parser that you can use to simplify processing command lines in ANSI-C.
***********************************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "cmdline.h"


static void deleteParameter(char** const argumentValues, int* const argumentCount, int const argumentNumber) {
    #define ARGUMENT_COUNT (*argumentCount)

    char** s   = argumentValues + argumentNumber + 1;
    char** d   = argumentValues + argumentNumber;
    int  count = ARGUMENT_COUNT - argumentNumber - 1;

    while (count) {
        *(d++) = *(s++);
        count--;
    }

    *d = NULL;

    ARGUMENT_COUNT--;

    #undef ARGUMENT_COUNT
}


static int buildExitCode(CmdLineExitCode const errorCode, int const argumentNumber) {
    return ((int) errorCode << 24) + argumentNumber;
}


long cmdLineParse(int* const argumentCount, char** const argumentValues, CmdLineSwitch* const switches) {
    #define ARGUMENT_COUNT (*argumentCount)

    int             argumentNumber = 1;
    int             index          = 0;
    CmdLineExitCode exitCode       = CMDLINE_SUCCESSFUL;

    while (switches[index].switchType != CMDLINE_END) {
        if (switches[index].switchType != CMDLINE_HELP) {
            *((void **) switches[index].stateVariable) = NULL;
        }

        index++;
    }


    while (argumentNumber < ARGUMENT_COUNT && exitCode == 0) {
        int            index    = 0;
        char*          argument = argumentValues[argumentNumber];
        CmdLineSwitch* sw;

        while (switches[index].switchType != CMDLINE_END && strcmp(switches[index].text, argument) != 0) {
            index++;
        }

        sw = switches+index;

        switch (sw->switchType) {
            case CMDLINE_BOOL: {
                #define BOOL_STATE_VARIABLE ((int **) sw->stateVariable)

                if (*BOOL_STATE_VARIABLE != NULL) {
                    exitCode = CMDLINE_DUPLICATE_SWITCH;
                } else {
                    *BOOL_STATE_VARIABLE = (int *) malloc(sizeof(int));
                    assert(*BOOL_STATE_VARIABLE != NULL);

                    **BOOL_STATE_VARIABLE = sw->boolState;
                    deleteParameter(argumentValues, argumentCount, argumentNumber);
                }

                break;

                #undef BOOL_STATE_VARIABLE
            }

            case CMDLINE_STRING: {
                #define STRING_STATE_VARIABLE ((char **) sw->stateVariable)

                if (*STRING_STATE_VARIABLE != NULL) {
                    exitCode = CMDLINE_DUPLICATE_SWITCH;
                } else if (argumentNumber == ARGUMENT_COUNT-1) {
                    exitCode = CMDLINE_PARAMETER_REQUIRED;
                } else {
                    deleteParameter(argumentValues, argumentCount, argumentNumber);

                    *STRING_STATE_VARIABLE = strdup(argumentValues[argumentNumber]);
                    assert(*STRING_STATE_VARIABLE != NULL);

                    deleteParameter(argumentValues, argumentCount, argumentNumber);
                }

                break;

                #undef STRING_STATE_VARIABLE
            }

            case CMDLINE_LONG: {
                #define LONG_STATE_VARIABLE ((long **) sw->stateVariable)

                if (*LONG_STATE_VARIABLE != NULL) {
                    exitCode = CMDLINE_DUPLICATE_SWITCH;
                } else if (argumentNumber == ARGUMENT_COUNT-1) {
                    exitCode = CMDLINE_PARAMETER_REQUIRED;
                } else {
                    char* endPointer;

                    *LONG_STATE_VARIABLE = (long *) malloc(sizeof(long));
                    assert(*LONG_STATE_VARIABLE != NULL);

                    **LONG_STATE_VARIABLE = strtol(argumentValues[argumentNumber+1], &endPointer, 10);

                    if (endPointer != NULL && strlen(endPointer) != 0) {
                        exitCode = CMDLINE_INTEGER_VALUE_EXPECTED;
                    } else {
                        deleteParameter(argumentValues, argumentCount, argumentNumber);
                        deleteParameter(argumentValues, argumentCount, argumentNumber);
                    }
                }

                break;

                #undef LONG_STATE_VARIABLE
            }

            case CMDLINE_LONG_LONG: {
                #define LONG_LONG_STATE_VARIABLE ((long long**) sw->stateVariable)

                if (*LONG_LONG_STATE_VARIABLE != NULL) {
                    exitCode = CMDLINE_DUPLICATE_SWITCH;
                } else if (argumentNumber == ARGUMENT_COUNT-1) {
                    exitCode = CMDLINE_PARAMETER_REQUIRED;
                } else {
                    char* endPointer;

                    *LONG_LONG_STATE_VARIABLE = (long long*) malloc(sizeof(long long));
                    assert(*LONG_LONG_STATE_VARIABLE != NULL);

                    **LONG_LONG_STATE_VARIABLE = strtol(argumentValues[argumentNumber+1], &endPointer, 10);

                    if (endPointer != NULL && strlen(endPointer) != 0) {
                        exitCode = CMDLINE_INTEGER_VALUE_EXPECTED;
                    } else {
                        deleteParameter(argumentValues, argumentCount, argumentNumber);
                        deleteParameter(argumentValues, argumentCount, argumentNumber);
                    }
                }

                break;

                #undef LONG_LONG_STATE_VARIABLE
            }

            case CMDLINE_DOUBLE: {
                #define DOUBLE_STATE_VARIABLE ((double **) sw->stateVariable)

                if (*DOUBLE_STATE_VARIABLE != NULL) {
                    exitCode = CMDLINE_DUPLICATE_SWITCH;
                } else if (argumentNumber == ARGUMENT_COUNT-1) {
                    exitCode = CMDLINE_PARAMETER_REQUIRED;
                } else {
                    char* endPointer;

                    *DOUBLE_STATE_VARIABLE = (double *) malloc(sizeof(double));
                    assert(*DOUBLE_STATE_VARIABLE != NULL);

                    **DOUBLE_STATE_VARIABLE = strtod(argumentValues[argumentNumber+1], &endPointer);
                    if (endPointer != NULL && strlen(endPointer) != 0) {
                        exitCode = CMDLINE_NUMBER_EXPECTED;
                    } else {
                        deleteParameter(argumentValues, argumentCount, argumentNumber);
                        deleteParameter(argumentValues, argumentCount, argumentNumber);
                    }
                }

                break;

                #undef DOUBLE_STATE_VARIABLE
            }

            case CMDLINE_HELP: {
                exitCode = CMDLINE_HELP_REQUESTED;
                break;
            }

            case CMDLINE_END: {
                argumentNumber++;
                break;
            }
        }
    }

    if (exitCode == CMDLINE_SUCCESSFUL) {
        return 0;
    } else {
        return buildExitCode(exitCode, argumentNumber);
    }

    #undef ARGUMENT_COUNT
}


long cmdLineReportError(long const exitCode, char** const argumentValues, CmdLineSwitch* const switches) {
    CmdLineExitCode exitCondition  = cmdLineExitCode(exitCode);
    int             argumentNumber = cmdLineArgumentNumber(exitCode);
    char*           argument       = argumentValues[argumentNumber];

    switch (exitCondition) {
        case CMDLINE_SUCCESSFUL: {
            break;
        }

        case CMDLINE_DUPLICATE_SWITCH: {
            fprintf(stderr, "*** Error: Duplicate switch \"%s\".\n", argument);
            break;
        }

        case CMDLINE_PARAMETER_REQUIRED: {
            fprintf(stderr, "*** Error: Parameter required for switch \"%s\".\n", argument);
            break;
        }

        case CMDLINE_INTEGER_VALUE_EXPECTED: {
            fprintf(stderr, "*** Error: Integer value required for switch \"%s\".\n", argument);
            break;
        }

        case CMDLINE_NUMBER_EXPECTED: {
            fprintf(stderr, "*** Error: Numerical value required for switch \"%s\".\n", argument);
            break;
        }

        case CMDLINE_HELP_REQUESTED: {
            CmdLineSwitch* sw = switches;

            while (sw->switchType != CMDLINE_HELP && sw->switchType != CMDLINE_END) {
                ++sw;
            }

            assert(sw->switchType != CMDLINE_END);

            fprintf(stdout, "%s\n", (char*) sw->stateVariable);
            break;
        }

        default: {
            assert(0);
        }
    }

    return exitCode;
}


void cmdLineDeallocate(CmdLineSwitch* const switches) {
    CmdLineSwitch* sw = switches;

    while (sw->switchType != CMDLINE_END) {
        if (sw->switchType != CMDLINE_HELP) {
            void** stateVariable = (void**) (sw->stateVariable);
            assert(stateVariable != NULL);

            if (*stateVariable != NULL) {
                free(*stateVariable);
                *stateVariable = NULL;
            }
        }

        sw++;
    }
}
