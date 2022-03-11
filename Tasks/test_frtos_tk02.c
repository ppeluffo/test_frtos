/*
 * File:   tkCtl.c
 * Author: pablo
 *
 * Created on 25 de octubre de 2021, 12:50 PM
 */


#include "test_frtos.h"
#include "math.h"

#define TKCTL_DELAY_S	1


//------------------------------------------------------------------------------
void tk02(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;
int16_t i;
char c;
float f;

	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );

    i = -1000;
    c = ' ';
    f=32.456;
    
    
	for( ;; )
	{
		vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
        led_flash();
        
        while ( xSemaphoreTake( sem_SYSVars, ( TickType_t ) 5 ) != pdTRUE )
  			taskYIELD();
        
        i++;
        c++;
        systemVars.iVal = i;
        systemVars.fVal = i*f;
        systemVars.rVal = rand();
        systemVars.cVal = c;
        if ( (i % 2) == 0 ) {
            strncpy( systemVars.sVal, "Numero PAR", MAX_LENGTH );
        } else {
            strncpy( systemVars.sVal, "Numero IMPAR", MAX_LENGTH );
        }
        
        xSemaphoreGive( sem_SYSVars );
        
        
	}
}
//------------------------------------------------------------------------------
