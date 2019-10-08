#include "adc.h"

#define MAX_CHANNELS 6

adc_cfg channel[MAX_CHANNELS];
uint8_t occupied_channels[MAX_CHANNELS] = {0, 0, 0, 0, 0, 0};
uint8_t current_channel = 0;
uint8_t first_configuration = 1;
volatile uint16_t adcVal;
const unsigned char PS_16     = (1 << ADPS2);
const unsigned char PS_32     = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64     = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128    = (1 << ADPS2) | (1 << ADPS0) | (1 << ADPS0);

/*Rutina del adc que se ejecuta todo el tiempo. Debe ser llamada en el main*/
void adc_loop() {
    for (int i = 0; i < MAX_CHANNELS; i++) { //Miro en todos los canales
        if (occupied_channels[i] && channel[i].finished) { //Si esta ocupado, y ademas tiene una conversion lista
            cli(); //Es necesario deshabilitar interrupciones aca?
            channel[i].finished = 0; //Desmarco el terminado
            sei();
            channel[i].callback(channel[i].valor); //Llamo a la funcion callback con el valor obtenido por el adc
        }
    }
}

/*Metodo auxiliar para configurar el adc*/
/*
    Tengo que cambiar la forma en que asigno....
*/
void adc_configure() {
    cli();
    ADCSRA |= B10000000;//    Enable ADC.
    // ADMUX &= B11011111;//        Set right - adjust results.
    ADMUX = (1 << REFS0);   //select AVCC as reference
    ADCSRA |= PS_128;//        Set prescaler.
    ADCSRA |= B00001000;//    Enable ADC interrupt.
    first_configuration = 0;
    sei();//                    Enbale global interrupts.
    ADCSRA |= B01000000;//        Start first conversion.
}

/*Metodo auxiliar para cambiar el canal actual*/
void adc_set_new_channel() {
    // deshabilitar interrupciones?
    switch (current_channel) {
        case 0:
            ADMUX &= ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0); //Vuelvo a poner todos los bits en 0 y luego hago el corrimiento apropiado.
            break;
        case 1:
            ADMUX &= ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);
            ADMUX |= (1 << MUX0);
            break;
        case 2:
            ADMUX &= ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);
            ADMUX |= (1 << MUX1);
            break;
        case 3:
            ADMUX &= ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);
            ADMUX |= (1 << MUX1) | (1 << MUX0);
            break;
        case 4:
            ADMUX &= ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);
            ADMUX |= (1 << MUX2);
            break;
        case 5:
            ADMUX &= ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);
            ADMUX |= (1 << MUX2) | (1 << MUX0);
            break;
    }
}

/*  Metodo heredado del header
    Ingresa una estructura a un canal del adc.
*/
bool adc_init(adc_cfg *cfg) {
    if (first_configuration) //Si es la primera vez que entro...
        adc_configure(); //Configuro el adc.
    uint8_t canal_elegido = cfg -> channel;
    if (canal_elegido >= 0 && canal_elegido < 6 && !occupied_channels[canal_elegido]) {
        channel[canal_elegido] = *cfg; //Ingreso la estructura en el arreglo
        occupied_channels[canal_elegido] = 1;
        return true;
    }
    return false;
}

/*Metodo auxiliar para seleccionar el siguiente canal*/
void adc_select_next_channel() {
    if (current_channel < (MAX_CHANNELS - 1)) { //Si no es el ultimo canal...
        current_channel ++; //Sumo normalmente.
    }
    else {
        current_channel = 0; //Sino, vuelvo a empezar desde el canal 0.
    }
    if (!occupied_channels[current_channel]) { //Si no estaba ocupado el canal...
        adc_select_next_channel(); //Vuelvo a elegir otro canal.
    }
}

/*Rutina de interrupcion llevada a cabo por el ADC*/
ISR(ADC_vect) {
    adcVal = ADCL | (ADCH << 8); //Obtengo el valor leido por el ADC
    channel[current_channel].finished = 1; //Marco que ya termino la conversion
    channel[current_channel].valor = adcVal; //Guardo el valor.
    adc_select_next_channel(); //Veo cual va a ser el siguiente canal a revisar.
    adc_set_new_channel(); //Seteo el nuevo canal
    if (current_channel == 0) { //Si ya recorri todos los canales
        fnqueue_add(adc_loop); //Busco entre todos los canales si puedo ejecutar un callback.
    }//
    ADCSRA |= B01000000; //Vuelvo a iniciar la conversion
}
