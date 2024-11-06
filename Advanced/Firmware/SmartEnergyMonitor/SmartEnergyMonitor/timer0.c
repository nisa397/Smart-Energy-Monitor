/*
 * timer0.c
 *
 * Created: 7/10/2024 4:23:39 pm
 *  Author: Ikenna, Ashwin, Raiyan, Murshid
 */ 


#include "common.h"
#include "timer0.h"



ISR(TIMER0_COMPA_vect){

}

void timer0_init() {
//initialization of the counter.
TCCR0A |= (1 << WGM01);//CTC mode

//Output compare value, store counter number. 
OCR0A = 199;
//Enable interrupts for output compare A
TIMSK0 |= (1<<OCIE0A);
}





