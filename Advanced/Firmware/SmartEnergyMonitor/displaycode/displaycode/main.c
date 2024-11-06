/*
 * displaycode.c
 *
 * Created: 15/10/2024 10:26:19 am
 * Author : Ray
 */ 
//#include "display.h"
//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//#include <stdint.h>
//
//int main(void) {
	//init_display();
	//
	//float voltage = 13.4;  // Example voltage value
	//float current = 729
	//;   // Example current value
	//float power = 1.87;   // Example power value
//
	//while (1) {
		//display_voltage(voltage);
		//for (int i = 0; i < 200; i++) {
			//send_next_character_to_display();
			//_delay_ms(5);
		//}
//
		//display_current(current);
		//for (int i = 0; i < 200; i++) {
			//send_next_character_to_display();
			//_delay_ms(5);
		//}
//
		//display_power(power);
		//for (int i = 0; i < 200; i++) {
			//send_next_character_to_display();
			//_delay_ms(5);
		//}
	//}
//}

#define F_CPU 2000000UL  // Set clock frequency to 2 MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"
#include <util/delay.h>  // Include delay library

// Global variable to track the current parameter being displayed
volatile uint8_t display_mode = 0; // 0 for voltage, 1 for current, 2 for power

// Example values for voltage, current, and power
float voltage = 12.3;
float current = 729;
float power = 1.85;

// Initialize Timer1 in CTC mode for 1-second intervals
void init_timer1_ctc(void) {
	// Set CTC mode with OCR1A as top value
	TCCR1B |= (1 << WGM12);

	// Set the compare value for 1-second intervals
	// Formula: OCR1A = (F_CPU / (Prescaler * Desired frequency)) - 1
	// For F_CPU = 2 MHz, 1-second interval, Prescaler = 1024
	OCR1A = 1953;

	// Enable Timer1 Compare Match A interrupt
	TIMSK1 |= (1 << OCIE1A);

	// Set the prescaler to 1024 and start the timer
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

int main(void) {
	init_display();  // Initialize the 7-segment display
	init_timer1_ctc();  // Initialize Timer1 in CTC mode

	sei();  // Enable global interrupts

	while (1) {
		// Periodically send the next character to the display
		send_next_character_to_display();
		_delay_ms(5);  // Small delay to control the refresh rate
	}
}

// Timer1 Compare Match A Interrupt Service Routine (ISR)
ISR(TIMER1_COMPA_vect) {
	// Display the selected parameter (voltage, current, or power)
	switch (display_mode) {
		case 0:
		display_voltage(voltage);
		break;
		case 1:
		display_current(current);
		break;
		case 2:
		display_power(power);
		break;
	}

	// Switch between voltage, current, and power every 1 second
	display_mode = (display_mode + 1) % 3; // Cycle through 0 (voltage), 1 (current), 2 (power)
}



