/*! \file cmdline.c \brief Command-Line Interface Library. */
//*****************************************************************************
//
// File Name	: 'cmdline.c'
// Title		: Command-Line Interface Library
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.07.16
// Revised		: 2003.07.23
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

//----- Include Files ---------------------------------------------------------
#include "cmdline.h"


// defines
#define ASCII_BEL				0x07
#define ASCII_BS				0x08
#define ASCII_CR				0x0D
#define ASCII_LF				0x0A
#define ASCII_ESC				0x1B
#define ASCII_DEL				0x7F

#define VT100_ARROWUP			'A'
#define VT100_ARROWDOWN			'B'
#define VT100_ARROWRIGHT		'C'
#define VT100_ARROWLEFT			'D'

#define CMDLINE_HISTORY_SAVE	0
#define CMDLINE_HISTORY_PREV	1
#define CMDLINE_HISTORY_NEXT	2


// Global variables
// command list
// -commands are null-terminated strings
static char CmdlineCommandList[CMDLINE_MAX_COMMANDS][CMDLINE_MAX_CMD_LENGTH];
// command function pointer list
static CmdlineFuncPtrType CmdlineFunctionList[CMDLINE_MAX_COMMANDS];
// number of commands currently registered
uint8_t CmdlineNumCommands;

uint8_t CmdlineBuffer[CMDLINE_BUFFERSIZE];
uint8_t CmdlineBufferLength;
uint8_t CmdlineBufferEditPos;
uint8_t CmdlineInputVT100State;
uint8_t CmdlineHistory[CMDLINE_HISTORYSIZE][CMDLINE_BUFFERSIZE];
CmdlineFuncPtrType CmdlineExecFunction;

// Functions

// function pointer to single character output routine
//static void (*cmdlineOutputFunc)(unsigned char c);

//------------------------------------------------------------------------------
void cmdlineInit(void)
{
	// reset vt100 processing state
	CmdlineInputVT100State = 0;
	// initialize input buffer
	CmdlineBufferLength = 0;
	CmdlineBufferEditPos = 0;
	// initialize executing function
	CmdlineExecFunction = 0;
	// initialize command list
	CmdlineNumCommands = 0;
}
//------------------------------------------------------------------------------
void cmdlineAddCommand(const char* newCmdString, void (*newCmdFuncPtr)(void) )
//void cmdlineAddCommand(const uint8_t* newCmdString, CmdlineFuncPtrType newCmdFuncPtr)
{
	// add command string to end of command list
	strcpy(CmdlineCommandList[CmdlineNumCommands], newCmdString);
	// add command function ptr to end of function list
	CmdlineFunctionList[CmdlineNumCommands] = newCmdFuncPtr;
	// increment number of registered commands
	CmdlineNumCommands++;
 
}
//------------------------------------------------------------------------------
void cmdlineSetOutputFunc(void (*output_func)(unsigned char c))
{
	// set new output function
	//cmdlineOutputFunc = output_func;
	
	// should we really do this?
	// print a prompt 
	//cmdlinePrintPrompt();
}
//------------------------------------------------------------------------------
void cmdlineInputFunc(unsigned char c)
{
	uint8_t i;
	// process the received character

	// VT100 handling
	// are we processing a VT100 command?
	if(CmdlineInputVT100State == 2)
	{
		// we have already received ESC and [
		// now process the vt100 code
		switch(c)
		{
		case VT100_ARROWUP:
			cmdlineDoHistory(CMDLINE_HISTORY_PREV);
			break;
		case VT100_ARROWDOWN:
			cmdlineDoHistory(CMDLINE_HISTORY_NEXT);
			break;
		case VT100_ARROWRIGHT:
			// if the edit position less than current string length
			if(CmdlineBufferEditPos < CmdlineBufferLength)
			{
				// increment the edit position
				CmdlineBufferEditPos++;
				// move cursor forward one space (no erase)
                xprintf("%c[%c",ASCII_ESC,VT100_ARROWRIGHT);
				//cmdlineOutputFunc(ASCII_ESC);
				//cmdlineOutputFunc('[');
				//cmdlineOutputFunc(VT100_ARROWRIGHT);
			}
			else
			{
				// else, ring the bell
				//cmdlineOutputFunc(ASCII_BEL);
                xprintf("%c",ASCII_BEL);
			}
			break;
		case VT100_ARROWLEFT:
			// if the edit position is non-zero
			if(CmdlineBufferEditPos)
			{
				// decrement the edit position
				CmdlineBufferEditPos--;
				// move cursor back one space (no erase)
				//cmdlineOutputFunc(ASCII_BS);
                xprintf("%c",ASCII_BS);
			}
			else
			{
				// else, ring the bell
				//cmdlineOutputFunc(ASCII_BEL);
                xprintf("%c",ASCII_BEL);
			}
			break;
		default:
			break;
		}
		// done, reset state
		CmdlineInputVT100State = 0;
		return;
	}
	else if(CmdlineInputVT100State == 1)
	{
		// we last received [ESC]
		if(c == '[')
		{
			CmdlineInputVT100State = 2;
			return;
		}
		else
			CmdlineInputVT100State = 0;
	}
	else
	{
		// anything else, reset state
		CmdlineInputVT100State = 0;
	}

	// Regular handling
	if( (c >= 0x20) && (c < 0x7F) )
	{
		// character is printable
		// is this a simple append
		if(CmdlineBufferEditPos == CmdlineBufferLength)
		{
			// echo character to the output
			//cmdlineOutputFunc(c);
            xputChar(c);
            
			// add it to the command line buffer
			CmdlineBuffer[CmdlineBufferEditPos++] = c;
			// update buffer length
			CmdlineBufferLength++;
		}
		else
		{
			// edit/cursor position != end of buffer
			// we're inserting characters at a mid-line edit position
			// make room at the insert point
			CmdlineBufferLength++;
			for(i=CmdlineBufferLength; i>CmdlineBufferEditPos; i--)
				CmdlineBuffer[i] = CmdlineBuffer[i-1];
			// insert character
			CmdlineBuffer[CmdlineBufferEditPos++] = c;
			// repaint
			cmdlineRepaint();
			// reposition cursor
			for(i=CmdlineBufferEditPos; i<CmdlineBufferLength; i++)
				//cmdlineOutputFunc(ASCII_BS);
                xprintf("%c", ASCII_BS);
		}
	}
	// handle special characters
	else if(c == ASCII_CR)
	{
		// user pressed [ENTER]
		// echo CR and LF to terminal
		//cmdlineOutputFunc(ASCII_CR);
		//cmdlineOutputFunc(ASCII_LF);
        xprintf("\r\n");
        
		// add null termination to command
		CmdlineBuffer[CmdlineBufferLength++] = 0;
		CmdlineBufferEditPos++;
		// command is complete, process it
        //xprintf("IN: %s\r\n", CmdlineBuffer);
		cmdlineProcessInputString();
        //xprintf("OUT1: %s\r\n", CmdlineBuffer);

		// reset buffer
		CmdlineBufferLength = 0;
		CmdlineBufferEditPos = 0;
        
        //xprintf("OUT2: %s\r\n", CmdlineBuffer);

	}
	else if(c == ASCII_BS)
	{
		if(CmdlineBufferEditPos)
		{
			// is this a simple delete (off the end of the line)
			if(CmdlineBufferEditPos == CmdlineBufferLength)
			{
				// destructive backspace
				// echo backspace-space-backspace
				//cmdlineOutputFunc(ASCII_BS);
				//cmdlineOutputFunc(' ');
				//cmdlineOutputFunc(ASCII_BS);
                xprintf("%c %c",ASCII_BS, ASCII_BS);
				// decrement our buffer length and edit position
				CmdlineBufferLength--;
				CmdlineBufferEditPos--;
			}
			else
			{
				// edit/cursor position != end of buffer
				// we're deleting characters at a mid-line edit position
				// shift characters down, effectively deleting
				CmdlineBufferLength--;
				CmdlineBufferEditPos--;
				for(i=CmdlineBufferEditPos; i<CmdlineBufferLength; i++)
					CmdlineBuffer[i] = CmdlineBuffer[i+1];
				// repaint
				cmdlineRepaint();
				// add space to clear leftover characters
				//cmdlineOutputFunc(' ');
                xputChar(' ');
				// reposition cursor
				for(i=CmdlineBufferEditPos; i<(CmdlineBufferLength+1); i++)
					//cmdlineOutputFunc(ASCII_BS);
                    xprintf("%c", ASCII_BS );
			}
		}
		else
		{
			// else, ring the bell
			//cmdlineOutputFunc(ASCII_BEL);
             xprintf("%c", ASCII_BEL);
		}
	}
	else if(c == ASCII_DEL)
	{
		// not yet handled
	}
	else if(c == ASCII_ESC)
	{
		CmdlineInputVT100State = 1;
	} 
}
//------------------------------------------------------------------------------
void cmdlineRepaint(void)
{
	uint8_t* ptr;
	uint8_t i;

	// carriage return
	//cmdlineOutputFunc(ASCII_CR);
    xprintf("%c",ASCII_CR);
	// print fresh prompt
	cmdlinePrintPrompt();
	// print the new command line buffer
	i = CmdlineBufferLength;
	ptr = CmdlineBuffer;
	//while(i--) cmdlineOutputFunc(*ptr++);
    while(i--) xprintf("%c", *ptr++);
    xprintf("%s", ptr);
    
}
//------------------------------------------------------------------------------
void cmdlineDoHistory(uint8_t action)
{
	switch(action)
	{
	case CMDLINE_HISTORY_SAVE:
		// copy CmdlineBuffer to history if not null string
		if( strlen( (const char*)CmdlineBuffer) )
			strcpy( (char *)CmdlineHistory[0], (const char*)CmdlineBuffer);
		break;
	case CMDLINE_HISTORY_PREV:
		// copy history to current buffer
		strcpy( (char *)CmdlineBuffer, (const char*)CmdlineHistory[0]);
		// set the buffer position to the end of the line
		CmdlineBufferLength = strlen( (const char*)CmdlineBuffer);
		CmdlineBufferEditPos = CmdlineBufferLength;
		// "re-paint" line
		cmdlineRepaint();
		break;
	case CMDLINE_HISTORY_NEXT:
		break;
	}
}
//------------------------------------------------------------------------------
void cmdlineProcessInputString(void)
{
	uint8_t cmdIndex;
	uint8_t i=0;
    
	// save command in history
	cmdlineDoHistory(CMDLINE_HISTORY_SAVE);

	// find the end of the command (excluding arguments)
	// find first whitespace character in CmdlineBuffer
	while( !((CmdlineBuffer[i] == ' ') || (CmdlineBuffer[i] == 0)) ) i++;

	if(!i)
	{
		// command was null or empty
		// output a new prompt
		cmdlinePrintPrompt();
		// we're done
		return;
	}

	// search command list for match with entered command
	for(cmdIndex=0; cmdIndex<CmdlineNumCommands; cmdIndex++)
	{
		if( !strncmp( (const char*)CmdlineCommandList[cmdIndex], (const char*)CmdlineBuffer, i) )
		{
			// user-entered command matched a command in the list (database)
			// run the corresponding function
			CmdlineExecFunction = CmdlineFunctionList[cmdIndex];
			// new prompt will be output after user function runs
			// and we're done
			return;
		}
	}

	// if we did not get a match
	// output an error message
	cmdlinePrintError();
	// output a new prompt
	cmdlinePrintPrompt();
}
//------------------------------------------------------------------------------
void cmdlineMainLoop(void)
{
	// do we have a command/function to be executed
	if(CmdlineExecFunction)
	{
		// run it
		CmdlineExecFunction();
		// reset
		CmdlineExecFunction = 0;
		// output new prompt
		cmdlinePrintPrompt();
        
	}
}
//------------------------------------------------------------------------------
void cmdlinePrintPrompt(void)
{
    xprintf("cmd>");
}
//------------------------------------------------------------------------------
void cmdlinePrintError(void)
{
	uint8_t * ptr;

	// print a notice header
	xprintf("cmdline: ");
    
	// print the offending command
	ptr = CmdlineBuffer;
	while((*ptr) && (*ptr != ' ')) xprintf("%c", *ptr++);

	//cmdlineOutputFunc(':');
	//cmdlineOutputFunc(' ');

	// print the not-found message
    xprintf(": command not found\r\n");

}
//------------------------------------------------------------------------------
// argument retrieval commands
// return string pointer to argument [argnum]
uint8_t *cmdlineGetArgStr(uint8_t argnum)
{
	// find the offset of argument number [argnum]
	uint8_t idx=0;
	uint8_t arg;
	
	// find the first non-whitespace character
	while( (CmdlineBuffer[idx] != 0) && (CmdlineBuffer[idx] == ' ')) idx++;
	
	// we are at the first argument
	for(arg=0; arg<argnum; arg++)
	{
		// find the next whitespace character
		while( (CmdlineBuffer[idx] != 0) && (CmdlineBuffer[idx] != ' ')) idx++;
		// find the first non-whitespace character
		while( (CmdlineBuffer[idx] != 0) && (CmdlineBuffer[idx] == ' ')) idx++;
	}
	// we are at the requested argument or the end of the buffer
	return &CmdlineBuffer[idx];
}
//------------------------------------------------------------------------------
uint8_t cmdlineMakeArgv(void) {
    
uint8_t nro_args = 0;
uint8_t idx=0;


    // memset(cmdLine_buffer, '\0', sizeof(cmdLine_buffer) );
    //strcpy(cmdLine_buffer, "   write dac prueba pablo\r");

	// inicialmente todos los punteros deben apuntar a NULL.
	memset(argv, '\0', sizeof(argv) );

    // Primer argumento si comienza al inicio del buffer
    if ( CmdlineBuffer[0] != ' ') {
        argv[nro_args++] =  (char *)&CmdlineBuffer[0];
    }
    
 	for ( idx=1; idx<(CMDLINE_BUFFERSIZE - 1); idx++ ) {
        if ( (CmdlineBuffer[idx-1] == ' ') && (CmdlineBuffer[idx] != ' ')) {
            CmdlineBuffer[idx-1] = '\0';
            argv[nro_args++] =  (char *)&CmdlineBuffer[idx];
            if (nro_args == MAX_NUM_ARGS) 
                goto quit;
        }
    }    
   
quit:
        
    return(nro_args);
}
//------------------------------------------------------------------------------
bool cmdlineExtractPayload(char *dst ) 
{
    /*
     Estrae de la linea de comando el string encerrado entre los delimitadores
     */
  
uint8_t i;
uint8_t pos = 0;
//char dest[32];

    // Recien el tercer argumento es el que tiene el payload.
    if (argv[3] == NULL ) {
        return(false);
    }
    
    //memset(&dest, '\0', 32);

    for (i=3; i < MAX_NUM_ARGS; i++) {
        if (argv[i] == NULL ) {
            break;
        }
        //xprintf("i=%d\r\n", i );
       // xprintf("s=[%s]\r\n", argv[i]);
        //xprintf("pos=%d\r\n", pos);
        //strcpy(&dest[pos], argv[i] );
        strcpy( &dst[pos], argv[i] );
        pos += strlen(argv[i]);
        //dest[pos++] = ' ';
        //xprintf("DEST=%s\r\n", dest);
        dst[pos++] = ' ';
        //xprintf("DEST=%s\r\n", dst);  
    }
    dst[--pos] = '\0';
    return(true);     
}
//------------------------------------------------------------------------------
