#ifndef ADC_H
#define ADC_H
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "Arduino.h"
#include "fnqueue.h"

typedef struct adc_cfg{
  uint8_t channel;
  void (*callback)(uint16_t);
  uint8_t finished;
  uint16_t valor;
};

bool adc_init(adc_cfg *cfg);

#endif
