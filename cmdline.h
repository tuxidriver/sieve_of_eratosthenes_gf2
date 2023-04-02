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

#ifndef CMDLINE_H
#define CMDLINE_H

/*******************************************************************************************************************//**
* \brief Enumeration of support switch types.
*
* You can use this enumeration to specify the type of parameter supported by a given switch.
***********************************************************************************************************************/
typedef enum CmdLineSwitchType {
    /***************************************************************************************************************//**
    * \brief Indicates a boolean value.
    *
    * You can use this value to indicate a switch can take on a true or false value.
    *******************************************************************************************************************/
    CMDLINE_BOOL,

    /***************************************************************************************************************//**
    * \brief Indicates a string value.
    *
    * You can use this value to indicate a switch can take on a string value.
    *******************************************************************************************************************/
    CMDLINE_STRING,

    /***************************************************************************************************************//**
    * \brief Indicates a long integer value.
    *
    * You can use this value to indicate a switch can take on a long integer value.
    *******************************************************************************************************************/
    CMDLINE_LONG,

    /***************************************************************************************************************//**
    * \brief Indicates a long long integer value.
    *
    * You can use this value to indicate a switch can take on a long long integer value.
    *******************************************************************************************************************/
    CMDLINE_LONG_LONG,
    
    /***************************************************************************************************************//**
    * \brief Indicates a double precision value.
    *
    * You can use this value to indicate a switch can take on a double precision value.
    *******************************************************************************************************************/
    CMDLINE_DOUBLE,

    /***************************************************************************************************************//**
    * \brief Indicates a switch that should display help text.
    *
    * You can use this switch to force help text to be displayed.
    *******************************************************************************************************************/
    CMDLINE_HELP,

    /***************************************************************************************************************//**
    * \brief Indicates the end of the list of command line parameters.
    *
    * You can use this value to indicate the end of the list of command line parameters.
    *******************************************************************************************************************/
    CMDLINE_END
} CmdLineSwitchType;

/*******************************************************************************************************************//**
* \brief Enumeration of error codes.
*
* You can use this enumeration to determine the type of error that has occurred.
***********************************************************************************************************************/
typedef enum CmdLineExitCode {
    /***************************************************************************************************************//**
    * \brief Indicates successful completion.
    *
    * You can use this value to indicate that the command line parsing operation was successful.
    *******************************************************************************************************************/
    CMDLINE_SUCCESSFUL = 0,

    /***************************************************************************************************************//**
    * \brief Indicates a duplicate switch was found on the command line.
    *
    * You can use this value to indicate that a duplicate switch was found on the command line.
    *******************************************************************************************************************/
    CMDLINE_DUPLICATE_SWITCH,

    /***************************************************************************************************************//**
    * \brief Indicates a switch that should have includes a parameter did not.
    *
    * You can use this value to indicate that a switch that should have a parameter did not have the required value.
    *******************************************************************************************************************/
    CMDLINE_PARAMETER_REQUIRED,

    /***************************************************************************************************************//**
    * \brief Indicates a switch that should have includes an integer parameter did not.
    *
    * You can use this value to indicate that a switch found a value that was not an integer.
    *******************************************************************************************************************/
    CMDLINE_INTEGER_VALUE_EXPECTED,

    /***************************************************************************************************************//**
    * \brief Indicates a switch that should have includes an numerical parameter did not.
    *
    * You can use this value to indicate that a switch found a value that was not numerical.
    *******************************************************************************************************************/
    CMDLINE_NUMBER_EXPECTED,

    /*******************************************************************************************************************
    * Indicates a help switch was included on the command line.
    *
    * You can use this value to indicate that help text was requested.
    *******************************************************************************************************************/
    CMDLINE_HELP_REQUESTED
} CmdLineExitCode;

/*******************************************************************************************************************//**
* \brief Enumeration of support switch types.
*
* You can use this enumeration to specify the type of parameter supported by a given switch.  You should use the macros:
*
* * \ref CMDLINE_DEFINITION_START
* * \ref CMDLINE_BOOL_TRUE
* * \ref CMDLINE_BOOL_FALSE
* * \ref CMDLINE_STRING
* * \ref CMDLINE_LONG
* * \ref CMDLINE_LONE_LONG                                                                                                                      
* * \ref CMDLINE_DOUBLE
* * \ref CMDLINE_DEFINITION_END
***********************************************************************************************************************/
typedef struct CmdLineSwitch {
    CmdLineSwitchType switchType;
    char              *text;
    void              *stateVariable;
    int               boolState;
} CmdLineSwitch;

/*******************************************************************************************************************//**
* \brief Macro that starts a command line definition.
*
* You can use this macro to start the description of a list of command line parameters.
***********************************************************************************************************************/
#define CMDLINE_DEFINITION_START(_foo) CmdLineSwitch (_foo)[] = {

/*******************************************************************************************************************//**
* \brief Macro that defines a command line switch that creates a true boolean value.
*
* You can use this macro to define switch that supports a true or non-zero boolean value.
***********************************************************************************************************************/
#define CMDLINE_BOOL_TRUE(_switch, _stateVariable)  { CMDLINE_BOOL, (_switch), &(_stateVariable), 1 },

/*******************************************************************************************************************//**
* \brief Macro that defines a command line switch that creates a false boolean value.
*
* You can use this macro to define a switch that supports a false or non-zero boolean value.
***********************************************************************************************************************/
#define CMDLINE_BOOL_FALSE(_switch, _stateVariable) { CMDLINE_BOOL, (_switch), &(_stateVariable), 0 },

/*******************************************************************************************************************//**
* \brief Macro that defines a command line switch that allows for string values.
*
* You can use this macro to define switch that supports string values.
***********************************************************************************************************************/
#define CMDLINE_STRING(_switch, _stateVariable)     { CMDLINE_STRING, (_switch), &(_stateVariable), 0 },

/*******************************************************************************************************************//**
* \brief Macro that defines a command line switch that allows for long integer values.
*
* You can use this macro to define switch that supports long integer values.
***********************************************************************************************************************/
#define CMDLINE_LONG(_switch, _stateVariable)       { CMDLINE_LONG, (_switch), &(_stateVariable), 0 },

/*******************************************************************************************************************//**
* \brief Macro that defines a command line switch that allows for long long integer values.
*
* You can use this macro to define switch that supports long long integer values.
***********************************************************************************************************************/
#define CMDLINE_LONG_LONG(_switch, _stateVariable)  { CMDLINE_LONG_LONG, (_switch), &(_stateVariable), 0 },

/*******************************************************************************************************************//**
* \brief Macro that defines a command line switch that allows for double precision values.
*
* You can use this macro to define switch that supports double precision values.
***********************************************************************************************************************/
#define CMDLINE_DOUBLE(_switch, _stateVariable)     { CMDLINE_DOUBLE, (_switch), &(_stateVariable), 0 },

/*******************************************************************************************************************//**
* \brief Macro that defines a command line switch that requests help text.
*
* You can use this macro to define switch that displays help text.
***********************************************************************************************************************/
#define CMDLINE_HELP(_switch, _helpText)     { CMDLINE_HELP, (_switch), _helpText, 0 },

/*******************************************************************************************************************//**
* \brief Macro that marks the end of a command line definition.
*
* You can use this macro to mark the end of a command line definition.
***********************************************************************************************************************/
#define CMDLINE_DEFINITION_END { CMDLINE_END, NULL, NULL, 0 } };

/*******************************************************************************************************************//**
* \brief Macro that returns the CmdLineExit code.
*
* You can use this macro/function to determine the type of error reported by \ref cmdLineParse.
*
* \param[in] exitValue The exit value returned by \ref cmdLineParse.
*
* \return Returns a value from \ref CmdLineExitCode indicating the type of error that has occurred.
***********************************************************************************************************************/
#define cmdLineExitCode(_exitValue) ((CmdLineExitCode) ((long) _exitValue >> 24))

/*******************************************************************************************************************//**
* \brief Macro that returns the argument number for the argument that triggered the error.
*
* You can use this macro/function to determine the argument number of the argument that triggered the error.
*
* \param[in] exitValue The exit value returned by \ref cmdLineParse.
*
* \return Returns the zero based argument number that triggered the error.
***********************************************************************************************************************/
#define cmdLineArgumentNumber(_exitValue) ((int) ((long) _exitValue & 0x00FFFFFF))

/*******************************************************************************************************************//**
* \brief Function that parses command line parameters.
*
* You can use this function to parse command line parameters.  The function takes an array of \ref CmdLineSwitch
* instances as an input to drive the parsing process.
*
* \param[in,out] argumentCount  The count of the number of command line switches.  The count is adjusted downward to
*                               include only those parameters that could not be parsed by this function.
*
* \param[in,out] argumentValues The values taken from the command line.  Parsed switches and parameters are removed
*                               from the array.
*
* \param[in]     switches       The array of \ref CmdLineSwitch instances to use to drive the parsing process.
*
* \return Returns zero on success.  Returns a non-zero on error.
***********************************************************************************************************************/
long cmdLineParse(int* const argumentCount, char** const argumentValues, CmdLineSwitch* const switches);

/*******************************************************************************************************************//**
* \brief Function that reports errors.
*
* You can use this function to generate a standard error message based on the returned exit code.
*
* \param[in] exitCode       The exit code returned by \ref cmdLineParse.
*
* \param[in] argumentValues The argument values returned by the function.
*
* \param[in] switches       The array of \ref CmdLineSwitch instances to use to drive the parsing process.
*
* \return Returns the exit code.
***********************************************************************************************************************/
long cmdLineReportError(long const exitCode, char** const argumentValues, CmdLineSwitch* const switches);

/*******************************************************************************************************************//**
* \brief Function that conditionally deallocates an allocated pointer.
*
* You can use this function to conditionally deallocate space used for command line parameters.
*
* \param[in] switches The array of \ref CmdLineSwitch instances that were used to parse the command line.
***********************************************************************************************************************/
void cmdLineDeallocate(CmdLineSwitch* const switches);

#endif
