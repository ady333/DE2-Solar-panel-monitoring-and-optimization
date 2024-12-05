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
#define photorezistor_ADC 2
#define servo_PWM PB2
// --Defines--------------------------------------------------------------------//
// define pins here
//OLED is A5 and A4

// panel
#define PANEL_VOLTAGE_ADC 0
#define PANEL_CURRENT_ADC 1

//calculation - NEEDS DEFINITION, offset, current const, high-low
uint16_t I0 = 1;

uint16_t R_load = 24;

uint16_t U_supply = 5;

uint16_t module = 185; // sensitivity of current sensor [mV/A]

volatile uint8_t round = 0;

volatile uint16_t I_sum = 0;

volatile uint16_t I_offset = 0;

volatile uint16_t I_radiation_final = 0;

volatile uint16_t power_final_panel = 0;

volatile uint16_t I_radiation = 0;

uint8_t efficiency = 0;

volatile uint16_t energy_panel = 0;

volatile uint16_t power_of_panel = 0;

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

int16_t photores_difference(uint16_t photoresistor_values[2])
{
    //read values from photoresistors
    int16_t diff = 0;
    for (uint8_t i = 0; i < 2; i++)
    {
        photoresistor_values[i] = read_photoresistor(photorezistor_ADC, photoresistor_pins[i]);
    }
    diff = (photoresistor_values[0] - photoresistor_values[1]) * 180 / 1023;
    return diff;
}

void pwm_init(void)
{
    //set fast PWM mode with non-inverted output
    TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B |= (1 << CS00); //no prescaler
    //set output pin
    DDRB |= (1 << servo_PWM);
}

void servo_set_angle(uint16_t angle)
{
    //calculate duty cycle
    static uint8_t duty = 90;
    duty = (angle / 180.0) * 255;
    OCR0A = duty;
}

void determine_servo_shift(int16_t difference)
{
    //determine the shift of the servo
    if (difference > -5 && difference < 5)
    {
        return;
    }
    panel.servo_angle = panel.servo_angle + difference;
    if (panel.servo_angle > 180)
    {
        panel.servo_angle = 180;
    }
    else if (panel.servo_angle < 0)
    {
        panel.servo_angle = 0;
    }
    servo_set_angle(panel.servo_angle);
    return;
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

    sei();

    for (uint8_t i = 0; i < 2; i++)
    {
        GPIO_mode_output(&DDRD, photoresistor_pins[i]);
    }

    GPIO_mode_output(&DDRB, servo_PWM);

    
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
            // VALUES ONLY PLACEHOLDER. Adame prosím zkontroluj, případně urči konstanty
            uint16_t adc_value = adc_read(PANEL_VOLTAGE_ADC);
            panel.voltage = (adc_value / 1023.0) * 5000; // Convert to millivolts

            adc_value = adc_read(PANEL_CURRENT_ADC);
            panel.current = (adc_value / 1023.0) * 5000; // Convert to milliamps

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
ISR(TIMER1_COMPA_vect)
{
    //execute whole routine once a second
    static uint8_t cnt = 0;
    if (cnt == 3)
    {

        //read value from ADC
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
