#include <math.h>
#include <LiquidCrystal.h>
#include "device.h"
#include "timer.h"
#include "lm35.h"
#include "fnqueue.h"



float max_temp = -55.0;
float min_temp = 150.0;
float temp_actual = 0.0;
/*Variable para seleccionar modo.
0 = temp_actual
1 = max_temp
2 = min_temp
3 = average
*/
uint8_t mode = 0;
uint8_t mode_change = 1;
uint8_t started = 1;
volatile uint16_t contador = 0;
volatile uint16_t contador2 = 0;
float temperature_record[100];
uint8_t lastIndex = 0;
uint8_t size = 0;


// these constants won't change.  But you can change the size of
// your LCD using them:
const uint8_t numRows = 2;
const uint8_t numCols = 16;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



void setup() {
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
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(10, OUTPUT);
    // set up the LCD's number of columns and rows:
    lcd.begin(numCols, numRows);
    analogWrite(10, 204); //Controla intensidad backlight
    /*Imprimo la pantalla de inicio...*/
    lcd.setCursor(0, 0);
    lcd.print("Laboratorio  2  ");
    lcd.setCursor(0, 1);
    lcd.print("Sist.Emb. 2019  ");
    /*Inicio funciones,sensores y colas*/
    fnqueue_init();
    lm35_init();
    timer_init();
    keyboard_setup();
    // fnqueue_add(act2);
    // fnqueue_add(ide_io);
}

void loop() {
    // put your main code here, to run repeatedly:

    fnqueue_run();
    act2();
    ide_io();
}

void ide_io()
{
    if (Serial.available() > 0)
    {
        byte val = Serial.read();
        switch (val)
        {
            case 0:
                Serial.println(temp_actual, 2);
                break;
            case 1:
                Serial.println(max_temp, 2);
                break;
            case 2:
                Serial.println(min_temp, 2);
                break;
            case 3:
                Serial.println(get_average(), 2);
                break;
            case 4:
                mode = 0;
                //Serial.println(1);
                break;
            case 5:
                mode = 1;
                //Serial.println(1);
                break;
            case 6:
                mode = 2;
                //Serial.println(1);
                break;
            case 7:
                mode = 3;
                //Serial.println(1);
                break;
            default:
                break;
        }
    }
    //fnqueue_add(ide_io);
}

void show_temp(){
  if(contador2 >= 8000 || mode_change){
    lcd.setCursor(0,0);
    lcd.print("  Temp. Actual  ");
    lcd.setCursor(0,1);
    lcd.print((String)"     "+temp_actual+"C     ");
    contador2 = 0;
    mode_change = 0;
  }
}

void show_max_temp() {
    lcd.setCursor(0, 0);
    lcd.print("  Temp. Maxima  ");
    lcd.setCursor(0, 1);
    lcd.print((String)"     " + max_temp + "C     ");
}

void show_min_temp() {
    lcd.setCursor(0, 0);
    lcd.print("  Temp. Minima  ");
    lcd.setCursor(0, 1);
    lcd.print((String)"     " + min_temp + "C     ");
}

void show_avg_temp() {
    lcd.setCursor(0, 0);
    lcd.print(" Temp. Promedio ");
    lcd.setCursor(0, 1);
    float prom = get_average();
    lcd.print((String)"     " + prom + "C     ");
}

/*  VOUT= 10 mv/°C × T
    VOUT/10mv = T
    VOUT/(10*10^-3) = T
    VOUT * 10 = T
*/

void act2(){
  float temp = lm35_get_temp();
  if(!started){

    if(temp > max_temp){
      max_temp = temp;
    }
    if(temp < min_temp){
      min_temp = temp;
    }
    /*
      Tengo que guardar 100 muestras en 15 seg, 100 / 15 = 6.666... lo redondeo
      a 7. Entonces en un segundo tengo que guardar 7 muestras. Entonces calculo
      cada cuanto tengo que guardar una sola muestra, 1 / 7 = 0.142857...
      Multiplicandolo por 1000 me da la cantidad de milisegundos = 142.8571429
      Como el timer hace 1000 ms en 8000 ticks, 142.8571429 * 8000 / 1000 = 1142.8..
      redondeando a 1143.
    */
    if(contador >= 1143){
      saveTemperature(temp);
      contador = 0;
    }
  }
  else{
    if(temp != 0.0){
      started = 0;
    }
  }


  /*Veo que hacer segun el modo.*/
  switch(mode){
    case 0: temp_actual = temp;
            show_temp();
            break;
    case 1: show_max_temp();
            break;
    case 2: show_min_temp();
            break;
    case 3: show_avg_temp();
            break;
  }
  //fnqueue_add(act2);
}

void saveTemperature(float t) {
    temperature_record[lastIndex] = t;
    if (size < 100)
        size++;
    if (lastIndex < 99) {
        lastIndex++;
    }
    else
        lastIndex = 0;
}

float get_average() {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += temperature_record[i];
    }
    sum /= size;
    return sum;
}

ISR(TIMER2_COMPA_vect)
{
    contador++;
    contador2++;
}


void right_keydown()
{
}

void up_keydown()
{
    if (mode == 0) {
        mode = 1;
    }
    else {
        if (mode == 1) {
            mode = 0;
            mode_change = 1;
        }
    }
}

void down_keydown()
{
    if (mode == 0) {
        mode = 2;
    }
    else {
        if (mode == 2) {
            mode = 0;
            mode_change = 1;
        }
    }
}

void left_keydown()
{
}

void select_keydown()
{
    if (mode == 0) {
        mode = 3;
    }
    else {
        if (mode == 3) {
            mode = 0;
            mode_change = 1;
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

void select_keyup()
{
}
