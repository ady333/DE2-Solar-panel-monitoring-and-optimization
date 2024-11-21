#ifndef ADC_H
# define ADC_H


// -- Includes-------------------------------------------------------------------//
#include <avr/io.h>


//-- Function prototypes --------------------------------------------------------//

/**
 * @brief Initializes ADC peripheral.
 *
 * This function configures and initializes the ADC peripheral for use.
 * It sets up the necessary registers and parameters to enable ADC
 * functionality.
 */
void adc_init(void);
   
/**
 * @brief Reads the analog value from the specified ADC pin.
 *
 * This function initiates an analog-to-digital conversion on the specified 
 * ADC pin and returns the resulting digital value.
 *
 * @param ADC_pin The ADC pin number from which to read the analog value.
 *                This should be a valid ADC pin number for the microcontroller.
 * 
 * @return The 10-bit digital value corresponding to the analog input on the 
 *         specified ADC pin. The value ranges from 0 to 1023.
 */
uint16_t adc_read(uint8_t ADC_pin);

#endif