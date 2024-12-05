#include <avr/io.h>
#include <adc.h>
#include <uart.h>
#include <stdlib.h>
#include <avr/interrupt.h>



char string[10];

int main(void) {
    uart_init(UART_BAUD_SELECT(9600, F_CPU));
    adc_init();
    
    uart_puts("ADC Test\n");

    sei();
    
    while (1) {
        static uint16_t adc_value;
        adc_value = adc_read(0);
        uart_puts("ADC Value: ");
        itoa(adc_value, string, 10);
        uart_puts(string);
        uart_puts("\n");
    }

    return 0;
}

