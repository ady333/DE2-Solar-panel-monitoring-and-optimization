/*
Description place holder
*/

// --Includes-------------------------------------------------------------------//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>        
#include <twi.h>            
#include <oled.h>
#include <stdio.h>          
#include <adc.h>
#include <gpio.h>
#include <stdbool.h>
#include <servo.h>
#include <stdint.h>

// --Defines--------------------------------------------------------------------//
// define pins here
#define ADC_PIN 0
#define photorezistor_ADC = 2

// --Defines--------------------------------------------------------------------//
// define pins here
//OLED is A5 and A4

// panel
#define panel_voltage 0
#define panel_current 1

//calculation - NEEDS DEFINITION, offset, current const, high-low


// --Variables------------------------------------------------------------------//


// sensor data var
// -- Global variables -----------------------------------------------
uint8_t photoresistor_pins[] = {PD2, PD3}; // digital photoresistor pins

struct PanelData {
    int16_t voltage;
    int16_t current;
    int16_t power;
    int16_t servo_angle;
};
struct PanelData panel;
volatile bool flag_measure = false; 
volatile bool flag_update_oled = false;

// --Function definitions-------------------------------------------------------//

// -- Photoresistor reading function -------------------------------------------//

/**
 * @brief Reads the analog value from specified photoresistor.
 *
 *  This function initiates an analog-to-digital conversion on the specified
 *  photoresistor by setting the digital pin high
 * 
 * @param ADC_pin The ADC pin number from which to read the analog value.
 *           
 * @param digital_pin The digital pin number to set high to enable the photoresistor.
 * 
 * @return The 10-bit digital value corresponding to the analog input on the 
 *         specified ADC pin. The value ranges from 0 to 1023.
 */
uint16_t read_photoresistor(uint8_t ADC_pin, uint8_t digital_pin)
{
    static uint16_t adc_reading;

    //gpio high digital pin
    GPIO_write_high(&PORTD, digital_pin);
    adc_reading = adc_read(ADC_pin);
    GPIO_write_low(&PORTD, digital_pin);

    //gpio low digital pin

    return adc_reading;
}




// oled setup placeholder
void oled_setup(void)
{
    oled_init(OLED_DISP_ON);
    oled_clrscr();

    // Copy buffer to display RAM
    oled_display();
}

// main loop
int main(void)
{
    //initialization
    twi_init();
    oled_setup();
    adc_init();
    servo_init();
    
    TIM1_ovf_262ms(); //double check

    sai();

    for (uint8_t i = 0; i < 2; i++)
    {
        GPIO_mode_output(&DDRD, photoresistor_pins[i]);
    }
    
    // Initialize panel data
    panel.voltage = 0;
    panel.current = 0;
    panel.power = 0;
    panel.servo_angle = 0; 
    
    // Main loop
    while (1)
    {
        if (flag_measure)
        {
            // Read and calculate panel voltage
            uint16_t adc_value = adc_read(PANEL_VOLTAGE_ADC);
            panel.voltage = (adc_value / 1023.0) * 5000; // Convert to millivolts

            // Read and calculate panel current
            adc_value = adc_read(PANEL_CURRENT_ADC);
            panel.current = (adc_value / 1023.0) * 5000; // Convert to milliamps

            // Calculate power (P = V * I)
            panel.power = (panel.voltage * panel.current) / 1000; // Convert to milliwatts

            flag_measure = false; 
        }
        
        if (flag_update_oled)
        {
            // Update OLED display
            char current_str[8];
            char voltage_str[8];
            char power_str[8];

            // Current data display
            oled_gotoxy(0, 0);
            oled_puts("Current [mA]:");
            sprintf(current_str, "%d", panel.current);
            oled_gotoxy(0, 1);
            oled_puts("        ");  // Clear previous value
            oled_gotoxy(0, 1);
            oled_puts(current_str);
            
            // Voltage data display
            oled_gotoxy(0, 2);
            oled_puts("Voltage [mV]:");
            sprintf(voltage_str, "%d", panel.voltage);
            oled_gotoxy(0, 3);
            oled_puts("        ");  // Clear previous value
            oled_gotoxy(0, 3);
            oled_puts(voltage_str);
            
            // Power data display
            oled_gotoxy(0, 4);
            oled_puts("Power [mW]:");
            sprintf(power_str, "%d", panel.power);
            oled_gotoxy(0, 5);
            oled_puts("        ");  // Clear previous value
            oled_gotoxy(0, 5);
            oled_puts(power_str);

            // Update the OLED display
            oled_display();

            flag_update_oled = false;
        }
    }

    
  
}

// -- Interrupt service routines ------------------------------------------------//
ISR(TIMER0_COMPA_vect)
{
    //execute whole routine once a second
    static uint8_t cnt = 0;
    if (cnt == 3)
    {

        //read value from ADC
        static uint16_t solar_voltage;
        solar_voltage = adc_read(ADC_PIN);
            //TODO: Convert ADC value to voltage base on the refference voltage and resolution of the ADC


        //calculate power
        //TODO:

        //calculate energy
        //TODO:

        //calculate efficiency
        //TODO:

        //update OLED
        flag_update_oled = 1;
    }
    cnt++;
}

// -- end of file --//
