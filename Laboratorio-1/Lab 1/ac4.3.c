#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void cambiarLED() {
	PORTB ^= (1 << PB5); //operacion xor, si esta high lo pone en low y viceversa
}
void apagarLED() {
	PORTB &= ~(1 << PB5); //apaga el led;
}
void prenderLED() {
	PORTB |= (1 << PB5); //prende el led;
}

int buttonState;
int lastButtonState = 0;
int modo = 0;

int main(void) {

	//DDRB |= (1 << PB5);  // setea el numero de port como salida
	DDRB |= (1 << DDB5);

	//Inicializar el pin del pulsador como entrada
	DDRD &= ~(1 << DDD2);

	while (1) {

		//Leer el estado del pulsador
		buttonState = PIND & (1 << PD2);

		//Si cambió el estado del pulsador...
		if (buttonState != lastButtonState)
		{
			//Se espera un tiempo para evitar leer el ruido del rebote del pulsador
			_delay_ms(50);
			//Se vuelve a leer el estado del pulsador
			buttonState = PIND & (1 << PD2);
			//Si el cambio se mantiene, se interpreta como evento de keydown/keyup válido
			if (buttonState != lastButtonState)
			{

				//Si no está presionado el pulsador (se detectó un keyup)...
				if (!buttonState)
				{
					modo++;
				}
				//Si hubo cambios válidos actualizo el valor de lectura anterior
				lastButtonState = buttonState;
			}
		}

		switch (modo) {
			case 0:
			prenderLED();
			break;
			case 1:
			cambiarLED();
			_delay_ms(1000);
			break;
			case 2:
			cambiarLED();
			_delay_ms(500);
			break;
			case 3:
			cambiarLED();
			_delay_ms(250);
			break;
			case 4:
			prenderLED();
			_delay_ms(500);
			cambiarLED();
			_delay_ms(1000);
			break;
			case 5:
			apagarLED();
			break;
			default:
			modo = 0;
		}
	}
}