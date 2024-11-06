/*
 * main.c
 *
 * Created: 17/08/2024 2:03:33 PM
 * Author : Ikenna, Ashwin, Raiyan, Murshid
 */ 

#define F_CPU 2000000
#define RMSVoltage 14.5
#define PeakCurrent 101
#define Power 1.60


#include <avr/io.h>
#include <util/delay.h>
#include "header.h"  // Include the UART header

int main(void)
{
    uart_init();  // Initialize UART before any transmission
	
    while (1) 
    {
		// Transmit RMSVoltage
        uart_transmit_string("RMS Voltage is: ");
        uart_transmit_float(RMSVoltage, 1);
        uart_transmit_byte('\r');
		uart_transmit_byte('\n');

        
        // Transmit PeakCurrent
        uart_transmit_string("Peak Current is: ");
        uart_transmit_integer(PeakCurrent);
        uart_transmit_byte('\r');
		uart_transmit_byte('\n');

        
        // Transmit Power
        uart_transmit_string("Power is: ");
        uart_transmit_float(Power, 2);
        uart_transmit_byte('\r');
		uart_transmit_byte('\n');
		
		// Add an extra newline for spacing between sets of outputs
		uart_transmit_byte('\r');
		uart_transmit_byte('\n');

        
        _delay_ms(1000);  // Wait for 1 second
		
    }
}

