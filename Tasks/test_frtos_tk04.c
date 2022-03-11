#include "test_frtos.h"
//#include "cmdline.h"
#include "frtos_cmd.h"

static void cmdClsFunction(void);
static void cmdHelpFunction(void);
static void cmdResetFunction(void);
static void cmdStatusFunction(void);
static void cmdWriteFunction(void);
static void cmdReadFunction(void);
static void cmdTestFunction(void);

static void pv_snprintfP_OK(void );
static void pv_snprintfP_ERR(void );


#define MAX_NUM_ARGS            16
#define MAX_LENGTH_ARGS         64
char *argv[MAX_NUM_ARGS];
char test_buffer[MAX_LENGTH_ARGS];

#define PAYLOAD_SIZE    32


//------------------------------------------------------------------------------
void tk04(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;

	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );

uint8_t c = 0;
uint8_t ticks = 0;

    FRTOS_CMD_init();

    FRTOS_CMD_register( "cls", cmdClsFunction );
	FRTOS_CMD_register( "help", cmdHelpFunction );
    FRTOS_CMD_register( "reset", cmdResetFunction );
    FRTOS_CMD_register( "status", cmdStatusFunction );
    FRTOS_CMD_register( "write", cmdWriteFunction );
    FRTOS_CMD_register( "read", cmdReadFunction );
    FRTOS_CMD_register( "test", cmdTestFunction );
    
    
	// Fijo el timeout del READ
	ticks = 5;
	frtos_ioctl( fdTERM, ioctl_SET_TIMEOUT, &ticks );

	xprintf( "\r\n\r\nstarting tkCmd VER: %s %s..\r\n", FW_REV, FW_DATE );
    
	// loop
	for( ;; )
	{
		c = '\0';	// Lo borro para que luego del un CR no resetee siempre el timer.
		// el read se bloquea 50ms. lo que genera la espera.
		while ( frtos_read( fdTERM, (char *)&c, 1 ) == 1 ) {
            FRTOS_CMD_process(c);
        }
        //vTaskDelay( ( TickType_t)( 10 / portTICK_PERIOD_MS ) );
	}    
}
//------------------------------------------------------------------------------
static void cmdHelpFunction(void)
{

    FRTOS_CMD_makeArgv();
        
    if ( strcmp( strupr(argv[1]), "WRITE") == 0 ) {
		xprintf( "-write:\r\n");
        xprintf("   lora cts,reset {on|off}\r\n");
        xprintf("   lora cmd\r\n");
        xprintf("   flush, led, dac {val}\r\n");
        
    }  else if ( strcmp( strupr(argv[1]), "CONFIG") == 0 ) {
		xprintf("-config:\r\n");
		xprintf("   default,save\r\n");

    }  else if ( strcmp( strupr(argv[1]), "READ") == 0 ) {
		xprintf("-read:\r\n");
		xprintf("   eeprom\r\n");
        xprintf("   lora rsp, buffer\r\n");
        xprintf("   ina {regName}\r\n");
        
    }  else {
        // HELP GENERAL
        xprintf("Available commands are:\r\n");
        xprintf("-cls\r\n");
        xprintf("-help\r\n");
        xprintf("-status\r\n");
        xprintf("-reset\r\n");
        xprintf("-write...\r\n");
        xprintf("-config...\r\n");
        xprintf("-read...\r\n");

    }
   
	xprintf("Exit help \r\n");

}
//------------------------------------------------------------------------------
static void cmdClsFunction(void)
{
	// ESC [ 2 J
	xprintf("\x1B[2J\0");
}
//------------------------------------------------------------------------------
static void cmdResetFunction(void)
{

    reset();
}
//------------------------------------------------------------------------------
static void cmdStatusFunction(void)
{

    // https://stackoverflow.com/questions/12844117/printing-defined-constants
    
	xprintf("\r\nSpymovil %s %s %s %s \r\n" , HW_MODELO, FRTOS_VERSION, FW_REV, FW_DATE);
	
}
//------------------------------------------------------------------------------
static void cmdWriteFunction(void)
{

    FRTOS_CMD_makeArgv();
    
    if ( strcmp( strupr(argv[1]),"INT") == 0 ) {
        systemVars.iVal = atoi(argv[2]);
        pv_snprintfP_OK();
        return;
    }

    if ( strcmp( strupr(argv[1]),"FLOAT") == 0 ) {
        systemVars.fVal = atof(argv[2]);
        pv_snprintfP_OK();
        return;
    }    
    
    if ( strcmp( strupr(argv[1]),"CHAR") == 0 ) {
        systemVars.cVal = argv[2][0];
        pv_snprintfP_OK();
        return;
    }
    
    if ( strcmp( strupr(argv[1]),"STRING") == 0 ) {
        strncpy( systemVars.sVal, argv[2], MAX_LENGTH);
        pv_snprintfP_OK();
        return;
    }
        
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
 
}
//------------------------------------------------------------------------------
static void cmdReadFunction(void)
{
  
    FRTOS_CMD_makeArgv();
    
    if ( strcmp( strupr(argv[1]),"INT") == 0 ) {
        xprintf("INT=%d\r\n", systemVars.iVal);
        pv_snprintfP_OK();
        return;
    }

    if ( strcmp( strupr(argv[1]),"FLOAT") == 0 ) {
        xprintf("FLOAT=%d\r\n", systemVars.fVal);
        pv_snprintfP_OK();
        return;
    }    
    
    if ( strcmp( strupr(argv[1]),"CHAR") == 0 ) {
        xprintf("CHAR=%c\r\n", systemVars.cVal);
        pv_snprintfP_OK();
        return;
    }
    
    if ( strcmp( strupr(argv[1]),"STRING") == 0 ) {
        xprintf("STRING=%s\r\n", systemVars.sVal);
        pv_snprintfP_OK();
        return;
    }
        
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
 
}
//------------------------------------------------------------------------------
static void cmdTestFunction(void)
{
  
uint8_t nro_args;

    nro_args = FRTOS_CMD_makeArgv();
   
    if ( strcmp( strupr(argv[1]),"PUTS") == 0 ) {
        while(nro_args > 0) {
            xputs(argv[nro_args]);
            xputsCRLF();
            nro_args--;
        }
        return;
    }
    
    if ( strcmp( strupr(argv[1]),"XPRINTF1") == 0 ) {
        while(nro_args > 0) {
            xprintf(argv[nro_args]);
            xputsCRLF();
            nro_args--;
        }
        return;
    }

    if ( strcmp( strupr(argv[1]),"XPRINTF2") == 0 ) {
        while(nro_args > 0) {
            xprintf("%s\r\n", argv[nro_args]);
            nro_args--;
        }
        return;
    }

    if ( strcmp( strupr(argv[1]),"XPRINTF3") == 0 ) {
        xprintf("%s %s %s\r\n", argv[0], argv[1], argv[2]);
        return;
    }
  
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
    
}
//------------------------------------------------------------------------------
static void pv_snprintfP_OK(void )
{
	xprintf("ok\r\n\0");
}
//------------------------------------------------------------------------------
static void pv_snprintfP_ERR(void)
{
	xprintf("error\r\n\0");
}
//------------------------------------------------------------------------------

