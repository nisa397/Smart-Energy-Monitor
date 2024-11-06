/*
 * display.h
 *
 * Created: 15/10/2024 10:29:28 am
 *  Author: Ray
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_


void init_display(void);
void display_voltage(float voltage);
void display_current(float current);
void display_power(float power);
void send_next_character_to_display(void);



#endif /* DISPLAY_H_ */