//--Includes-------------------------------------------------------------------//
#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"

//--Function definitions-------------------------------------------------------//

void adc_init(void)
{
    //TODO: determine which what setting would be best for the application

    ADMUX = 0x40; //Reference voltage is AVCC and 10-bit resolution
    ADCSRA |= 1<<ADEN; // Enable ADC
    ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0; // Set prescaler to 128
    ADCSRA |= 0<<ADIE; //turn off ADC interrupt
}

uint16_t adc_read(uint8_t ADC_pin)
{
    // Set ADC_pin
    ADC_pin &= 0x07; // AND operation to limit the range of ADC_pin to 0-5
    if (ADC_pin > 5)
    {
        return 1;
    }
    ADMUX |= ADC_pin; // Select ADC_pin

    // Start conversion
    ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC)); //loop till conversion is done

    return (ADC);
}