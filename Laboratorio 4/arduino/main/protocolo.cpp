#include "protocolo.h"
#include <string.h>

//arma un paquete de acuerdo al protocolo elegido
pkg* armarPaquete(uint8_t type, char payload[16])
{
    pkg *toSend = new pkg;
    toSend->init_symbol = INIT_SYMBOL;
    toSend->size = 16;
    memcpy(toSend->payload, payload, 16);
    toSend->type = type;
    toSend->end_symbol = END_SYMBOL;
    return toSend;
}
