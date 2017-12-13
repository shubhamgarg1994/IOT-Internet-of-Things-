/*
lcdpcf8574 lib sample

copyright (c) Davide Gironi, 2013

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define UART_BAUD_RATE 9600
#include "uart.h"

#define F_CPU 8000000

int main(void)
{
	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
    sei();
    uint8_t led = 0;

    while(1) {
    	int i = 0;
    	int line = 0;
    	for(i=0; i<10; i++) {
    		char buf[10];
    		itoa(i, buf, 10);
        	uart_puts(buf);
        	uart_puts("\r\n");
    		_delay_ms(500);
    	}
    }
}


