


#include "xprintf.h"
#include "string.h"
#include "stdio.h"
#include "frtos-io.h"
#include "clock_config.h"
#include <util/delay.h>
#include "printf.h"
#include "semphr.h"

#define PRINTF_BUFFER_SIZE        256U
static uint8_t stdout_buff[PRINTF_BUFFER_SIZE];

SemaphoreHandle_t sem_STDOUT;
StaticSemaphore_t STDOUT_xMutexBuffer;

//------------------------------------------------------------------------------
void XPRINTF_init(void)
{
   // Version 4: driver c/interrupcion
   // La UART queda abierta con frtos_open()
    sem_STDOUT = xSemaphoreCreateMutexStatic( &STDOUT_xMutexBuffer );
    // Verision 3:
   // USART4_init();
   
}
//------------------------------------------------------------------------------
int xprintf( const char *fmt, ...)
{
 
va_list args;
int i = 0;

	// Espero el semaforo del buffer en forma persistente.
	while ( xSemaphoreTake( sem_STDOUT, ( TickType_t ) 5 ) != pdTRUE )
		vTaskDelay( ( TickType_t)( 5 ) );

	memset(stdout_buff,'\0',PRINTF_BUFFER_SIZE);
	va_start(args, fmt);
	//vsnprintf( (char *)stdout_buff,sizeof(stdout_buff),fmt,args);
    vsnprintf_( (char *)stdout_buff,sizeof(stdout_buff),fmt,args);
    //va_end();

    //Version 3
	//USART4_sendString( (char *)stdout_buff);
    
    // Version 4
    i = frtos_write(fdTERM, (char *)stdout_buff, strlen((char *)stdout_buff) );
    _delay_ms(10);
    
    xSemaphoreGive( sem_STDOUT );
    
	return(i);

}
//------------------------------------------------------------------------------
int xputs( char *str )
{
 
int i = 0;

	while ( xSemaphoreTake( sem_STDOUT, ( TickType_t ) 5 ) != pdTRUE )
		vTaskDelay( ( TickType_t)( 5 ) );

	memset(stdout_buff,'\0',PRINTF_BUFFER_SIZE);
    strncpy((char *)stdout_buff, str, PRINTF_BUFFER_SIZE );
    i = frtos_write(fdTERM, (char *)stdout_buff, strlen((char *)stdout_buff) );
    _delay_ms(50);
    
    xSemaphoreGive( sem_STDOUT );
    
	return(i);

}
//------------------------------------------------------------------------------
void xputsCRLF(void)
{

char str_crlf[] = "\r\n";

	while ( xSemaphoreTake( sem_STDOUT, ( TickType_t ) 5 ) != pdTRUE )
		vTaskDelay( ( TickType_t)( 5 ) );

	memset(stdout_buff,'\0',PRINTF_BUFFER_SIZE);
    strncpy((char *)stdout_buff, str_crlf, PRINTF_BUFFER_SIZE );
    frtos_write(fdTERM, (char *)stdout_buff, strlen((char *)stdout_buff) );
    _delay_ms(50);
    
    xSemaphoreGive( sem_STDOUT );
	return;    
}
//------------------------------------------------------------------------------
void xputChar(unsigned char c)
{
    
char cChar;

	cChar = c;
    
   // Version 3
   //USART4_sendChar(c); 
    
   // Version 4
   frtos_write( fdTERM, &cChar, 1 );
   
}
//------------------------------------------------------------------------------