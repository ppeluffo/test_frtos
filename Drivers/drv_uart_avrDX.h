/*
 * drv_uart_spx.h
 *
 *  Created on: 8 dic. 2018
 *      Author: pablo
 */

#ifndef SRC_SPX_DRIVERS_DRV_UART_SPX_H_
#define SRC_SPX_DRIVERS_DRV_UART_SPX_H_

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"

#include "ringBuffer.h"
#include <xc.h>

#ifndef F_CPU
#define F_CPU 24000000
#endif

#define USART_SET_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5);

//-----------------------------------------------------------------------
#define UART0_RXSTORAGE_SIZE	64
#define UART0_TXSTORAGE_SIZE	8	// trasmito por poleo. Si uso interrupcion lo subo a 128
uint8_t uart0_rxStorage[UART0_RXSTORAGE_SIZE];
uint8_t uart0_txStorage[UART0_TXSTORAGE_SIZE];

#define UART3_RXSTORAGE_SIZE	64
#define UART3_TXSTORAGE_SIZE	8	// trasmito por poleo. Si uso interrupcion lo subo a 128
uint8_t uart3_rxStorage[UART3_RXSTORAGE_SIZE];
uint8_t uart3_txStorage[UART3_TXSTORAGE_SIZE];

#define UART4_RXSTORAGE_SIZE	64
#define UART4_TXSTORAGE_SIZE	8	// trasmito por poleo. Si uso interrupcion lo subo a 128
uint8_t uart4_rxStorage[UART4_RXSTORAGE_SIZE];
uint8_t uart4_txStorage[UART4_TXSTORAGE_SIZE];


// Enumenerador de los puertos fisicos.
typedef enum {
	iUART0 = 0,
    iUART3,
    iUART4,
} uart_id_t;

// Estructura generica de una UART
typedef struct {
	uart_id_t uart_id;			// Identificador de la uart fisico
	rBchar_s TXringBuffer;	// ringbuffer de trasmision
	rBchar_s RXringBuffer;	// ringbuffer de recepcion.
	USART_t *usart;
} uart_control_t;

// Creo las uart's en memoria.
uart_control_t uart_ctl_0, uart_ctl_3, uart_ctl_4;

//-----------------------------------------------------------------------
uart_control_t *drv_uart_init( uart_id_t iUART, uint32_t baudrate );
void drv_uart_interruptOn(uart_id_t iUART);
void drv_uart_interruptOff(uart_id_t iUART);

void drv_uart_enable_tx_int( uart_id_t iUART );
void drv_uart_disable_tx_int( uart_id_t iUART );

void drv_uart_enable_rx_int( uart_id_t iUART );
void drv_uart_disable_rx_int( uart_id_t iUART );

void drv_uart_enable_tx( uart_id_t iUART );
void drv_uart_disable_tx( uart_id_t iUART );

void drv_uart_enable_rx( uart_id_t iUART );
void drv_uart_disable_rx( uart_id_t iUART );

//-----------------------------------------------------------------------


#endif /* SRC_SPX_DRIVERS_DRV_UART_SPX_H_ */
