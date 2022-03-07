/*
 * File:   frtos10_main.c
 * Author: pablo
 *
 * Created on 25 de octubre de 2021, 11:55 AM
 * 
 * Debido a que es un micro nuevo, conviene ir a https://start.atmel.com/ y
 * crear un projecto con todos los perifericos que usemos y bajar el codigo
 * para ver como se inicializan y se manejan.
 * 
 * -----------------------------------------------------------------------------
 * Pendiente:
 * - Implementar el cmdMode(async) de modo de leer en buffers.
 * - Probar los printf con los buffers leidos por cmdMode
 * - Implementar la entrada/salida serial con FRTOS-IO
 * -----------------------------------------------------------------------------
 * Version 3: Implementamos una salida serial directa con printf.
 * Agregamos el directorio Drivers con los archivos usart.c y usart.h donde
 * implemento las funciones de lectura/escritura de los puertos seriales 0 y 4.
 * El manejo es por poleo.
 * La placa donde estamos probando usa el puerto 4.
 * Creo una nueva tarea tk3 que se va a encargar de imprimir un string y donde vamos
 * a probar las funciones de printf.
 * Utilizo la funcion vsnprintf del XC8
 * Formatea bien pero se come el ultimo caracter a veces.
 * El problema es que al configurar para TICKLESS, luego de poner el ultimo
 * byte se duerme y por eso a veces no lo transmite.
 * Para evitarlo, luego de transmitir el buffer debo agregar un busy-loop
 * con _delay_ms(50) de modo que no entre en sleep y pueda vaciarse los buffers.
 * 
 * 
 * -----------------------------------------------------------------------------
 * Version 2: Modo Tickless
 * Al codigo anterior le agregamos en FreeRTOSConfig.h la linea
 * #define configUSE_TICKLESS_IDLE     1
 * 
 * -----------------------------------------------------------------------------
 * Version 1: Modo normal ( no pwrsave )
 * Tiene 2 tareas:
 * - Una resetea el watchdog c/5s.
 * - La otra prende y apaga el led.
 * 
 * -----------------------------------------------------------------------------
 * Version 0 @ 2022-03-07.
 * Este proyecto es para probar el funcionamiento del FRTOS en la plataforma 
 * AVR128DA64 que es la que usamos en nuestros desarrollos
 * En particular lo ajustamos a la placa del sensor de cloro.
 * 
 * 
 * El resultado es que la placa "sensor de cloro" reduce el consumo de 11 a 7mA.
 * ( la placa no esta optimizada para reducir el consumo ).
 *  
 */

#include <avr/io.h>

#include <avr/io.h>

FUSES = {
	.WDTCFG = 0x0B, // WDTCFG {PERIOD=8KCLK, WINDOW=OFF}
	.BODCFG = 0x00, // BODCFG {SLEEP=DISABLE, ACTIVE=DISABLE, SAMPFREQ=128Hz, LVL=BODLEVEL0}
	.OSCCFG = 0xF8, // OSCCFG {CLKSEL=OSCHF}
	.SYSCFG0 = 0xD2, // SYSCFG0 {EESAVE=CLEAR, RSTPINCFG=GPIO, CRCSEL=CRC16, CRCSRC=NOCRC}
	.SYSCFG1 = 0xF8, // SYSCFG1 {SUT=0MS}
	.CODESIZE = 0x00, // CODESIZE {CODESIZE=User range:  0x0 - 0xFF}
	.BOOTSIZE = 0x00, // BOOTSIZE {BOOTSIZE=User range:  0x0 - 0xFF}
};

LOCKBITS = 0x5CC5C55C; // {KEY=NOLOCK}


#include "test_frtos.h"


//------------------------------------------------------------------------------
int main(void) {


    system_init();
    
    
    xHandle_tk01 = xTaskCreateStatic( tk01, "TK01", tk01_STACK_SIZE, (void *)1, tk01_TASK_PRIORITY, tk01_Buffer, &tk01_Buffer_Ptr );
    xHandle_tk02 = xTaskCreateStatic( tk02, "TK02", tk02_STACK_SIZE, (void *)1, tk02_TASK_PRIORITY, tk02_Buffer, &tk02_Buffer_Ptr );
    xHandle_tk03 = xTaskCreateStatic( tk03, "TK03", tk03_STACK_SIZE, (void *)1, tk03_TASK_PRIORITY, tk03_Buffer, &tk03_Buffer_Ptr );

    /* Arranco el RTOS. */
	vTaskStartScheduler();
  
	// En caso de panico, aqui terminamos.
//	exit (1);
    
}
//------------------------------------------------------------------------------
/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
//------------------------------------------------------------------------------------
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
//-----------------------------------------------------------------------------


