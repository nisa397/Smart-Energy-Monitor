/*
 * display.h
 *
 * Created: 16/10/2024 1:18:36 pm
 *  Author: Ray
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_
void init_display(void);
void display_voltage(float voltage);
void display_current(int current);
void display_power(float power);
void send_next_character_to_display(void);



#endif /* DISPLAY_H_ */