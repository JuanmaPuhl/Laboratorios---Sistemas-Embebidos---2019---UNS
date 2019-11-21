/*
 * protocolo.h
 *
 *  Created on: 5/10/2019
 *      Author: ptcni
 */

#include <stdint.h>
#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

//constantes para tipos de mensaje
const uint8_t OBTENER_TEMP = 0;
const uint8_t OBTENER_MAX = 1;
const uint8_t OBTENER_MIN = 2;
const uint8_t OBTENER_PROM = 3;
const uint8_t OBTENER_TODO = 4;
const uint8_t RESPONDER_TEMP = 5;
const uint8_t RESPONDER_MAX = 6;
const uint8_t RESPONDER_MIN = 7;
const uint8_t RESPONDER_PROM = 8;
const uint8_t RESPONDER_TODO = 9;
const uint8_t INIT_SYMBOL = 35; //  #
const uint8_t END_SYMBOL = 64; //  @
const uint8_t ESCAPE_SYMBOL = 92; //  \

//estructura del paquete de datos a utilizar
struct pkg{
	char init_symbol;
	uint8_t size;
	char payload[16];
	uint8_t type;
	char end_symbol;
};

//arma el paquete
pkg* armarPaquete(uint8_t type, char payload[16]);




#endif /* PROTOCOLO_H_ */
