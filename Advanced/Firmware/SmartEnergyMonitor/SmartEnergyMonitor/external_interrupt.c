/*
 * external_interrupt.c
 *
 * Created: 9/10/2024 4:47:15 PM
 *  Author: Ikenna, Ashwin, Raiyan, Murshid
 */ 

#include "common.h"
#include "adc.h"
#include "external_interrupt.h"
#include <avr/interrupt.h>
#include <avr/io.h>



void external_interrupt_init() {
	// Set PD2 (INT0 pin) as input for voltage signal
	DDRD &= ~(1 << DDD2);


	// Enable rising edge detection on INT0 (voltage)
	EICRA |= (1 << ISC01) | (1 << ISC00);


	// Enable external interrupts on INT0
	EIMSK |= (1 << INT0);
}

ISR(INT0_vect) {
	// Rising edge detected for voltage signal
	
	
	TOGGLE_MUX0();//Switches between ADC0 and 1
	
	
	ADCSRA |= (1<<ADATE)|(1<<ADSC);//Enable Autotrigger and starts conversion
	
	TCCR0B |= (1<<CS00);//set prescaler to 1
	
	EIMSK &= ~(1 << INT0);//Disables rising edge detection
	
	
}

