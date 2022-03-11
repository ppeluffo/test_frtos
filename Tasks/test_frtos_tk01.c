/*
 * File:   tkTerm.c
 * Author: pablo
 *
 * Created on 25 de octubre de 2021, 12:50 PM
 */


#include "test_frtos.h"

//------------------------------------------------------------------------------
void tk01(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;

	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );
    
    
	for( ;; )
	{

		vTaskDelay( ( TickType_t)( 5000 / portTICK_PERIOD_MS ) );
		wdt_reset();
       
        
	}
}
//------------------------------------------------------------------------------
