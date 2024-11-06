/*
 * SmartEnergyMonitor.c
 *
 * Created: 25/09/2024 5:17:20 PM
 * Author : Ikenna, Ashwin, Raiyan, Murshid
 */

#include <avr/io.h>
#include "common.h"
#include "adc.h"
#include "timer0.h"
#include "external_interrupt.h"
#include "uart.h"
#include "math.h"
#include "timer1.h"
#include "display.h"
#include "timer2.h"
#define NUM_ITERATIONS 3

float Vrms = 0, totalVrms = 0;
float Irms = 0, totalIrms = 0;
float avgPower = 0, totalAvgPower = 0;
int16_t Ipeak = 0, totalIpeak = 0;
float trapPower = 0;

uint8_t iterationCount = 0;  // To track the number of iterations

int main(void) {
    cli();  // Disable global interrupts
    adc_init();  // Initialize ADC
    timer0_init();  // Initialize Timer0
    external_interrupt_init();  // Initialize external interrupts (INT0)
    timer1_init();
    uart_init();  // Initialize UART
    init_display();
    timer2_init();
    sei();  // Enable global interrupts
    DDRB |= (1 << DDB5);  // Set a pin for testing/debugging output

    while (1) {
        if (sampleRetrieved == 2) {  // Both voltage and current samples collected
            sampleRetrieved = 0;
            stop_sampling();  // Disable INT0 interrupt and ADC conversion

            TCCR2B |= (1 << CS22) | (1 << CS21);  // Turning on the clock for refresh rate

            // Process voltage and current samples
            for (uint8_t i = 0; i < SAMPLESIZE; i++) {
                vvsSamples[i] = calculate_vac(vvsSamples[i]);
                visSamples[i] = calculate_il(visSamples[i]);
            }

            // Find the maximum value in the current samples (Peak Current)
            for (uint8_t i = 0; i < SAMPLESIZE; i++) {
                if (visSamples[i] > Ipeak) {
                    Ipeak = visSamples[i];
                }
            }

            // Calculate RMS values, average power, and trapezoidal power
            Vrms = calculate_rms(vvsSamples, SAMPLESIZE);
            Irms = calculate_rms(visSamples, SAMPLESIZE);
            avgPower = calculate_avgPower(vvsSamples, visSamples, SAMPLESIZE);
            //trapPower = trapezoidal(vvsSamples, visSamples, SAMPLESIZE);

            // Accumulate values for averaging
            totalVrms += Vrms;
            totalIrms += Irms;
            totalAvgPower += avgPower;
            totalIpeak += Ipeak;

            iterationCount++;  // Increment iteration count

            if (iterationCount >= NUM_ITERATIONS) {
                // Calculate the average of 3 iterations
                float avgVrms = totalVrms / NUM_ITERATIONS;
                float avgIrms = totalIrms / NUM_ITERATIONS;
                float avgPowerFinal = totalAvgPower / NUM_ITERATIONS;
                int16_t avgIpeak = totalIpeak / NUM_ITERATIONS;

                // Reset accumulation variables for the next averaging cycle
                totalVrms = 0;
                totalIrms = 0;
                totalAvgPower = 0;
                totalIpeak = 0;
                iterationCount = 0;

                // Display and transmit the averaged results
                if (displayReady) {
                    displayReady = 0;

                    // Transmit RMS Voltage
                    uart_transmit_string("RMS Voltage: ");
                    uart_transmit_float(avgVrms, 1);
                    uart_transmit_string(" V\r\n");

                    // Transmit RMS Current
                    uart_transmit_string("RMS Current: ");
                    uart_transmit_float(avgIrms, 2);
                    uart_transmit_string(" A\r\n");

                    // Transmit Peak Current
                    uart_transmit_string("Peak Current: ");
                    uart_transmit_integer(avgIpeak);
                    uart_transmit_string(" mA\r\n");

                    // Transmit Average Power
                    uart_transmit_string("Real Power: ");
                    uart_transmit_float(avgPowerFinal, 4);
                    uart_transmit_string(" W\r\n");

                    // Extra newline for spacing
                    uart_transmit_byte('\r');
                    uart_transmit_byte('\n');

                    // Display mode logic for voltage/current/power
                    switch (display_mode) {
                        case 0:
                            display_voltage(avgVrms);  // Display RMS Voltage
                            break;
                        case 1:
                            display_current(avgIpeak);  // Display peak Current
                            break;
                        case 2:
                            display_power(avgPowerFinal);  // Display Average Power
                            break;
                    }
                }
            }

            // Reset peak current for the next set of samples
            Ipeak = 0;
            TCCR2B &= ~((1 << CS20) | (1 << CS21) | (1 << CS22));  // Turn off the clock for refresher rate

            start_sampling();  // Start the next set of samples
        }
    }
}
