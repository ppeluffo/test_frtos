/*
 * drv_uart_spx.c
 *
 *  Created on: 11 jul. 2018
 *      Author: pablo
 * 
 * PLACA BASE: sensor_cloro.
 * 
 * El driver de las uart permite crear las uarts y 2 estructuras tipo 
 * ringbuffer (chars) para c/u.
 * Estos son las interfaces a la capa de FRTOS-IO.
 * Para transmitir se escribe en el ringBuffer de TX y para leer lo recibido
 * se lee del ringBuffer de RX.
 * La transmision / recepcion se hace por interrupcion. Estas ISR son 
 * provistas por el driver
 * Cada placa tiene diferente asignacion de puertos por lo tanto hay
 * que modificar el driver a c/placa.
 * 
 * 
 */

#include "drv_uart_avrDX.h"

//------------------------------------------------------------------------------
uart_control_t *drv_uart_init( uart_id_t iUART, uint32_t baudrate )
{
	// El puerto del USB es PORTD:
	// TXD pin = high
	// TXD pin output
	// baudrate / frame format
	// Enable TX,RX

uart_control_t *pUart = NULL;

	switch(iUART) {
	case iUART0:
		// Abro el puerto serial y fijo su velocidad
        PORTA.DIR &= ~PIN1_bm;
        PORTA.DIR |= PIN0_bm;
        USART0.BAUD = (uint16_t)USART_SET_BAUD_RATE(baudrate);    
        USART0.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;
		// Inicializo los ringBuffers que manejan el puerto. Son locales al driver.
		rBchar_CreateStatic( &uart_ctl_0.RXringBuffer, &uart0_rxStorage[0], UART0_RXSTORAGE_SIZE );
		rBchar_CreateStatic( &uart_ctl_0.TXringBuffer, &uart0_txStorage[0], UART0_TXSTORAGE_SIZE );
		// Asigno el identificador
		uart_ctl_0.uart_id = iUART0;
		uart_ctl_0.usart = &USART0;
		// Devuelvo la direccion de uart_gprs para que la asocie al dispositvo GPRS el frtos.
		pUart = (uart_control_t *)&uart_ctl_0;
		break;
        
	case iUART3:
		// Abro el puerto serial y fijo su velocidad
        PORTB.DIR &= ~PIN1_bm;
        PORTB.DIR |= PIN0_bm;
        USART3.BAUD = (uint16_t)USART_SET_BAUD_RATE(baudrate);     
        USART3.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;
		// Inicializo los ringBuffers que manejan el puerto. Son locales al driver.
		rBchar_CreateStatic( &uart_ctl_3.RXringBuffer, &uart3_rxStorage[0], UART3_RXSTORAGE_SIZE );
		rBchar_CreateStatic( &uart_ctl_3.TXringBuffer, &uart3_txStorage[0], UART3_TXSTORAGE_SIZE );
		// Asigno el identificador
		uart_ctl_3.uart_id = iUART3;
		uart_ctl_3.usart = &USART3;
		// Devuelvo la direccion de uart_gprs para que la asocie al dispositvo GPRS el frtos.
		pUart = (uart_control_t *)&uart_ctl_3;
		break; 
        
    case iUART4:
		// Abro el puerto serial y fijo su velocidad
        PORTE.DIR &= ~PIN1_bm;
        PORTE.DIR |= PIN0_bm;
        USART4.BAUD = (uint16_t)USART_SET_BAUD_RATE(baudrate);   
        USART4.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;
		// Inicializo los ringBuffers que manejan el puerto. Son locales al driver.
		rBchar_CreateStatic( &uart_ctl_4.RXringBuffer, &uart4_rxStorage[0], UART4_RXSTORAGE_SIZE );
		rBchar_CreateStatic( &uart_ctl_4.TXringBuffer, &uart4_txStorage[0], UART4_TXSTORAGE_SIZE );
		// Asigno el identificador
		uart_ctl_4.uart_id = iUART4;
		uart_ctl_4.usart = &USART4;
		// Devuelvo la direccion de uart_gprs para que la asocie al dispositvo GPRS el frtos.
		pUart = (uart_control_t *)&uart_ctl_4;
		break; 
	}

    // Lo hacemos por ioctl !!
    // Habilitamos el modulo de TX y RX
    //drv_uart_enable_tx(iUART);  // CTRLB:USART_TXEN
    //drv_uart_enable_rx(iUART);  // CTRLB:USART_RXEN
    //
    // Habilitamos la interrupcin de TX y RX
    //drv_uart_enable_tx_int(iUART);  // CTRLA: USART_TXCIE
    //drv_uart_enable_rx_int(iUART);  // CTRLA: USART_RXCIE
    //
	return(pUart);
}
//------------------------------------------------------------------------------
void drv_uart_interruptOn(uart_id_t iUART)
{
	// Habilito la interrupcion TX del UART lo que hace que se ejecute la ISR_TX y
	// esta vaya a la TXqueue y si hay datos los comienze a trasmitir.

    drv_uart_enable_tx_int(iUART);

}
//------------------------------------------------------------------------------
void drv_uart_interruptOff(uart_id_t iUART)
{

    drv_uart_disable_tx_int(iUART);

}
//------------------------------------------------------------------------------
void drv_uart_enable_tx_int( uart_id_t iUART )
{
	// Habilita la interrrupcion por DRE

	switch(iUART) {
	case iUART0:
		USART0.CTRLA |= USART_DREIE_bm;
		break;
    case iUART3:
		USART3.CTRLA |= USART_DREIE_bm;
		break;
    case iUART4:
		USART4.CTRLA |= USART_DREIE_bm;
		break;        
	}
}
//------------------------------------------------------------------------------
void drv_uart_disable_tx_int( uart_id_t iUART )
{
	// Deshabilita la interrrupcion por DRE

	switch(iUART) {
	case iUART0:
		USART0.CTRLA &= USART_DREIE_bm;
		break;
    case iUART3:
		USART3.CTRLA &= USART_DREIE_bm;
		break;
    case iUART4:
		USART4.CTRLA &= USART_DREIE_bm;
		break;
	}
}
//------------------------------------------------------------------------------
void drv_uart_enable_rx_int( uart_id_t iUART )
{
	// Habilita la interrrupcion por RXC

	switch(iUART) {
	case iUART0:
		USART0.CTRLA |= USART_RXCIE_bm;
		break;
    case iUART3:
		USART3.CTRLA |= USART_RXCIE_bm;
		break;
    case iUART4:
		USART4.CTRLA |= USART_RXCIE_bm;
		break;
	}
}
//------------------------------------------------------------------------------
void drv_uart_disable_rx_int( uart_id_t iUART )
{
	// Deshabilita la interrrupcion por RXC

	switch(iUART) {
	case iUART0:
		USART0.CTRLA &= ~USART_RXCIE_bm;
		break;
    case iUART3:
		USART3.CTRLA &= ~USART_RXCIE_bm;
		break;
    case iUART4:
		USART4.CTRLA &= ~USART_RXCIE_bm;
		break;
	}
}
//------------------------------------------------------------------------------
void drv_uart_enable_tx( uart_id_t iUART )
{
	// Enable USART transmitter

	switch(iUART) {
	case iUART0:
		USART0.CTRLB |= USART_TXEN_bm;
		break;
    case iUART3:
		USART3.CTRLB |= USART_TXEN_bm;
		break;
    case iUART4:
		USART4.CTRLB |= USART_TXEN_bm;
		break;
	}

}
//------------------------------------------------------------------------------
void drv_uart_disable_tx( uart_id_t iUART )
{
	// Disable USART transmitter

	switch(iUART) {
	case iUART0:
		USART0.CTRLB &= ~USART_TXEN_bm;
		break;
    case iUART3:
		USART3.CTRLB &= ~USART_TXEN_bm;
		break;
    case iUART4:
		USART4.CTRLB &= ~USART_TXEN_bm;
		break;
	}
}
//------------------------------------------------------------------------------
void drv_uart_enable_rx( uart_id_t iUART )
{
	// Enable USART receiver

	switch(iUART) {
	case iUART0:
		USART0.CTRLB |= USART_RXEN_bm;
		break;
    case iUART3:
		USART3.CTRLB |= USART_RXEN_bm;
		break;
    case iUART4:
		USART4.CTRLB |= USART_RXEN_bm;
		break;
	}
}
//------------------------------------------------------------------------------
void drv_uart_disable_rx( uart_id_t iUART )
{
	// Disable USART receiver

	switch(iUART) {
	case iUART0:
		USART0.CTRLB &= ~USART_RXEN_bm;
		break;
    case iUART3:
		USART3.CTRLB &= ~USART_RXEN_bm;
		break;
    case iUART4:
		USART4.CTRLB &= ~USART_RXEN_bm;
		break;
	}
}
//------------------------------------------------------------------------------
// USART3: Terminal 
//------------------------------------------------------------------------------
ISR(USART3_DRE_vect)
{

char cChar = ' ';
int8_t res = false;

	res = rBchar_Pop( &uart_ctl_3.TXringBuffer, (char *)&cChar );

	if( res == true ) {
		// Send the next character queued for Tx
		USART3.TXDATAL = cChar;
	} else {
		// Queue empty, nothing to send.
		drv_uart_interruptOff(uart_ctl_3.uart_id);
	}
}
//------------------------------------------------------------------------------
ISR(USART3_RXC_vect)
{

char cChar = ' ';

	cChar = USART3.RXDATAL;
 	if( rBchar_PokeFromISR( &uart_ctl_3.RXringBuffer, &cChar ) ) {
		taskYIELD();
	}
}
//------------------------------------------------------------------------------
// USART4: LORA
//------------------------------------------------------------------------------
ISR(USART4_DRE_vect)
{

char cChar = ' ';
int8_t res = false;

	res = rBchar_Pop( &uart_ctl_4.TXringBuffer, (char *)&cChar );

	if( res == true ) {
		// Send the next character queued for Tx
		USART4.TXDATAL = cChar;
	} else {
		// Queue empty, nothing to send.
		drv_uart_interruptOff(uart_ctl_4.uart_id);
	}
}
//------------------------------------------------------------------------------
ISR(USART4_RXC_vect)
{

char cChar = ' ';

	cChar = USART4.RXDATAL;
 	if( rBchar_PokeFromISR( &uart_ctl_4.RXringBuffer, &cChar ) ) {
		taskYIELD();
	}
}
//------------------------------------------------------------------------------


