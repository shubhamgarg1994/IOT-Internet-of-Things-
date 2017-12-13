/*
 * DHT11_Atmega328.c
 *
 * Created: 27/12/2015 3:27:21 PM
 *  Author: Shubham Garg
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "avr_compat.h"
#include "hw_hd44780.h"
#include "hw_ds18b20.h"
#include "aux_globals.h"

int		milis = 0,sec = 0, 
		// sensor data
		dht11_temp=0, dht11_humidity=0;
 /*
	timer0 overflow interrupt
	event to be executed every  1.024ms here
*/
ISR (TIMER0_OVF_vect)  
{
	milis ++;
	if (milis >= 976) {
		 sec++;
		 milis = 0; // 976 x 1.024ms ~= 1000 ms = 1sec
		
		if (1)
		{//sec%1 == 0) { //every 2 seconds
			// read sensors //
			
			// ds18b20
			ds18b20_temp = therm_read_temperature();
			// dht-11
			int tmp_humi = 0 , tmp_temp = 0;
			if (DHT11_read(&tmp_temp, &tmp_humi) == 0) {
				dht11_temp = tmp_temp;
				dht11_humidity = tmp_humi;
			}			
		}	
		if (sec%60 ==0) {
			sec = 0;
		}
	}				
}

int main(void)
{
    while(1)
    {
        
    }
}