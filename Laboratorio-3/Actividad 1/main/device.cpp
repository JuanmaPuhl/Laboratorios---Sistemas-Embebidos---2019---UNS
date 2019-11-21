#include "device.h"
#include "adc.h"

const unsigned char PS_16     = (1 << ADPS2);
const unsigned char PS_32     = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64     = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128    = (1 << ADPS2) | (1 << ADPS0) | (1 << ADPS0);

void (*down_functions[5])(); //mis funciones de callback para los botones apretados
void (*up_functions[5])(); //mis funciones de callback para los botones soltados

uint16_t adc_key_val[5] = {30, 220, 360, 535, 760 };
uint8_t NUM_KEYS = 5;
uint16_t adc_key_in;
uint16_t key = -1;
uint16_t oldkey = -1;
//unsigned long adcValor;
adc_cfg cfg_teclado;

// Convert ADC value to key number
uint16_t get_key(uint16_t input)
{
  uint16_t k;
  for (k = 0; k < NUM_KEYS; k++)
    if (input < adc_key_val[k])
      return k;

  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

  return k;
}

void aReadIn(uint16_t adcValor)
{
  key = get_key(adcValor);   // convert into key press
  if (key != oldkey) {
    delay(80);
    key = get_key(adcValor);
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

void key_down_callback(void (*handler)(), int tecla) {

  down_functions[tecla] = handler;

}

void key_up_callback(void (*handler)(), int tecla) {

  up_functions[tecla] = handler;

}


void keyboard_setup(){
//Crear la estructura
  cfg_teclado.channel = 0;
  cfg_teclado.callback = aReadIn;
  cfg_teclado.valor = 0;
  cfg_teclado.finished = 0;
  adc_init(&cfg_teclado);
}
