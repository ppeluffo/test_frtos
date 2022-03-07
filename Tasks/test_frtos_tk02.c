/*
 * File:   tkCtl.c
 * Author: pablo
 *
 * Created on 25 de octubre de 2021, 12:50 PM
 */


#include "test_frtos.h"

#define TKCTL_DELAY_S	1


//------------------------------------------------------------------------------
void tk02(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;

	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );

    
	for( ;; )
	{
		vTaskDelay( ( TickType_t)( 3000 / portTICK_PERIOD_MS ) );
        led_flash();
	}
}
//------------------------------------------------------------------------------
