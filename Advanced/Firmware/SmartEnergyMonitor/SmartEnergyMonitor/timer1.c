/*
 * timer1.c
 *
 * Created: 15/10/2024 12:58:27 PM
 *  Author: rkha931
 */ 

/*
 * timer1.c
 *
 * Created: 15/10/2024 12:18:26 PM
 *  Author: rkha931
 */

#include "common.h"
#include "timer1.h"
#include "display.h"

volatile uint8_t display_mode = 0;
volatile uint8_t displayReady = 0;

//volatile uint8_t displayReady = 0;
//
//
//
//ISR(TIMER1_COMPA_vect){
	//displayReady=1;
	//display_mode = (display_mode + 1) % 3;
	//
//}


/*volatile uint8_t display_mode = 0;
volatile uint8_t displayReady = 0;*/

ISR(TIMER1_COMPA_vect){
	
	displayReady=1;
	
	//Changes between voltage, current and power
	display_mode++;
	if (display_mode >2)
	{
		display_mode = 0;
	}
	
	
}



void timer1_init(){
	TCCR1B |= (1<<WGM12); //CTC mode
	TCCR1B|=(1<<CS12)|(1<<CS10); //Turning clock on to detect 1 second, prescaler of 1024
	

	//Counts to 1 second
	OCR1A = 1953;


	
	TIMSK1 |= (1<<OCIE1A); //Enables Output compare interrupt
}