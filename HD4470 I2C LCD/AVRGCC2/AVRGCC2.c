/*
 *  Author: Shubham Garg
	27th December 2015
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <inttypes.h>
#include <compat/twi.h>
#include "lcd.h"   
        
int main(void)
{
    lcd_init(LCD_ON_DISPLAY);

	lcd_backlight(0);

	_delay_ms(500);

	lcd_backlight(1);

	_delay_ms(500);

        lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts_P("Hello World");
		
}


