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
    //oled_puts("Temperature [C]:");
    oled_gotoxy(0, 7);
    //oled_puts("Efficiency [\%]:");

    // Copy buffer to display RAM
    //oled_display();
}

// main loop
int main(void)
{
    //oled init
    twi_init();
    oled_setup();
    //oled placeholder loop
     while (1)
    {
        if (flag_update_oled == 1)
        {
            // Clear previous temperature value on OLED
            oled_gotoxy(17, 6);
            oled_puts("    ");

            // Display new temperature data
            oled_gotoxy(17, 6);
            sprintf(oled_msg, "%u.%u", dht12_values[2], dht12_values[3]);
            oled_puts(oled_msg);

            // Clear previous humidity values on OLED
            oled_gotoxy(17, 7);
            oled_puts("    ");

            // Display new humidity data
            oled_gotoxy(17, 7);
            sprintf(oled_msg, "%u.%u", dht12_values[0], dht12_values[1]);
            oled_puts(oled_msg);

            oled_display();

            // Do not print it again and wait for the new data
            flag_update_oled = 0;
        }
    }

    
  
}

// -- Interrupt service routines ------------------------------------------------//




// -- end of file --//
