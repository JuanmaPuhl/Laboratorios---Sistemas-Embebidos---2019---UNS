#include "adc.h"
#include <math.h>
#include <LiquidCrystal.h>
#include "device.h"


adc_cfg cfg;
adc_cfg cfg2;
adc_cfg cfg3;

float max_temp = 0.0;
float min_temp = 1000.0;
float temp_actual = 0.0;
/*Variable para seleccionar modo.
0 = temp_actual
1 = max_temp
2 = min_temp
3 = average
*/
uint8_t mode = 0;
// these constants won't change.  But you can change the size of
// your LCD using them:
const uint8_t numRows = 2;
const uint8_t numCols = 16;
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
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

volatile uint16_t contador = 0;
float temperature_record[100];
uint8_t lastIndex = 0;
uint8_t size = 0;
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
  fnqueue_init();
  cfg2.channel = 1;
  cfg2.callback = imprimir2;
  cfg2.valor = 0;
  cfg2.finished = 0;
  pinMode(10, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
  analogWrite(10, 204); //Controla intensidad backlight
  /*Imprimo la pantalla de inicio...*/
  lcd.setCursor(0, 0);
  lcd.print("Laboratorio  2  ");
  lcd.setCursor(0, 1);
  lcd.print("Sist.Emb. 2019  ");
  cli();
  /*Seteo el timer 2
   * El timer 0 se usa para la funcion delay() millis(), etc etc.
   * El timer 1 se usa en el analogwrite parece. Cuando lo uso la pantalla se apaga.
   * El unico que queda es el timer 2. Uso una frecuencia de 8K y cada 1144 ticks, es un ms.
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
  keyboard_setup();
  adc_init(&cfg2);

}

void loop() {
  // put your main code here, to run repeatedly:
  fnqueue_run();
}

void show_temp(){
  lcd.setCursor(0,0);
  lcd.print("  Temp. Actual  ");
  lcd.setCursor(0,1);
  lcd.print((String)"     "+temp_actual+"C     ");
}

void show_max_temp(){
  lcd.setCursor(0,0);
  lcd.print("  Temp. Maxima  ");
  lcd.setCursor(0,1);
  lcd.print((String)"     "+max_temp+"C     ");
}

void show_min_temp(){
  lcd.setCursor(0,0);
  lcd.print("  Temp. Minima  ");
  lcd.setCursor(0,1);
  lcd.print((String)"     "+min_temp+"C     ");
}

void show_avg_temp(){
  lcd.setCursor(0,0);
  lcd.print(" Temp. Promedio ");
  lcd.setCursor(0,1);
  float prom = get_average();
  lcd.print((String)"     "+prom+"C     ");
}

/*VOUT= 10 mv/°C × T
  VOUT/10mv = T
  VOUT/(10*10^-3) = T
  VOUT * 10 = T
*/

void imprimir2(uint16_t valor){
  float temp = ((float)valor*(float)5) * (float)10 / (float)1024;
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
    Serial.println("Voy a guardar");
    saveTemperature(temp);
    contador = 0;
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
}

void saveTemperature(float t){
  temperature_record[lastIndex] = t;
  if(size < 100)
    size++;
  if(lastIndex < 99){
    lastIndex++;
  }
  else
    lastIndex = 0;
}

float get_average(){
  float sum = 0;
  for(int i=0; i<size; i++){
    sum += temperature_record[i];
  }
  sum /= size;
  return sum;
}

ISR(TIMER2_COMPA_vect)
{
  contador++;
}


void right_keydown()
{
}

void up_keydown()
{
  if(mode == 0){
    mode = 1;
  }
  else{
    if(mode == 1){
      mode = 0;
    }
  }
}

void down_keydown()
{
  if(mode == 0){
    mode = 2;
  }
  else{
    if(mode == 2){
      mode = 0;
    }
  }
}

void left_keydown()
{
}

void select_keydown()
{
  if(mode == 0){
    mode = 3;
  }
  else{
    if(mode == 3){
      mode = 0;
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
