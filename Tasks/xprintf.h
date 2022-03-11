/* 
 * File:   xprintf.h
 * Author: pablo
 *
 * Created on 8 de marzo de 2022, 10:55 AM
 */

#ifndef XPRINTF_H
#define	XPRINTF_H

#ifdef	__cplusplus
extern "C" {
#endif


void XPRINTF_init(void);
int xprintf( const char *fmt, ...);
void xputChar(unsigned char c);
int xputs( char *str );
void xputsCRLF(void);


#ifdef	__cplusplus
}
#endif

#endif	/* XPRINTF_H */

