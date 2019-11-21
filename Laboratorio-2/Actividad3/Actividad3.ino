/*
  LCD1602 Keypad Shield 1.0 Test Sketch - LiquidCrystal Library

  Este sketch demuestra el uso del LCD1602 Key Shield 1.0.
  Para ello se toman los pulsos de los botones mediante la entrada
  analógica AD0 y, mediante la librería LiquidCrystal de Arduino 1.0,
  se muestra en el display la tecla pulsada. La librería LiquidCrystal
  permite controlar cualquier display LCD compatible con el
  controlador Hitachi HD44780.

  La configuración de la Librería se realiza en base al esquemático
  del shield.

  Este ejemplo está basado en un ejemplo provisto con la documentación del
  LCD1602 Keypad Shield, el cuál ha sido adaptado en base a los ejemplos
  de la librería LiquidCrystal de Arduino.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
  http://arduino.cc/en/Reference/LiquidCrystal
*/
//https://thomashammond1991.wordpress.com/projects/arduino-dfrobotshop-rover/adc-interrupts/

#include <stdbool.h>
#include <stdint.h>
#include "Arduino.h"
// include the library code:
#include <LiquidCrystal.h>
#include "fnqueue.h"
#include "device.h"

// these constants won't change.  But you can change the size of
// your LCD using them:
const uint8_t numRows = 2;
const uint8_t numCols = 16;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Key message
char msgs[5][17] =
{
  " Right Key:  OK ",
  " Up Key:     OK ",
  " Down Key:   OK ",
  " Left Key:   OK ",
  " Select Key: OK "
};
//Teclas soltadas
char msgs2[5][17] =
{
  " Right Key: OFF ",
  " Up Key:    OFF ",
  " Down Key:  OFF ",
  " Left Key:  OFF ",
  " Select Key:OFF "
};

const uint8_t TECLA_RIGHT = 0; //botón up del LCD Keypad Shield
const uint8_t TECLA_UP = 1; //botón down del LCD Keypad Shield
const uint8_t TECLA_DOWN = 2; //botón left del LCD Keypad Shield
const uint8_t TECLA_LEFT = 3; //botón right del LCD Keypad Shield
const uint8_t TECLA_SELECT = 4; //botón select del LCD Keypad Shield

/*Seteo las variables para la actividad 3*/
/*Indica el estado actual
  MP = 0,
  MCA = 1,
  MVT = 2,
  MAD = 3
*/
uint8_t estado = 0;
uint8_t ultimoEstado = 10;
char statesNames[4][16]=
{"  MP  ",
 " MCA  ",
 " MVT  ",
 " MAD  "
};

/*Estructura para guardar el tiempo*/
struct tiempo{
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
}t;
/*Arreglo donde guardo los ultimos 10 tiempos.*/
struct tiempo timeRecord[10];
uint8_t nextUpdate = 0;
uint8_t nextToShow = 0;
uint8_t started = 0;
uint8_t cant = 0;
uint8_t countSelect = 0;
uint8_t contadorSelect = 0;
uint8_t brightness = 1;
uint8_t lastBrightness = 1;
uint8_t timerPaused = 1;
uint8_t timerMAD = 0;
uint8_t countMAD = 0;
volatile uint16_t contador = 0;
uint16_t contador2 = 0;
uint16_t contador3 = 0;

void setup()
{
  key_down_callback(right_keydown, 0);
  key_down_callback(up_keydown, 1);
  key_down_callback(down_keydown, 2);
  key_down_callback(left_keydown, 3);
  key_down_callback(select_keydown, 4);
  key_up_callback(right_keyup, 0);
  key_up_callback(up_keyup, 1);
  key_up_callback(down_keyup, 2);
  key_up_callback(left_keyup, 3);
  key_up_callback(select_keyup, 4);
  pinMode(10, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
  analogWrite(10, 204); //Controla intensidad backlight
  /*Imprimo la pantalla de inicio...*/
  lcd.setCursor(0, 0);
  lcd.print("Laboratorio  2  ");
  lcd.print("2do Cuatrimestre");
  lcd.setCursor(0, 1);
  lcd.print("Sist.Emb. 2019  ");
  lcd.print("Landau.N-Puhl.JM");
  fnqueue_init();
  keyboard_setup();
  cli();
  /*Seteo el timer 2
   * El timer 0 se usa para la funcion delay() millis(), etc etc.
   * El timer 1 se usa en el analogwrite parece. Cuando lo uso la pantalla se apaga.
   * El unico que queda es el timer 2. Uso una frecuencia de 8K y escribo en pantalla 
   * cada vez que se hicieron 8000 interrupciones. Con 1K no se por que va muy rapido.
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
  //Inicializo los tiempos en el arreglo...
  for(int i=0; i<10; i++)
  {
    timeRecord[i].seconds = 0;
    timeRecord[i].minutes = 0;
    timeRecord[i].hours = 0;
  }
}

void loop()
{
  fnqueue_run(); //Ejecuto la funcion en la cola
  actividad3();
}

/*========================================METODOS ACTIVIDAD 3==================================*/

/*Metodo completo de la actividad 3*/
void actividad3()
{
    if(!started) //Si estoy en la pantalla de inicio
  {
    timerPaused = 0;
    countTime(); //Aca puedo usar esta funcion porque todavia no arranco el programa
    if(contador2 >= 1) //Cada un segundo...
    {
      lcd.scrollDisplayLeft(); //Hago scroll
      contador2 = 0;
      cant++;
    }
  }
  else
  { //Aca ya no puedo utilizarla...
    if(countSelect) //Si estoy presionando select
    {
      if(contador > 8000) //Si paso un segundo...
      {
        contadorSelect ++; //Aumento un segundo del select.
        contador = 0;
      }   
    }
    else
    {
      if(timerMAD)
      {
        if(contador > 8000){
          countMAD ++;
          contador = 0;
          
        }
      }
    }
    printHeader(); //Imprimo el header 
    executeFunctions(estado);  //Ejecuto las funciones para el estado actual.
  }
}

/*Metodo que pausa o reanuda el conteo en modo MCA/MP*/
void pauseUnpause()
{
  if(!timerPaused && estado == 1)
  {
    timerPaused = 1;
    contador3 = contador;
    estado = 0;
    contador = 0;
  }
  else
    if(timerPaused && estado == 0)
    {
      timerPaused = 0;
      contador = contador3;
      estado = 1;
    }
}

/*Metodo que cambia el brillo de la pantalla*/
void changeBrightness(int b)
{
  if(lastBrightness != b)
  {
    switch(b)
    {
      case 0: analogWrite(10,255); //100%
              break;
      case 1: analogWrite(10,204); //80%
              break;
      case 2: analogWrite(10,153); //60%
              break;
      case 3: analogWrite(10,102); //40%
              break;
      case 4: analogWrite(10,51);  //20%
              break;
    }
    lastBrightness = b;
  }
}

/*Metodo que ejecuta las funciones para un determinado estado*/
void executeFunctions(int state)
{
  switch(state)
  {
    case 0 : printTime();
             break;
    case 1 : countTime();
             printTime();
             break;
    case 2 : showSavedTime(nextToShow);
             break;
    case 3 : if(countMAD < 5) //Si todavia no paso el tiempo maximo sin actividad...
             {
               changeBrightness(brightness);
               lcd.setCursor(0,1);
               printBrightness(brightness);        
             }
              else
              {
                estado = 0;
                countMAD = 0;
                timerMAD = 0;
                timerPaused = 1;
              }
              break;
              
  }
}

/*Metodo que imprime por pantalla el nivel de brillo actual en modo MAD*/
void printBrightness(int b)
{
  switch(b)
  {
    case 0: lcd.print(" <    100%    > ");
            break;
    case 1: lcd.print(" <    080%    > ");
            break;
    case 2: lcd.print(" <    060%    > ");
            break;
    case 3: lcd.print(" <    040%    > ");
            break;
    case 4: lcd.print(" <    020%    > ");
            break;
  }
}

/*Metodo que imprime en el display el modo actual*/
void printHeader()
{
  lcd.setCursor(0,0);
  if(ultimoEstado != estado)
  {
    ultimoEstado = estado;
    lcd.print("   Modo: ");
    lcd.print(statesNames[estado]);
  }
}

/*Metodo que imprime en el display el tiempo actual en modo MCA/MP*/
void printTime()
{
  countToTime(contador2);
    lcd.setCursor(0, 1);
    lcd.print("    ");
    if(t.hours<10)
      lcd.print("0");
    lcd.print((String)t.hours+":");
    if(t.minutes<10)
      lcd.print("0");
    lcd.print((String)t.minutes+":");
    if(t.seconds<10)
      lcd.print("0");
    lcd.print((String)t.seconds);
    lcd.print("    ");
}

/*Metodo que cuenta segundos a partir de las interrupciones del timer2*/
void countTime()
{
  if(!timerPaused && !countSelect)
  {
    if(contador>8000)
    {
      contador2++;
      contador = 0;  
    }
  }
}

/*Metodo que almacena el tiempo actual en un arreglo*/
void saveActualTime()
{
  timeRecord[nextUpdate].seconds = t.seconds;
  timeRecord[nextUpdate].minutes = t.minutes;
  timeRecord[nextUpdate].hours = t.hours;
  if(nextUpdate == 9)
  {
    nextUpdate = 0;
  }
  else
  {
    nextUpdate++;
  }
}

/*Metodo que resetea el tiempo en modo MP*/
void resetTime()
{
  contador2 = 0;
  t.seconds = 0;
  t.minutes = 0;
  t.hours = 0;
  contador = 0;
  contador3 = 0;
}

/*Metodo que muestra en el display los tiempos guardados*/
void showSavedTime(int i)
{
    lcd.setCursor(0, 1);
    lcd.print(" <  ");
    if(timeRecord[i].hours<10)
      lcd.print("0");
    lcd.print((String)timeRecord[i].hours+":");
    if(timeRecord[i].minutes<10)
      lcd.print("0");
    lcd.print((String)timeRecord[i].minutes+":");
    if(timeRecord[i].seconds<10)
      lcd.print("0");
    lcd.print((String)timeRecord[i].seconds);
    lcd.print("  > ");
}

/*Metodo que convierte un contador de segundos en HH:MM:SS*/
void countToTime(int count)
{
  int minutes = count / 60;
  int seconds = count % 60; 
  int hours = minutes / 60;
  minutes %= 60;
  t.seconds = seconds;
  t.minutes = minutes;
  t.hours = hours;
}

/*Metodo que sale de la pantalla de inicio y comienza la ejecucion normal*/
void iniciar()
{
  started = 1;
  timerPaused = 1;
  contador2 = 0;
  contador = 0;
  for(int i = 0; i<cant; i++) //Scrolleo hacia atras 
  {
    lcd.scrollDisplayRight();
  }
}

ISR(TIMER2_COMPA_vect)
{
  if(!timerPaused)
    contador++;
}

/*================================================TECLAS===============================================*/

void right_keydown()
{
}

void up_keydown()
{
  if(!started)
  {
    iniciar();
  }
  else
  {
    if(estado == 0 || estado == 1)
      pauseUnpause();
    else
    {
      if(estado == 2)
      {
        if(nextToShow<9)
          nextToShow++;
        else
          nextToShow = 0;
      }
      else
        if(estado == 3)
        {
          countMAD = 0;
          if(brightness == 0)
            brightness = 4;
          else
            brightness--;
        }
    }
  }
}
  
void down_keydown()
{
  if(!started)
  {
    iniciar();
  }
  else
  {
    if(estado == 0)
    {
      saveActualTime();
      resetTime();
    }
    else
    {
      if(estado == 1)
      {
        saveActualTime();
      }
      else{
        if(estado == 2)
        {
          if(nextToShow>0)
            nextToShow--;
          else
            nextToShow = 9;
        }
        else
          if(estado == 3)
          { 
            countMAD = 0;
            if(brightness == 4)
                brightness = 0;
              else
                brightness++;
          }
      }
    }
  }
}

void left_keydown()
{
}

/*Para capturar cuantos segundos deje presionado, el cambio de estado
se hace en el keyup. Aqui se setea para que cuente cuantos segundos pasaron
con el boton presionado. La cuenta se hace en el loop*/
void select_keydown()
{
  if(!started)
  {
    iniciar();
  }
  else{
    if(estado == 0)
    {
      nextToShow = 0;
      timerPaused = 0;
      countSelect = 1;
    }
    else
    {
      if(estado == 2 || estado == 3)
      {
        estado = 0;
        timerMAD = 0;
        countMAD = 0;
        contador = 0;
        timerPaused = 1;
      }
    }
  }
}

void right_keyup()
{
}
void up_keyup()
{
}
void down_keyup()
{
}
void left_keyup()
{
}

/*Cuando se suelta el boton select, se chequea cuantos segundos pasaron y cambia 
de estado.*/
void select_keyup()
{
  if(started)
  {
    if(estado == 0 && countSelect)
    {
      if (contadorSelect < 3)
      {
        estado = 2;
        timerPaused = 1;
        contadorSelect = 0;
        countSelect = 0;
        contador = 0;
      } 
      else
      {
        estado = 3;
        timerPaused = 0;
        timerMAD = 1;
        countMAD = 0;
        contadorSelect = 0;
        countSelect = 0;
        contador = 0;
      }
    } 
  }
}
