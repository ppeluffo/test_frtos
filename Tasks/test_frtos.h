/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#ifndef F_CPU
#define F_CPU 24000000
#endif


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "croutine.h"
#include "semphr.h"
#include "timers.h"
#include "limits.h"
#include "portable.h"

#include "protected_io.h"
#include "ccp.h"

#include <avr/io.h>
#include <avr/builtins.h>
#include <avr/wdt.h> 
#include <avr/pgmspace.h>
        
#include "stdbool.h"
#include "frtos-io.h"
#include "xprintf.h"

#define FW_REV "1.0.0a"
#define FW_DATE "@ 20220311"
#define HW_MODELO "Tcesting FRTOS R001 HW:AVR128DA64"
#define FRTOS_VERSION "FW:FreeRTOS V202111.00"

#define SYSMAINCLK 24

#define tk01_TASK_PRIORITY	 	( tskIDLE_PRIORITY + 1 )
#define tk02_TASK_PRIORITY	 	( tskIDLE_PRIORITY + 1 )
#define tk03_TASK_PRIORITY	 	( tskIDLE_PRIORITY + 1 )
#define tk04_TASK_PRIORITY	 	( tskIDLE_PRIORITY + 1 )

#define tk01_STACK_SIZE		384
#define tk02_STACK_SIZE		384
#define tk03_STACK_SIZE		384
#define tk04_STACK_SIZE		384

StaticTask_t tk01_Buffer_Ptr;
StackType_t tk01_Buffer [tk01_STACK_SIZE];

StaticTask_t tk02_Buffer_Ptr;
StackType_t tk02_Buffer [tk02_STACK_SIZE];

StaticTask_t tk03_Buffer_Ptr;
StackType_t tk03_Buffer [tk03_STACK_SIZE];

StaticTask_t tk04_Buffer_Ptr;
StackType_t tk04_Buffer [tk04_STACK_SIZE];

SemaphoreHandle_t sem_SYSVars;
StaticSemaphore_t SYSVARS_xMutexBuffer;
#define MSTOTAKESYSVARSSEMPH ((  TickType_t ) 10 )


TaskHandle_t xHandle_tk01, xHandle_tk02, xHandle_tk03, xHandle_tk04;

void tk01(void * pvParameters);
void tk02(void * pvParameters);
void tk03(void * pvParameters);
void tk04(void * pvParameters);

void system_init();
void reset(void);

#define LED_PORT	PORTD
#define LED_PIN_bm	PIN5_bm

#define PRENDER_LED() ( LED_PORT.OUT |= LED_PIN_bm )
#define APAGAR_LED() ( LED_PORT.OUT &= ~LED_PIN_bm )

void LED_init(void);
void led_flash(void);

int xprintf( const char *fmt, ...);
void xputChar(unsigned char c);

#define MAX_LENGTH 32

struct {
    int16_t iVal;
    float fVal;
    uint16_t rVal;
    char sVal[MAX_LENGTH];
    char cVal;
} systemVars;


#endif	/* XC_HEADER_TEMPLATE_H */

