#ifndef ADC_H
# define ADC_H


// -- Includes-------------------------------------------------------------------//
#include <avr/io.h>


//-- Function prototypes --------------------------------------------------------//
void adc1_init(void);
    
uint16_t adc_read(uint8_t ADC_pin);

#endif