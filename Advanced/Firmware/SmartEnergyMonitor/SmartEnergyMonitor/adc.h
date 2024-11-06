/*
 * adc.h
 *
 * Created: 7/10/2024 4:57:03 PM
 *  Author: iuna331
 */ 


#ifndef ADC_H_
#define ADC_H_

void adc_init();
extern volatile uint8_t sampleRetrieved;
void stop_sampling();
void start_sampling();
#define TOGGLE_MUX0()  (ADMUX ^= (1 << MUX0))

#endif /* ADC_H_ */