#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void cambiarLED()
{
  PORTB ^= (1 << PB5); //operacion xor, si esta high lo pone en low y viceversa
}
void apagarLED()
{
  PORTB &= ~(1 << PB5); //apaga el led;
}
void prenderLED()
{
  PORTB |= (1 << PB5); //prende el led;
}

int incomingByte = 0;
void setup()
{
  Serial.begin(9600);
  DDRB |= (1 << DDB5);  // setea el numero de port como salida
}

void loop()
{
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read(); // read the incoming byte:
    Serial.println(incomingByte, DEC);
  }
  switch (incomingByte) 
  {
    case '1':
      prenderLED();
      break;
    case '2':
      cambiarLED();
      _delay_ms(1000);
      break;
    case '3':
      cambiarLED();
      _delay_ms(500);
      break;
    case '4':
      cambiarLED();
      _delay_ms(250);
      break;
    case '5':
      prenderLED();
      _delay_ms(500);
      cambiarLED();
      _delay_ms(1000);
      break;
    case '6':
      apagarLED();
      break;
  }
}