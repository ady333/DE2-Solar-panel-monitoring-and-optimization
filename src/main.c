/*
Description place holder
*/

// --Includes-------------------------------------------------------------------//
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"          
#include <twi.h>            
#include <oled.h>
#include <stdio.h>          
#include "adc.h"

// --Defines--------------------------------------------------------------------//
// define pins here
#define ADC_PIN 0

// --Variables------------------------------------------------------------------//
// -- Global variables -----------------------------------------------
volatile uint8_t flag_update_oled = 0;
// sensor data var

// --Function definitions-------------------------------------------------------//
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

        //read value from I2C shunt
        //TODO:

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
