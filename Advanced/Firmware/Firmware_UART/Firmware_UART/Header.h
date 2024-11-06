/*
 * header.h
 *
 * Created: 17/08/2024 3:57:04 PM
 *  Author: Ikenna, Ashwin, Raiyan, Murshid
 */ 


#ifndef HEADER_H_
#define HEADER_H_

// Function Declarations
void uart_init(void);  // UART initialization
void uart_transmit_byte(char data);  // Function to transmit a single byte
void uart_transmit_string(const char* str);  // Function to transmit a string
void uart_transmit_float(float number, uint8_t decimal_places);  // Function to transmit a float
void uart_transmit_integer(int number);  // Function to transmit an integer



#endif /* HEADER_H_ */