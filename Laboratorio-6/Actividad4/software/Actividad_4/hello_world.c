/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 * Ejemplo minimal de trabajo con Timers e ISRs.

 *
 * Si bien se puede configurar la HAL para definir un system clock y registrar
 * alarmas (callbacks) en el mismo, vamos a trabajar con ISR a menor nivel de
 * abstracción.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <system.h>
#include <sys/alt_irq.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>

uint8_t registros[8]; //Arreglo donde guardo los valores de cada registro, para luego saber cual cambio.
unsigned int numerosOrdenados1;
unsigned int numerosOrdenados2;
int cambio = 1;
int lastSwitchs = -1;
int switchs;
int toShow = 0;
int numero = 0;
int menor = 0;
int toSend;

static void timerISR(void* context)
{
	numerosOrdenados1 = IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE); //Recibo de PIO_0 los primeros 4 registros. 32 bits
	numerosOrdenados2 = IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE); //Recibo de PIO_1 los ultimos 4 registros. 32 bits
	if(toShow < 4)
		numero = (numerosOrdenados1 << (8 * (3 - toShow)))>> 24; //Voy separando los numeros segun lo que necesite
	else
		numero = (numerosOrdenados2 << (8 * (7 - toShow ) ))>> 24;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_5_BASE, numero); //Mando a la salida el numero obtenido
	if(toShow < 7) //Incremento el puntero
		toShow++;
	else
		toShow = 0;
	//Limpia el flag de interrupción (evita la ejecución múltiple de la ISR ante la misma IRQ)
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
}

/*Con este metodo obtengo el numero de registro con el menor valor almacenado*/
void revisarMenor()
{
	unsigned int num = IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE); //Obtengo la salida de C
	num = (num << 24 ) >> 24; //Hago shift de los numeros obtenidos asi me quedo con el primero.
	//Corro todo a la izq asi me quedo con un solo numero y luego lo vuelvo a la posicion original.
	printf("El menor numero en los registros es: %d\n",num);
	int encontre = 0;
	int i=0;
	menor = 0;
	while (!encontre && i<8)//Busco en los registros cual es el que tiene ese valor
	{
		if(registros[i] == num)
			encontre = 1;
		else
			i++;
	}
	if(encontre)
		menor = i;
}

int main()
{
  //Inicializa el Timer escribiendo su registro de control.
  IOWR_ALTERA_AVALON_TIMER_CONTROL (TIMER_0_BASE,
              ALTERA_AVALON_TIMER_CONTROL_ITO_MSK  |
              ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
              ALTERA_AVALON_TIMER_CONTROL_START_MSK);
  //Registra la ISR (callback) del Timer
  int regStatus = alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID,
		  	  	  	  TIMER_0_IRQ,
		  	  	  	  &timerISR, 0x0, 0x0);
  //Ciclo principal vacío a la espera de interrupciones
  while (1)
  {
	switchs = IORD_ALTERA_AVALON_PIO_DATA(PIO_4_BASE); //Leo del registro de entrada el valor ingresado
	if(switchs != lastSwitchs)
	{ //Si cambio a lo que tenia antes
		revisarMenor(); //Chequeo cual es el menor registro, es el que voy a reemplazar
		printf("El numero a ingresar es: %d\n",switchs);
		printf("El menor es: %d\n",menor);
		registros[menor] = switchs; //Reemplazo
		if(menor < 4)
		{ //Envio al modulo C los registros. Tienen que ser de 32 bits
			toSend = (registros[0] << 24) | (registros[1] << 16) | (registros[2] << 8) | (registros[3]);
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, toSend);//Escribo en PIO_2 lo sacado de PIO_4
		}
		else
		{
			toSend = (registros[4] << 24) | (registros[5] << 16) | (registros[6] << 8) | (registros[7]);
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_3_BASE, toSend);//Escribo en PIO_2 lo sacado de PIO_4
		}
		lastSwitchs = switchs;
		for(int i=0; i<8 ; i++)//Imprimo en consola los registros.
			printf("REGISTRO %d : %d\n",i,registros[i]);
	}
  }
  return 0;
}
