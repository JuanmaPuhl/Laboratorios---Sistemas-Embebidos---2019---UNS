#ifndef DEVICE_H
#define DEVICE_H
void key_down_callback(void (*handler)(), int tecla);
void key_up_callback(void (*handler)(), int tecla);
void keyboard_setup();
#endif
