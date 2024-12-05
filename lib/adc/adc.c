//--Includes-------------------------------------------------------------------//
#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"

//--Function definitions-------------------------------------------------------//

void adc_init(void)
{
    ADMUX = 0x40; //Reference voltage is AVCC and 10-bit resolution
    ADCSRA |= 1<<ADEN; // Enable ADC
    ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0; // Set prescaler to 128
    ADCSRA &= ~(1<<ADIE);
}

uint16_t adc_read(uint8_t ADC_pin)
{
    //if invalid ADC pin return 1
    if (ADC_pin > 5 || ADC_pin < 0)
    {
        return 1;
    }
    ADMUX |= ADC_pin; // Select ADC_pin

    // Start conversion
    ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC)); //loop till conversion is done

    return (ADC);
}