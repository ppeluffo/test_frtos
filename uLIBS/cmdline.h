/*! \file cmdline.h \brief Command-Line Interface Library. */
//*****************************************************************************
//
// File Name	: 'cmdline.h'
// Title		: Command-Line Interface Library
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.07.16
// Revised		: 2003.07.16
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
///	\ingroup general
/// \defgroup cmdline Command-line Implementation (cmdline.c)
/// \code #include "cmdline.h" \endcode
/// \par Overview
///		This Command-Line interface library is meant to provide a reusable
///	terminal-like user interface much like a DOS command line or UNIX terminal.
///	A terminal with VT100 support is assumed on the user's end.  Common
///	line-editing is supported, including backspace, arrow keys, and even a
/// small command-history buffer.
///
///	\note One can imagine more efficient ways to implement the command and 
///		function database contained in this library, however, this is a decent
///		and functional first cut.  I may someday get around to making some
///		improvements.
///
/// \par Operation
///	The cmdline library does the following things for you:
///		- Prints command prompts
///		- Gathers a command string from the user (with editing features)
///		- Parses the command string when the user presses [ENTER]
///		- Compares the entered command to the command database
///		  -- Executes the corresponding function if a match is found
///		  -- Reports an error if no match is found
///		-Provides functions to retrieve the command arguments:
///		  --as strings
///		  --as decimal integers
///		  --as hex integers
///
///	Supported editing features include:
///		- Backspace support
///		- Mid-line editing, inserting and deleting (left/right-arrows)
///		- Command History (up-arrow) (currently only one command deep)
///
///	To use the cmdline system, you will need to associate command strings
///	(commands the user will be typing) with your function that you wish to have
///	called when the user enters that command.  This is done by using the
///	cmdlineAddCommand() function.
///
///	To setup the cmdline system, you must do these things:
///		- Initialize it: cmdlineInit()
///		- Add one or more commands to the database: cmdlineAddCommand()
///		- Set an output function for your terminal: cmdlineSetOutputFunc()
///
///	To operate the cmdline system, you must do these things repeatedly:
///		- Pass user input from the terminal to: cmdlineSetOutputFunc()
///		- Call cmdlineMainLoop() from your program's main loop
///
///	The cmdline library does not assume an input or output device, but can be
///	configured to use any user function for output using cmdlineSetOutputFunc()
///	and accepts input by calling cmdlineInputFunc().  This means the cmdline
///	library can operate over any interface including UART (serial port),
///	I2c, ethernet, etc.
///
///	***** FOR MORE INFORMATION ABOUT USING cmdline SEE THE AVRLIB EXAMPLE *****
///	***** CODE IN THE avrlib/examples DIRECTORY                           *****
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
//@{

#ifndef CMDLINE_H
#define CMDLINE_H

//----- Include Files ---------------------------------------------------------
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support
#include <avr/pgmspace.h>	// include AVR program memory support
#include <string.h>			// include standard C string functions
#include <stdlib.h>			// include stdlib for string conversion functions
#include "stdbool.h"

#include "xprintf.h"

// size of command database
// (maximum number of commands the cmdline system can handle)
#define CMDLINE_MAX_COMMANDS	10

// maximum length (number of characters) of each command string
// (quantity must include one additional byte for a null terminator)
#define CMDLINE_MAX_CMD_LENGTH	15

// allotted buffer size for command entry
// (must be enough chars for typed commands and the arguments that follow)
#define CMDLINE_BUFFERSIZE		64

// number of lines of command history to keep
// (each history buffer is CMDLINE_BUFFERSIZE in size)
// ***** ONLY ONE LINE OF COMMAND HISTORY IS CURRENTLY SUPPORTED
#define CMDLINE_HISTORYSIZE		1

// constants/macros/typdefs
typedef void (*CmdlineFuncPtrType)(void);

// functions

//! initalize the command line system
void cmdlineInit(void);

//! add a new command to the database of known commands
// newCmdString should be a null-terminated command string with no whitespace
// newCmdFuncPtr should be a pointer to the function to execute when
//   the user enters the corresponding command tring
//void cmdlineAddCommand( const uint8_t* newCmdString, CmdlineFuncPtrType newCmdFuncPtr);
void cmdlineAddCommand(const char* newCmdString, void (*newCmdFuncPtr)(void) );

//! sets the function used for sending characters to the user terminal
void cmdlineSetOutputFunc(void (*output_func)(unsigned char c));

//! call this function to pass input charaters from the user terminal
void cmdlineInputFunc(unsigned char c);

//! call this function in your program's main loop
void cmdlineMainLoop(void);

// internal commands
void cmdlineRepaint(void);
void cmdlineDoHistory(uint8_t action);
void cmdlineProcessInputString(void);
void cmdlinePrintPrompt(void);
void cmdlinePrintError(void);

// argument retrieval commands
//! returns a string pointer to argument number [argnum] on the command line
uint8_t* cmdlineGetArgStr(uint8_t argnum);
uint8_t cmdlineMakeArgv(void);
bool cmdlineExtractPayload(char *dst );

#define MAX_NUM_ARGS 16
char *argv[MAX_NUM_ARGS];


#endif
//@}
