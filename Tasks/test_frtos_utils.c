/* 
 * File:   frtos20_utils.c
 * Author: pablo
 *
 * Created on 22 de diciembre de 2021, 07:34 AM
 */



#include "test_frtos.h"
#include "usart.h"
#include "string.h"
//#include "stdarg.h"
#include "stdio.h"
#include "printf.h"


#define PRINTF_BUFFER_SIZE        256U
static uint8_t stdout_buff[PRINTF_BUFFER_SIZE];

//------------------------------------------------------------------------------
int8_t WDT_init(void);
int8_t CLKCTRL_init(void);
void XPRINTF_init(void);

//-----------------------------------------------------------------------------
void system_init()
{
//	mcu_init();

	CLKCTRL_init();
    WDT_init();
    LED_init();
    XPRINTF_init();
    
}
//-----------------------------------------------------------------------------
int8_t WDT_init(void)
{
	/* 8K cycles (8.2s) */
	/* Off */
	ccp_write_io((void *)&(WDT.CTRLA), WDT_PERIOD_8KCLK_gc | WDT_WINDOW_OFF_gc );  
	return 0;
}
//-----------------------------------------------------------------------------
int8_t CLKCTRL_init(void)
{
	// Configuro el clock para 24Mhz
	
	ccp_write_io((void *)&(CLKCTRL.OSCHFCTRLA), CLKCTRL_FREQSEL_24M_gc         /* 24 */
	| 0 << CLKCTRL_AUTOTUNE_bp /* Auto-Tune enable: disabled */
	| 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),CLKCTRL_CLKSEL_OSCHF_gc /* Internal high-frequency oscillator */
	//		 | 0 << CLKCTRL_CLKOUT_bp /* System clock out: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKLOCK),0 << CLKCTRL_LOCKEN_bp /* lock enable: disabled */);

	return 0;
}
//-----------------------------------------------------------------------------
void reset(void)
{
	/* Issue a Software Reset to initilize the CPU */
	ccp_write_io( (void *)&(RSTCTRL.SWRR), RSTCTRL_SWRF_bm );   
}
//------------------------------------------------------------------------------
void LED_init(void)
{
	// Configura el pin del led como output
	LED_PORT.DIR |= LED_PIN_bm;	
	APAGAR_LED();
}
//------------------------------------------------------------------------------
void led_flash(void)
{
	// Prende el led 50ms y lo apaga
	PRENDER_LED();
	vTaskDelay( (TickType_t)( 100 / portTICK_PERIOD_MS ) );
	APAGAR_LED();
}
//------------------------------------------------------------------------------
void XPRINTF_init(void)
{
    USART4_init();
}
//------------------------------------------------------------------------------
int xprintf( const char *fmt, ...)
{

	va_list args;
	int i = 0;

	memset(stdout_buff,'\0',PRINTF_BUFFER_SIZE);
	va_start(args, fmt);
	vsnprintf( (char *)stdout_buff,sizeof(stdout_buff),fmt,args);
    //vsnprintf_( (char *)stdout_buff,sizeof(stdout_buff),fmt,args);
    //va_end();
	USART4_sendString( (char *)stdout_buff);

	return(i);

}
//------------------------------------------------------------------------------
void xputChar(unsigned char c)
{
   USART4_sendChar(c); 
}
//------------------------------------------------------------------------------