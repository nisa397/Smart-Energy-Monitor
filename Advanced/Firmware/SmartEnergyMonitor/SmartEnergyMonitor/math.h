/*
 * math.h
 *
 * Created: 9/10/2024 10:14:23 PM
 *  Author: Owner
 */ 


#ifndef MATH_H_
#define MATH_H_
#include <stdint.h>

extern int16_t visSamples[SAMPLESIZE];
extern int16_t vvsSamples[SAMPLESIZE];


int16_t calculate_vac(int16_t adc_value);  // Function to calculate voltage
int16_t calculate_il(int16_t adc_value);  // Function to calculate current
float calculate_avgPower(int16_t *voltage_samples, int16_t *current_samples, uint8_t num_values);  // Function to calculate power
float calculate_rms(int16_t voltage_samples[], uint8_t num_values);  // Function to calculate Vrms
int16_t calculate_ipeak(int16_t* current_samples, uint8_t N);  // Function to calculate Ipeak
float trapezoidal(int16_t *voltage_samples, int16_t *current_samples, uint8_t num_values);




#endif /* MATH_H_ */