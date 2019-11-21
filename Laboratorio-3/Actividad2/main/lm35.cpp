#include "lm35.h"
#include "adc.h"

adc_cfg cfg_lm35;
float temp = 0.0;

void lm35_callback(uint16_t valor){
  temp = ((float)valor*(float)5) * (float)100 / (float)1024;
}

void lm35_init(){
  cfg_lm35.channel = 1;
  cfg_lm35.callback = lm35_callback;
  cfg_lm35.valor = 0;
  cfg_lm35.finished = 0;
  adc_init(&cfg_lm35);
}



float lm35_get_temp(){
  return temp;
}
