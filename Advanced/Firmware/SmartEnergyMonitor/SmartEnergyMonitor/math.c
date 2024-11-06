/*
 * math.c
 *
 * Created: 9/10/2024 10:14:11 PM
 *  Author: Owner
 */ 


#include "common.h"
#include "math.h"


int16_t vvsSamples[SAMPLESIZE];
int16_t visSamples[SAMPLESIZE];

// Define the offset voltage and gain values
#define Voff 2.065
#define Gvs 0.044715
#define Gvo 1
#define Gis 0.5
#define Gio 2.35

// Function to calculate the voltage from the ADC values
int16_t calculate_vac(int16_t adc_value) {
	// (ADC0Value * 5 / 1024) - Voff / (Gvs * Gvo)
	int32_t voltage = (int32_t)(adc_value) * 5000 / 1024;   // Scale by 1000 to avoid floating point
	voltage -= (Voff * 1000);  // Subtract the offset (scaled by 1000)
	voltage = voltage / (Gvs * Gvo);  // Apply the gain scaling*/

	return (int16_t) voltage;  // Return the calculated voltage
}

// Function to calculate the current from the ADC values
int16_t calculate_il(int16_t adc_value) {
	// (ADC1Value * 5 / 1024) - Voff / (Gis * Gio)
	int32_t current = (int32_t)(adc_value) * 5000 / 1024;  // Scale by 1000 to avoid floating point
	current -= (Voff * 1000);  // Subtract the offset (scaled by 1000)
	current = current / (Gis * Gio);  // Apply the gain scaling*/

	return (int16_t)current;  // Return the calculated current
}

// Function to calculate Vrms (V)
float calculate_rms(int16_t values[], uint8_t num_values) {
    // Variable to store the sum of squares (using int64_t to prevent overflow)
    int64_t sum_of_squares = 0;

    // Calculate the sum of the squares of the values
    for (int i = 0; i < num_values; i++) {
        sum_of_squares += (int64_t)values[i] * values[i];
    }

    // Store the mean of squares in int32_t (since it will fit)
    int32_t mean_of_squares = (int32_t)(sum_of_squares / num_values);

    // Calculate the RMS using double precision for sqrt
    double rms_double = sqrt((double)mean_of_squares);

    // Return the RMS value divided by 1000 as a float
    return (float)(rms_double / 1000.0);
}

// Function to calculate Ipeak
int16_t calculate_ipeak(int16_t* current_samples, uint8_t N) {
	int16_t ipeak = 0;

	// Find the maximum value in the current samples
	for (uint8_t i = 0; i < N; i++) {
		if (current_samples[i] > ipeak) {
			ipeak = current_samples[i];
		}
	}

	return ipeak;
}




// Function to calculate Vrms (V)
float calculate_avgPower(int16_t *voltage_samples, int16_t *current_samples, uint8_t num_values) {
	// Variable to store the sum of squares (using int64_t to prevent overflow)
	int64_t totalpower = 0;

	// Calculate the sum of the squares of the values
	for (int i = 0; i < num_values; i++) {
		totalpower += (int64_t)current_samples[i] * voltage_samples[i];
	}

	// Store the mean of squares in int32_t (since it will fit)
	int32_t mean = (int32_t)((totalpower / num_values)/1000);


	// Return the RMS value divided by 1000 as a float
	return (float)(mean / 1000.0);
}

float trapezoidal(int16_t *voltage_samples, int16_t *current_samples, uint8_t num_values) {
	// Variable to store the sum of the power values (using int64_t to prevent overflow)
	int64_t total_power = 0;

	// Apply the trapezoidal rule for power calculation
	for (int i = 0; i < num_values - 1; i++) {
		int32_t power1 = (int32_t)current_samples[i] * voltage_samples[i];
		int32_t power2 = (int32_t)current_samples[i + 1] * voltage_samples[i + 1];
		
		// Calculate the average power between two consecutive points
		total_power += (power1 + power2) / 2;
	}

	// Calculate the mean of the trapezoidal sums
	double mean_power = (double)total_power / (num_values - 1);

	// Return the average power scaled properly (divided by 1000 twice to adjust scaling)
	return (float)(mean_power / 1000000.0);
}