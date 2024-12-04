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
struct data panel_data; // USE FOR STORAGE OF MEASURED DATA
volatile uint8_t flag_update_oled = 0;
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
    oled_charMode(DOUBLESIZE);
    oled_puts("OLED disp.");
    oled_charMode(NORMALSIZE);

    // "The system will use a range of sensors to collect real-time data on solar radiation, energy output, and panel efficiency"
    /*
    oled_gotoxy(0, 4);
    //oled_puts("Solar radiation [Wh/m2]:);
    oled_gotoxy(0, 6);
    //oled_puts("Energy output [J]:");
    oled_gotoxy(0, 8);
    //oled_puts("Efficiency [\%]:");
    */
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

    for (uint8_t i = 0; i < 4; i++)
    {
        GPIO_mode_output(&DDRD, photoresistor_pins[i]);
    }
    panel_data.voltage = 0;
    panel_data.current = 0;
    panel_data.power = 0;
    panel_data.servo_angle = 0; 
    sei();
    
    //oled placeholder loop
     while (1)
    {
        // LOOP FOR TILT, switch case or ifelif? 
        
        if (flag_measure) {
            cli();
            panel_data.voltage = analog_read(panel_voltage); //vypocitat
            panel_data.current = analog_read(panel_current); //vypocitat
            flag_measure = false; 
            sei();
        }
        
        if (flag_update_oled)
        {
            cli();

            char current[4];
            char voltage[4];
            char servo_angle[2];
            char power[4];

            // current data disp
            oled_gotoxy(0, 0);
            oled_puts("Current [mA]:");
            oled_gotoxy(0, 1);
            sprintf(current, "%d", panel_data.current) //debug only
            oled_puts("    ");
            oled_gotoxy(0,1);
            oled_puts(current);
            
            // voltage data disp
            oled_gotoxy(0, 3);
            oled_puts("Voltage mV]:");
            oled_gotoxy(0, 4);
            sprintf(voltage, "%d", panel_data.voltage) //debug only
            oled_puts("    ");
            oled_gotoxy(0,4);
            oled_puts(voltage);
            
            // current data disp
            oled_gotoxy(0, 5);
            oled_puts("Power [mW/m2]:");
            oled_gotoxy(0, 6);
            sprintf(power, "%d", panel_data.power) //debug only
            oled_puts("    ");
            oled_gotoxy(0,6);
            oled_puts(power);
            
            // servo needs to be added

            oled_display();
            sei();
            // Do not print it again and wait for the new data
            flag_update_oled = false; // here or elsewhere???
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
