/*
 * UART.c
 *
 * Created: 17/08/2024 2:03:51 PM
 *  Author: Ikenna, Ashwin, Raiyan, Murshid

 */ 

	#include <stdlib.h>
	#include <avr/io.h>
	#include <stdio.h>

void uart_init(){
	UCSR0B = (1 << TXEN0);  // Enable Transmitter
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8 data bits, 1 stop bit, no parity
	UBRR0 = 12;  // Set baud rate (9600 for 20MHz clock)
}

void uart_transmit_byte(char data){
	while(!(UCSR0A & (1 << UDRE0))) {
		// Wait for the transmit buffer to be empty
	}
	UDR0 = data; // Transmit data
} 

void uart_transmit_string(const char* str) {
	while(*str) {
		uart_transmit_byte(*str++);
	}
}

void uart_transmit_float(float number, uint8_t decimal_places) {
	char buffer[10];
	dtostrf(number, 1, decimal_places, buffer);  // Convert float to string
	uart_transmit_string(buffer);
	
	
	
}

void uart_transmit_integer(int number) {
	char buffer[5];
	// Counting for edge case, where there are less than 3 digits. 
	if (number<10){
		sprintf(buffer, "00%d", number);
		uart_transmit_string(buffer);
	}
	else if(number<100){
		sprintf(buffer, "0%d", number);
		uart_transmit_string(buffer);
	}
	else{
		itoa(number, buffer, 10);  // Convert integer to string
		uart_transmit_string(buffer);
		
	}
}
