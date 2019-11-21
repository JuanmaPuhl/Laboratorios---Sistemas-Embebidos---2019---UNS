/* 
 Código AVR de ejemplo para detectar un flanco
 Se utiliza el hardware del ejemplo Button de Arduino
  http://www.arduino.cc/en/Tutorial/Button
 */

#include <avr/io.h>
#include <util/delay.h>

// Variables:
int buttonState;      
int lastButtonState = 0;   

int main()
{
	//Inicializar el pin del pulsador como entrada
	DDRD &= ~(1<<DDD2);
	//Inicializar el pin del led como salida
	DDRB |= (1<<DDB5);

	while (1)
	{
		//Leer el estado del pulsador
		buttonState = PIND & (1<<PD2);

		//Si cambió el estado del pulsador...
		if (buttonState != lastButtonState)
		{
			//Si no está presionado el pulsador (se detectó un keyup)...
			if (!buttonState)
			{					
				PORTB ^= (1<<PB5); //Se modifica el estado del led (toggle vía XOR)
			}

			//Si hubo cambios válidos actualizo el valor de lectura anterior
			lastButtonState = buttonState;
		}
	}
}
