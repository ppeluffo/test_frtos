/*
 * File:   tk03.c
 * Author: pablo
 *
 * Created on 25 de octubre de 2021, 12:50 PM
 */


#include "test_frtos.h"

//------------------------------------------------------------------------------
void tk03(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;

//uint8_t count = 0;

	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );
    
    
	for( ;; )
	{
        
        /*
        while ( xSemaphoreTake( sem_SYSVars, ( TickType_t ) 5 ) != pdTRUE )
  			taskYIELD();
        
        xprintf("V4d: UART test: %03d\r\n", count++);
        //xprintf("INT16: %d\r\n", systemVars.iVal);
        //xprintf("UINT16(rand): %d\r\n", systemVars.rVal);
        //xprintf("CHAR: %c\r\n", systemVars.cVal);
        xprintf("FLOAT: %0.4f\r\n", systemVars.fVal);
        xprintf("STRING: [%s]\r\n", systemVars.sVal);
        
        xprintf("INT16: %d, UINT16(rand): %d, CHAR: %c, FLOAT: %0.4f \r\n",systemVars.iVal,systemVars.rVal, systemVars.cVal, systemVars.fVal  );
        xprintf("\r\n");
     
        //xputChar('\r');
        //xputChar('\n');
        
        xSemaphoreGive( sem_SYSVars );
        */
        
		vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
        
	}
}
//------------------------------------------------------------------------------
