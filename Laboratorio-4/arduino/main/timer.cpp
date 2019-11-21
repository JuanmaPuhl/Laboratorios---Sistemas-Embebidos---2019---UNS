#include "timer.h"

void timer_init() {
    cli();
    /*  Seteo el timer 2
        El timer 0 se usa para la funcion delay() millis(), etc etc.
        El timer 1 se usa en el analogwrite parece. Cuando lo uso la pantalla se apaga.
        El unico que queda es el timer 2. Uso una frecuencia de 8K y cada 1144 ticks, es un ms.
    */
    TCCR2A = 0;// set entire TCCR2A register to 0
    TCCR2B = 0;// same for TCCR2B
    TCNT2  = 0;//initialize counter value to 0
    // set compare match register for 8khz increments
    OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
    // turn on CTC mode
    TCCR2A |= (1 << WGM21);
    // Set CS21 bit for 8 prescaler
    TCCR2B |= (1 << CS21);
    // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);
    sei();//                    Enbale global interrupts.
}
