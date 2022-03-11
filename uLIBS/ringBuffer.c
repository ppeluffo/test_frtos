/*
 * l_ringBuffer.c
 *
 *  Created on: 11 jul. 2018
 *      Author: pablo
 */


#include "ringBuffer.h"


//------------------------------------------------------------------------------------
/*
 *  RING BUFFERS DE ESTRUCTURAS
 */
void rBstruct_CreateStatic ( rBstruct_s *rB, void *storage_area, uint16_t buffersize, uint16_t elementsize  )
{
	rB->buff = storage_area;
	rB->head = 0;	// start
	rB->tail = 0;	// end
	rB->count = 0;
	rB->length = buffersize;
	rB->elementsize = elementsize;

}
//------------------------------------------------------------------------------------
bool rBstruct_Poke( rBstruct_s *rB, void *element )
{

	// Coloco una estructura presionTask ( put_on_top ) en la FIFO

bool ret = false;
void *p;

	taskENTER_CRITICAL();

	// Si el buffer esta vacio ajusto los punteros
	if( rB->count == 0) {
		rB->head = rB->tail = 0;
	}

	if ( rB->count < rB->length ) {

		p =  (rB->buff);
		p += sizeof(uint8_t)*( rB->head * rB->elementsize);

		memcpy( p, element, rB->elementsize );
		++rB->count;
		// Avanzo en modo circular
		rB->head = ( rB->head  + 1 ) % ( rB->length );
		ret = true;
    }

	taskEXIT_CRITICAL();
	return(ret);

}
//------------------------------------------------------------------------------------
bool rBstruct_Pop( rBstruct_s *rB, void *element )
{

	// Saco un valor ( get_from_tail ) de la FIFO.

bool ret = false;
void *p;

	taskENTER_CRITICAL();
	//  Si el buffer esta vacio retorno.
	if( rB->count == 0) {
		rB->head = rB->tail = 0;
		taskEXIT_CRITICAL();
		return(ret);
	}

	p =  (rB->buff);
	p += sizeof(uint8_t)*( rB->tail * rB->elementsize);

	memcpy( element, p, rB->elementsize );
	--rB->count;
	// Avanzo en modo circular
	rB->tail = ( rB->tail  + 1 ) % ( rB->length );
	ret = true;

	taskEXIT_CRITICAL();
	return(ret);
}
//------------------------------------------------------------------------------------
bool rBstruct_PopRead( rBstruct_s *rB, void *element )
{

	// Leo un valor ( red_from_tail ) de la FIFO.

bool ret = false;
void *p;

	taskENTER_CRITICAL();
	//  Si el buffer esta vacio retorno.
	if( rB->count == 0) {
		rB->head = rB->tail = 0;
		taskEXIT_CRITICAL();
		return(ret);
	}

	p =  (rB->buff);
	p += sizeof(uint8_t)*( rB->tail * rB->elementsize);

	memcpy( element, p, rB->elementsize );
	// No muevo el puntero

	ret = true;

	taskEXIT_CRITICAL();
	return(ret);
}
//------------------------------------------------------------------------------------
void rBstruct_Flush( rBstruct_s *rB )
{

	rB->head = 0;
	rB->tail = 0;
	rB->count = 0;
	memset( (rB->buff), '\0', (rB->length * rB->elementsize) );
}
//------------------------------------------------------------------------------------
uint16_t rBstruct_GetCount( rBstruct_s *rB )
{

	return(rB->count);

}
//------------------------------------------------------------------------------------
uint16_t rBstruct_GetFreeCount( rBstruct_s *rB )
{

	return(rB->length - rB->count);

}
//------------------------------------------------------------------------------------
/*
 *  RING BUFFERS DE CHAR
 */
bool rBchar_Poke( rBchar_s *rB, char *cChar )
{

bool ret = false;

	taskENTER_CRITICAL();

	// Si el buffer esta vacio ajusto los punteros
	if( rB->count == 0) {
		rB->head = rB->tail = 0;
	}

	if ( rB->count < rB->length ) {
		rB->buff[rB->head] = *cChar;
		++rB->count;
		// Avanzo en modo circular
		rB->head = ( rB->head  + 1 ) % ( rB->length );
		ret = true;
    }

	taskEXIT_CRITICAL();
	return(ret);

}
//------------------------------------------------------------------------------------
bool rBchar_PokeFromISR( rBchar_s *rB, char *cChar )
{

bool ret = false;

	// Si el buffer esta vacio ajusto los punteros
	if( rB->count == 0) {
		rB->head = rB->tail = 0;
	}

	if ( rB->count < rB->length ) {
		rB->buff[rB->head] = *cChar;
		++rB->count;
		// Avanzo en modo circular
		rB->head = ( rB->head  + 1 ) % ( rB->length );
		ret = true;
    }

	// Indico que estan llegando datos
	rB->arriving = true;

	return(ret);

}
//------------------------------------------------------------------------------------
bool rBchar_Pop( rBchar_s *rB, char *cChar )
{

bool ret = false;

	// Voy a leer un dato. Si estan llegando, espero.
	if ( rB->arriving == true ) {
		rB->arriving = false;
		vTaskDelay( ( TickType_t)50 );
		// Si siguen llegando, me voy
		if ( rB->arriving == true ) {
			return(false);
		}
	}

	taskENTER_CRITICAL();

	//  Si el buffer esta vacio retorno.
	if( rB->count == 0) {
		rB->head = rB->tail = 0;
		taskEXIT_CRITICAL();
		return(ret);
	}

	*cChar = rB->buff[rB->tail];
	--rB->count;
	// Avanzo en modo circular
	rB->tail = ( rB->tail  + 1 ) % ( rB->length );
	ret = true;

	taskEXIT_CRITICAL();
	return(ret);
}
//------------------------------------------------------------------------------------
bool rBchar_FromISR( rBchar_s *rB, char *cChar )
{

bool ret = false;

	//  Si el buffer esta vacio retorno.
	if( rB->count == 0) {
		rB->head = rB->tail = 0;
		return(ret);
	}

	*cChar = rB->buff[rB->tail];
	--rB->count;
	// Avanzo en modo circular
	rB->tail = ( rB->tail  + 1 ) % ( rB->length );
	ret = true;

	return(ret);
}
//------------------------------------------------------------------------------------
void rBchar_Flush( rBchar_s *rB )
{

	rB->head = 0;
	rB->tail = 0;
	rB->count = 0;
	memset(rB->buff,'\0', rB->length );
}
//------------------------------------------------------------------------------------
void rBchar_CreateStatic ( rBchar_s *rB, uint8_t *storage_area, uint16_t size  )
{
	rB->buff = storage_area;
	rB->head = 0;	// start
	rB->tail = 0;	// end
	rB->count = 0;
	rB->length = size;
	rB->arriving = false;
}
//------------------------------------------------------------------------------------
uint16_t rBchar_GetCount( rBchar_s *rB )
{

	return(rB->count);

}
//------------------------------------------------------------------------------------
uint16_t rBchar_GetFreeCount( rBchar_s *rB )
{

	return(rB->length - rB->count);

}
//------------------------------------------------------------------------------------
bool rBchar_ReachLowWaterMark( rBchar_s *rB)
{
bool retS = false;

	if ( rB->count  < (int)(0.2 * rB->length  ))
		retS = true;
	return(retS);

}
//------------------------------------------------------------------------------------
bool rBchar_ReachHighWaterMark( rBchar_s *rB )
{
bool retS = false;

	if ( rB->count  > (int)(0.8 * rB->length ))
		retS = true;

	return(retS);

}
//------------------------------------------------------------------------------------
