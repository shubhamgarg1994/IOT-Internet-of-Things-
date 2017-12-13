#ifndef _GLOBALS_H
#define _GLOBALS_H

#define F_CPU 16000000UL 		//Your clock speed in Hz (16Mhz here)

//-----------------delays---------------------------------------------------------
#define LOOP_CYCLES 8 				//Number of cycles that the loop takes

#define fcpu_delay_us(num) delay_int(num/(LOOP_CYCLES*(1/(F_CPU/1000000.0))))
#define fcpu_delay_ms(num) delay_int(num/(LOOP_CYCLES*(1/(F_CPU/1000.0))))


void delay_int(unsigned long delay)
{
	while(delay--) asm volatile("nop");
};

#endif
