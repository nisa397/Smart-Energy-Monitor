/*
 * display.c
 *
 * Created: 16/10/2024 12:55:54 pm
 *  Author: Ray
 */ 

/*
 * display.c
 *
 * Created: 15/10/2024 10:28:29 am
 *  Author: Ray
 */ 

#include "common.h"
#include "display.h"
#include <stdint.h>
#include <avr/io.h>
#include <math.h>

// Array containing segment patterns to display digits 0-9 (a-g, dp)
const uint8_t seg_pattern[10] = {
	0b00111111,  // 0 (g = 0)
	0b00000110,  // 1 (g = 0)
	0b01011011,  // 2 (g = 1)
	0b01001111,  // 3 (g = 1)
	0b01100110,  // 4 (g = 1)
	0b01101101,  // 5 (g = 1)
	0b01111101,  // 6 (g = 1)
	0b00000111,  // 7 (g = 0)
	0b01111111,  // 8 (g = 1)
	0b01101111   // 9 (g = 1)
};

const uint8_t seg_unit[3] = {0b00111110,0b01110111,0b01110011};

// 4 characters to be displayed on Ds1 to Ds4
static volatile uint8_t disp_characters[4] = {0, 0, 0, 0};
static volatile uint8_t disp_position = 0; // The current digit being displayed (0-3)

// Define control pins for shift register
#define SH_DS  PORTC4  // Data pin (DS)
#define SH_CP  PORTC3  // Clock pin (SHCP)
#define SH_ST  PORTC5  // Latch pin (STCP)

// Initialize the display pins
void init_display(void) {
	DDRC |= (1 << DDC3) | (1 << DDC4) | (1 << DDC5);  // SH_ST, SH_DS, SH_CP pins
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);  // DS1, DS2, DS3, DS4 pins
}

// Populate 'disp_characters[]' for voltage (XX.XV)
// Populate 'disp_characters[]' for voltage (XX.XV)
void display_voltage(float voltage) {
	uint16_t shifted_voltage = (uint16_t)(voltage * 10 + 0.5);  // Multiply by 10 for one decimal place
	
	// Display format: XX.XV
	disp_characters[2] = seg_pattern[(shifted_voltage / 100) % 10]; // Tens (if greater than 10)
	disp_characters[1] = seg_pattern[(shifted_voltage / 10) % 10];  // Units
	disp_characters[0] = seg_pattern[shifted_voltage % 10];         // Decimal place value
	disp_characters[3] = seg_unit[0];                               // Set 'V' symbol for voltage
	
	disp_characters[1] |= (1 << 7);  // Set the decimal point for the second digit (ones place)
}

// Populate 'disp_characters[]' for current (XXXA)
void display_current(int current) {
	uint16_t shifted_current = (uint16_t)(current + 0.5);  // No decimal point, integer
	disp_characters[2] = seg_pattern[(shifted_current / 100) % 10]; // Hundreds (was [0], now [3])
	disp_characters[1] = seg_pattern[(shifted_current / 10) % 10];  // Tens (was [1], now [2])
	disp_characters[0] = seg_pattern[shifted_current % 10];         // Units (was [2], now [1])
	disp_characters[3] = seg_unit[1];   // Set the 1st digit to A unit
	disp_characters[3] |= (1 << 7); // Set the decimal point for the first digit
}

void display_power(float power) {
    uint16_t shifted_power = (uint16_t)(power * 100 + 0.5);  // Multiply by 100 for two decimal places
    disp_characters[0] = seg_pattern[shifted_power % 10]; // Whole number (1 for 1.87)
    disp_characters[1] = seg_pattern[(shifted_power / 10) % 10];  // First decimal (8 for 1.87)
    disp_characters[2] = seg_pattern[(shifted_power / 100) % 10];         // Second decimal (7 for 1.87)
    disp_characters[3] = seg_unit[2];  // Set the 4th character to 'P'
    disp_characters[2] |= (1 << 7); // Set the decimal point for the first digit
}


// Send the current digit to the display (same as before)
void send_next_character_to_display(void) {
	uint8_t pattern = disp_characters[disp_position];

	PORTC &= ~(1 << SH_ST);  // Disable latch
	for (int i = 7; i >= 0; i--) {
		PORTC &= ~(1 << SH_CP);  // Set clock low
		if (pattern & (1 << i)) {
			PORTC |= (1 << SH_DS);  // Set data pin high if bit is 1
			} else {
			PORTC &= ~(1 << SH_DS);  // Set data pin low if bit is 0
		}
		PORTC |= (1 << SH_CP);  // Set clock high to shift in the bit
	}

	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);  // Disable all digits
	PORTC |= (1 << SH_ST);  // Latch the output

	switch (disp_position) {
		case 0: PORTD &= ~(1 << PORTD7); break;  // Enable Ds1
		case 1: PORTD &= ~(1 << PORTD6); break;  // Enable Ds2
		case 2: PORTD &= ~(1 << PORTD5); break;  // Enable Ds3
		case 3: PORTD &= ~(1 << PORTD4); break;  // Enable Ds4
	}

	disp_position = (disp_position + 1) % 4; // Move to the next position
}
