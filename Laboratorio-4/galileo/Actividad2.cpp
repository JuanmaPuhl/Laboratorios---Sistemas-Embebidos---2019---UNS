#include "mraa.hpp"
#include <iostream>
#include <string>
#include "protocolo.h"
using namespace std;
const unsigned char* t = reinterpret_cast<const unsigned char *>( "0" );
char ta[4];
char tmax[4];
char tmin[4];
char tprom[4];

int main() {
	/*Seteo cosas*/
	// Inicializar led conectado a GPIO y controlador de I2C
	mraa::Gpio* d_pin = NULL;

	d_pin = new mraa::Gpio(3, true, true);

    mraa::I2c* i2c;
    i2c = new mraa::I2c(0);
    i2c->address(8);

    uint8_t rx_tx_buf[32];
    uint8_t mode = 0;

    // Indefinidamente
    for (;;) {
    	char c ;
		/*Pido al usuario el modo*/
    	printf("----------------------------------\n");
    	printf("Seleccione el modo de operacion: \n");
    	printf("0: Temp. Actual.\n");
    	printf("1: Temp. Max.\n");
    	printf("2: Temp. Min.\n");
    	printf("3: Temp. Prom. \n");
    	printf("4: Temp. Total. \n");
    	cin >> c;

    	mode = c - '0';
    	//reinterpret_cast<const unsigned char *>( mode );

    	//armo el paquete a enviar
    	char payload[16] = "";
    	pkg *toSend = armarPaquete(mode,payload);

    	//imprime el paquete por pantalla
//    	for(int i = 0; i < 20; i++)
//    	{
//    		printf("%u ",destino[i]);
//    	}
//    	printf("\n");


		/*Espero 1 y envio al slave el modo con el que quiero trabajar*/
    	sleep(1);
    	i2c->write(reinterpret_cast<const unsigned char *>( toSend ),20);

		/*Espero 1 y recibo la temperatura*/
    	// Apagar led y recibir por I2C
    	sleep(1);
    	d_pin->write(0);

    	//leo el mensaje y armo el paquete
    	i2c->read(rx_tx_buf,20);
    	pkg *rec = new pkg;
    	memcpy(rec,rx_tx_buf,20);

    	// Luego de un segundo, encender led e imprimir por stdout
    	sleep(1);
    	d_pin->write(1);

    	if(mode==4){ //Si recibi todas las temperaturas, debo separar la cadena en 4 numeros, y convertirlos a float
    		for(int i=0; i<4; i++){
    			ta[i] = rec->payload[i];
    		}
    		for(int i=0; i<4; i++){
				tmax[i] = rec->payload[i+4];
			}
    		for(int i=0; i<4; i++){
				tmin[i] = rec->payload[i+8];
    		}
    		for(int i=0; i<4; i++){
				tprom[i] = rec->payload[i+12];
			}
			/*Convierto y muestro*/
    		float z = *(float*) ta;
    		printf("TA: %f.\n",z);
    		z = *(float*) tmax;
    		printf("TMAX: %f.\n",z);
    		z = *(float*) tmin;
    		printf("TMIN: %f.\n",z);
    		z = *(float*) tprom;
    		printf("TPROM: %f.\n",z);
    	}
    	else{ //Sino, la conversion es directa. No tengo que separar en distintas cadenas.
    		float f = *(float*) rec->payload;
			switch(mode){
			case 0: printf("TA: ");
					break;
			case 1: printf("TMAX: ");
					break;
			case 2: printf("TMIN: ");
					break;
			case 3: printf("TPROM: ");
					break;
			}
			printf("%f\n",f);

    	}

    	 //Forzar la salida de stdout


    	fflush(stdout);


    }

    return 0;
}


