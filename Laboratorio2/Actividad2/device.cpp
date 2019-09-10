#include "fnqueue.h"
#include "critical.h"
#include "device.h"
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "Arduino.h"

const unsigned char PS_16     = (1 << ADPS2);
const unsigned char PS_32     = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64     = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128    = (1 << ADPS2) | (1 << ADPS0) | (1 << ADPS0);

void (*down_functions[5])(); //mis funciones de callback para los botones apretados
void (*up_functions[5])(); //mis funciones de callback para los botones soltados

uint16_t adc_key_val[5] = {30, 150, 360, 535, 760 };
uint8_t NUM_KEYS = 5;
uint16_t adc_key_in;
uint16_t key = -1;
uint16_t oldkey = -1;
volatile unsigned long adcVal;

void keyboard_setup()
{
    cli();
    ADCSRA |= B10000000;//    Enable ADC.
    ADMUX &= B11011111;//        Set right - adjust results.
    ADMUX |= B01000000;//        Set REFS1 to external source.
    ADMUX |= 0;// (8 - 0)        Select analog input.
    ADCSRA |= B00100000;//    Enable auto trigger.
    ADCSRB &= B11111000;//    Set trigger to Free Running Mode.
    ADCSRA &= ~PS_128;//        removes bits set by Arduino library
    ADCSRA |= PS_64;//        Set prescaler.
    ADCSRA |= B00001000;//    Enable ADC interrupt.
    sei();//                    Enbale global interrupts.
    ADCSRA |= B01000000;//        Start first conversion.
}

// Convert ADC value to key number
uint16_t get_key(unsigned int input)
{
    uint16_t k;
    for (k = 0; k < NUM_KEYS; k++)
        if (input < adc_key_val[k])
            return k;

    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed

    return k;
}

void aReadIn()
{
    key = get_key(adcVal);   // convert into key press
    if (key != oldkey)
    {
        delay(50);
        key = get_key(adcVal);
        if (key != oldkey)
        {
            if (key >= 0 && key <= 4)
            {
                oldkey = key;
                down_functions[key]();
            }
            else
            {
                up_functions[oldkey]();
                oldkey = key;
            }
        }
    }
}

ISR(ADC_vect)
{
    adcVal = ADCL | (ADCH << 8);
    fnqueue_add(aReadIn);
}

void key_down_callback(void (*handler)(), int tecla)
{
    down_functions[tecla] = handler;
}

void key_up_callback(void (*handler)(), int tecla)
{
    up_functions[tecla] = handler;
}
