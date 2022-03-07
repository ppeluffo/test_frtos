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

uint8_t count = 0;

	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );
    
    
	for( ;; )
	{
        xprintf("UART test01: %03d\n\r", count++);
        //xputChar('\r');
        //xputChar('\n');
		vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
        
	}
}
//------------------------------------------------------------------------------
