/*
 * adc.c
 *
 * Created: 6/10/2024 5:08:25 PM
 *  Author: Ikenna, Ashwin, Raiyan, Murshid
 */ 
#include "common.h"
#include "adc.h"
#include "math.h"
#define ADC0() (!(ADMUX & (1 << 0)))


volatile uint8_t conversionCount=0;
volatile uint8_t sampleRetrieved=0;




ISR(ADC_vect) {
	
	
	if (ADC0()){//Checks if channel is ADC0, adds samples to corresponding channel
		vvsSamples[conversionCount]=ADC;
	}
	else{
		visSamples[conversionCount]=ADC;
	}
	
	
	ADCSRA |= (1 << ADIF);//Clears flag
	conversionCount++;
	
	
	if (conversionCount>=SAMPLESIZE){//Stops sampling 
		sampleRetrieved++;
		
		ADCSRA &= ~(1<<ADATE);//Disables autotrigger
		
		TCCR0B &= ~(1<<CS00);//stop timer by setting prescaler to 0
		conversionCount=0;
		
		EIMSK |= (1 << INT0);//Enables rising edge detector
	}
	
	

	
}



void adc_init() {
	ADMUX |= (1 << REFS0)|(1<<MUX0);  // Set reference voltage to AVcc (5V)
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADSC) | (1 << ADEN);  // Enable ADC with prescaler 8, manually start conversion, enables ADC
	
	while (!(ADCSRA & (1<<ADIF))){
	}
	
	ADCSRA|=(1<<ADIE);
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0);  // Set auto-trigger source
}


void stop_sampling(){
	EIMSK &= ~(1 << INT0);//Disables rising edge detection
	ADCSRA &= ~(1<<ADATE);//Disables autotrigger
	TCCR0B &= ~(1<<CS00);//stop timer by setting prescaler to 0
	ADCSRA&=~(1<<ADIE); //Disables interrupt
	EIFR |= (1 << INTF0); //Clears int0 flag
}

void start_sampling(){
	EIMSK |=(1<<INT0); //Enables INT0 interrupt
	ADCSRA|=(1<<ADIE); //Enables ADC conversion interrupt
}
