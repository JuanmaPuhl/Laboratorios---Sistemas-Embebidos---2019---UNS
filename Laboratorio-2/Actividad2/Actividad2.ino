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
    analogWrite(10, 100); //Controla intensidad backlight
    lcd.setCursor(0, 0);
    lcd.print("Key Pad Test");
    lcd.setCursor(0, 1);
    lcd.print("Sist.Emb. 2019");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Press any key...");
    fnqueue_init();
    keyboard_setup();
}

void loop()
{
    fnqueue_run();
}

void right_keydown()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs[TECLA_RIGHT]);
}
void up_keydown()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs[TECLA_UP]);
}
void down_keydown()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs[TECLA_DOWN]);
}
void left_keydown()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs[TECLA_LEFT]);
}
void select_keydown()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs[TECLA_SELECT]);
}

void right_keyup()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs2[TECLA_RIGHT]);
}
void up_keyup()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs2[TECLA_UP]);
}
void down_keyup()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs2[TECLA_DOWN]);
}
void left_keyup()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs2[TECLA_LEFT]);
}
void select_keyup()
{
    lcd.setCursor(0, 1);
    lcd.print(msgs2[TECLA_SELECT]);
}
