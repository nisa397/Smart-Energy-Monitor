/*
 * timer2.c
 *
 * Created: 16/10/2024 4:40:21 PM
 *  Author: iuna331
 */ 

#include "common.h"
#include "display.h"
#include "timer2.h"

ISR(TIMER2_COMPA_vect){
	send_next_character_to_display();
}

void timer2_init(){
	
	//Setting CTC mode
	TCCR2A|=(1<<WGM21);
	
	
	TCCR2B=0;
	
	
	
	OCR2A = 30;
	
	//Enabling interrupt when compare match event occurs
	TIMSK2=0;
	TIMSK2 |= (1<<OCIE2A);
	
	
}