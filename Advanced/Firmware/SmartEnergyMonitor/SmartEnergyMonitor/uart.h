/*
 * uart.h
 *
 * Created: 25/09/2024 5:18:12 PM
 *  Author: nisa397
 */ 


#ifndef UART_H_
#define UART_H_

// Function Declarations
void uart_init(void);  // UART initialization
void uart_transmit_byte(char data);  // Function to transmit a single byte
void uart_transmit_string(const char* str);  // Function to transmit a string
void uart_transmit_float(float number, uint8_t decimal_places);  // Function to transmit a float
void uart_transmit_integer(int number);  // Function to transmit an integer



#endif /* UART_H_ */