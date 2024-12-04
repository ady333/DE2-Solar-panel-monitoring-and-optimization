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

// --Defines--------------------------------------------------------------------//
// define pins here
#define ADC_PIN 0
#define photorezistor_ADC = 2
//OLED is A5 and A4

// --Variables------------------------------------------------------------------//

volatile uint8_t flag_update_oled = 0;
uint8_t photoresistor_pins[] = {PD2, PD3}; // digital photoresistor pins

// sensor data var

// --Function definitions-------------------------------------------------------//

// -- Photoresistor reading function -------------------------------------------//

/**
 * @brief Reads the analog value from specified phtorezistor.
 *
 *  This function initiates an analog-to-digital conversion on the specified
 *  photozezistor by setting the digital pin high
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

    oled_charMode(DOUBLESIZE);
    oled_puts("OLED disp.");

    oled_charMode(NORMALSIZE);

    oled_gotoxy(0, 2);
    oled_puts("128x64, SH1106");

    // oled_drawLine(x1, y1, x2, y2, color)
    //oled_drawLine(0, 25, 120, 25, WHITE);

    // "The system will use a range of sensors to collect real-time data on solar radiation, energy output, and panel efficiency"
    oled_gotoxy(0, 4);
    //oled_puts("Solar radiation [Wh/m2]:);
    oled_gotoxy(0, 6);
    //oled_puts("Energy output [J]:");
    oled_gotoxy(0, 8);
    //oled_puts("Efficiency [\%]:");

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
    TIM1_ovf_262ms();

    sai();

    for (uint8_t i = 0; i < 4; i++)
    {
        GPIO_mode_output(&DDRD, photoresistor_pins[i]);
    }
    
    //oled placeholder loop
     while (1)
    {
        if (flag_update_oled == 1)
        {
            // Clear previous radiation value on OLED
            oled_gotoxy(0, 5);
            oled_puts("    ");

            // Display new radiation data
            oled_gotoxy(0, 5);
            //sprintf(oled_msg, "%u.%u", var?, var?);
            //oled_puts(oled_msg);

            // Clear previous energy values on OLED
            oled_gotoxy(0, 7);
            oled_puts("    ");

            // Display new energy data
            oled_gotoxy(0, 7);
            //sprintf(oled_msg, "%u.%u", var?, var?);
            //oled_puts(oled_msg);

            // Clear previous efficiency values on OLED
            oled_gotoxy(0, 9);
            oled_puts("    ");
        
            // Display new efficiency data
            oled_gotoxy(0, 9);
            //sprintf(oled_msg, "%u.%u", var?, var?);
            //oled_puts(oled_msg);
            
            oled_display();

            // Do not print it again and wait for the new data
            flag_update_oled = 0;
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
