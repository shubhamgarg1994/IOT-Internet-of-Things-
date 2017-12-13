/*/*
 * main.c
 *
 * Created: 27/12/2015 4:51:38 PM
 *  Author: Dell
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <inttypes.h>
#include <compat/twi.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lcd.h"   
#include "dht11.h"
#include "VC0706.h"
#include "esp8266_AP.h"
#include "ILI9163.h"
#include "color.h"
#include "ILI9361.h"
#include "remote.h"

#define F_CPU 16000000UL
							 /* Baud rate */
//#define BAUD_PRESCALLER 103	//9600	//(((F_CPU / (BAUDRATE * 16UL))) - 1)
//#define BAUD_PRESCALLER	68	//14400
//#define BAUD_PRESCALLER	51	//19200
//#define BAUD_PRESCALLER	34	//28800
#define BAUD_PRESCALLER	25	//38400
//#define BAUD_PRESCALLER	16	//57600
//#define BAUD_PRESCALLER	12	//76800
//#define BAUD_PRESCALLER	8	//115200

void test_lcd()
{
	lcd_init(LCD_ON_DISPLAY);
	
	/*  Blinking of Background Light*/ 
	
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);	
	
	lcd_puts_P("Hello World");
}

void test_sensor()
{
	char buf1[10];
	char buf2[10];
	int	dht11_temp=0, dht11_humidity=0;
    lcd_init(LCD_ON_DISPLAY);
    
	while(1)
	{
		// sensor data	
		
		dht11_temp = dht11_gettemperature();
		dht11_humidity = dht11_gethumidity();
		itoa(dht11_temp,buf1,10);
		itoa(dht11_humidity,buf2,10);
		lcd_gotoxy(0, 0);
		lcd_puts_P("Temp = ");
		lcd_gotoxy(10,0);
		lcd_puts(buf1);
		
		lcd_gotoxy(0, 1);
		lcd_puts_P("Moisture = ");
		lcd_gotoxy(10,1);
		lcd_puts(buf2);
	}
}
void test_uart()
{
	int number = 15;
	char data;
	USART_init(BAUD_PRESCALLER);        //Call the USART initialization code 
	USART_putnumber(number);
	
	while(1)
	{
		data = USART_receive();
		USART_send(data);		
	}
}
void test_wifisensor()
{
	char ip[20];
	
	USART_init(BAUD_PRESCALLER);
	lcd_init(LCD_ON_DISPLAY);
	
	//If ESP8266 is working
	USART_putstring("AT\n");
	wait_for_OK();
	
	lcd_gotoxy(0,0);
	lcd_puts_p("IP:");
	
	connectAccessPoint(SSID,PASS,ip);
	
	lcd_gotoxy(0,1);
	lcd_puts_p(ip);
}

void test_TFT_ILI9163()
{
	spi_init();
	init(ROT270);
	drawClear(BLACK);
	
	
	for (int i=0;i<21;i++)
			for (int j=0;j<16;j++)
				drawChar(textX(i,1), textY(j,1), '0' + rand()%10, 1, LIGHTGREY, BLACK);

	drawString(textX(4,1), textY(8,1), 1, YELLOW, BLUE, "Hello World!");
}

void test_TFT_ILI9361()
{
	ili9341_init();	

	lcdFillRGB(BLACK);

	lcdDrawVLine(120,100, 50,ILI9341_CYAN);
	lcdDrawHLine(120,100, 50, ILI9341_YELLOW);
	lcdFillCircle(50,50,10,ILI9341_CYAN);
	lcdDrawRectangle(20,20,30,30,ILI9341_MAGENTA);

	lcdDrawString("Hello World",5,120,1,ILI9341_WHITE);
	lcdDrawString("Eitan Herman",5,180,2,ILI9341_WHITE);
}
void test_IR_remote()
{
	uint8_t cmd=0;
	char buf[10];
	
	RemoteInit();

	lcd_init(LCD_ON_DISPLAY);

	lcd_clrscr();
	
	lcd_gotoxy(0,0);
	lcd_puts_P("IR RemoteDecoder");
	
	lcd_gotoxy(0,1);
	lcd_puts_p("Key Code:");

	while(1)
	{
		cmd=GetRemoteCmd(1);
		itoa(cmd,buf,10);
		
		lcd_gotoxy(10,1);
		lcd_puts_p(buf);
	}
	
}
void test_camera()
{
	begin(BAUD_PRESCALLER);
	USART_putstring("VC0706 Camera test\n");
	
	char *reply = getVersion();
	if (reply == 0) 
		USART_putstring("Failed to get version");
	
	 else
	  {
		USART_putstring("-----------------");
		USART_putstring(reply);
		USART_putstring("-----------------");
	  }
/*
	setImageSize(VC0706_640x480);
	
	uint8_t imgsize = getImageSize();
	USART_putstring("Image size: ");
	
	if (imgsize == VC0706_640x480) USART_putstring("640x480\n");
	if (imgsize == VC0706_320x240) USART_putstring("320x240\n");
	if (imgsize == VC0706_160x120) USART_putstring("160x120\n");

	USART_putstring("Get ready !");
	
			if (!takePicture())
				USART_putstring("Failed to snap!");
			else
				USART_putstring("Picture taken!");
*/
}

int main(void)
{
	//test_lcd();
	//test_uart();
	//test_wifisensor();
	//test_TFT_ILI9163();
	//test_IR_remote();
	//test_sensor();
	test_camera();
	//test_TFT_ILI9361();
}

