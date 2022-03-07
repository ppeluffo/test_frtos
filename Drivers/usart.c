/*
 * usart.c
 *
 * Created: 21/10/2020 12:21:17
 *  Author: Pablo
 */ 

#include "clock_config.h"
#include <util/delay.h>
#include <xc.h>
#include "stdbool.h"
#include "string.h"
#include <avr/wdt.h>
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"

#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5);
#define USART4_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5);
// ---------------------------------------------------------------
void USART0_init(void)
{
	// Configuro para 115200
	
	PORTA.DIR &= ~PIN1_bm;
	PORTA.DIR |= PIN0_bm;
	
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(115200);
	USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}
// ---------------------------------------------------------------
void USART0_sendChar(char c)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART0.TXDATAL = c;	
}
// ---------------------------------------------------------------
void USART0_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART0_sendChar(str[i]);
	}
}
// ---------------------------------------------------------------
char USART0_readChar(bool echo)
{
char c;

	while (!(USART0.STATUS & USART_RXCIF_bm))
	{
		wdt_reset();
		;
	}
	c = USART0.RXDATAL;
	if ( echo)
		USART0_sendChar(c);
	return(c);
}
// ---------------------------------------------------------------
bool USART0_getChar( char *c )
{

	if ( USART0.STATUS & USART_RXCIF_bm) {
		*c = USART0.RXDATAL;
		return(true);
	}
	return(false);
}
// ---------------------------------------------------------------
void USART4_init(void)
{
	// Configuro para 115200
	
	PORTE.DIR &= ~PIN1_bm;
	PORTE.DIR |= PIN0_bm;
	
	USART4.BAUD = (uint16_t)USART4_BAUD_RATE(115200);
	USART4.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}
// ---------------------------------------------------------------
void USART4_sendChar(char c)
{
	while (!(USART4.STATUS & USART_DREIF_bm))
	{
		taskYIELD();
	}
    
	USART4.TXDATAL = c;
         
}
// ---------------------------------------------------------------
void USART4_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART4_sendChar(str[i]);
	}
    
    //while ( (USART4.STATUS & USART_TXCIF_bm) > 0 )
    //    ;
    
    /* 
     * Agrego un busy loop para que termine de transmitir el ultimo byte
     * antes de irse a dormir con tickless
     */
    _delay_ms(50);
}
// ---------------------------------------------------------------
char USART4_readChar(bool echo)
{
char c;

	while (!(USART4.STATUS & USART_RXCIF_bm))
        ;
	c = USART4.RXDATAL;
	if ( echo)
		USART4_sendChar(c);
	return(c);
}
// ---------------------------------------------------------------
bool USART4_getChar( char *c )
{

	if ( USART4.STATUS & USART_RXCIF_bm) {
		*c = USART4.RXDATAL;
		return(true);
	}
	return(false);
}
// ---------------------------------------------------------------
/**
 * \brief Check if the usart can accept data to be transmitted
 *
 * \return The status of USART TX data ready check
 * \retval false The USART can not receive data to be transmitted
 * \retval true The USART can receive data to be transmitted
 */
bool USART_4_is_tx_ready(void)
{
	return (USART4.STATUS & USART_DREIF_bm);
}
// ---------------------------------------------------------------
/**
 * \brief Check if USART_0 data is transmitted
 *
 * \return Receiver ready status
 * \retval true  Data is not completely shifted out of the shift register
 * \retval false Data completely shifted out if the USART shift register
 */
bool USART_4_is_tx_busy(void)
{
	return (!(USART4.STATUS & USART_TXCIF_bm));
}