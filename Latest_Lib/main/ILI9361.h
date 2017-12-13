
#include "ili9341_regs.h"
#include "font.h"

/* small color graphics LCD based on ILI9341 chip */

/* hardware definition. Data is in SPI */
#define PORT_ILI9361    PORTD
#define DDR_ILI9361     DDRD

#define CONF_ILI9341_CS_PIN         DDB2
#define CONF_ILI9341_DC_PIN         DDB3
#define CONF_ILI9341_RESET_PIN     DDB4

//TFT resolution 240*320
#define MIN_X	0
#define MIN_Y	0
#define MAX_X	239
#define MAX_Y	319

#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8

#define ILI9341_COLOR(red, green, blue)   ((unsigned int)( (( red >> 3 ) << 11 ) | (( green >> 2 ) << 5  ) |  ( blue  >> 3 ))) 

 #define ILI9341_BLACK                       ILI9341_COLOR(0x00, 0x00, 0x00) 
 #define ILI9341_WHITE                       ILI9341_COLOR(0xFF, 0xFF, 0xFF) 
 #define ILI9341_RED                         ILI9341_COLOR(0xFF, 0x00, 0x00) 
 #define ILI9341_GREEN                       ILI9341_COLOR(0x00, 0xFF, 0x00) 
 #define ILI9341_BLUE                        ILI9341_COLOR(0x00, 0x00, 0xFF) 
 #define ILI9341_YELLOW                      ILI9341_COLOR(0xFF, 0xFF, 0x00) 
 #define ILI9341_MAGENTA                     ILI9341_COLOR(0xFF, 0x00, 0xFF) 
 #define ILI9341_CYAN                        ILI9341_COLOR(0x00, 0xFF, 0xFF) 
 #define ILI9341_GRAY                        ILI9341_COLOR(0x80, 0x80, 0x40) 
 #define ILI9341_SILVER                      ILI9341_COLOR(0xA0, 0xA0, 0x80) 
 #define ILI9341_GOLD                        ILI9341_COLOR(0xA0, 0xA0, 0x40) 


	
#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8


#define LCD_WIDTH 320
#define LCD_HEIGHT 240

 void ili9341_select_chip(void)
{
	PORT_ILI9361 &= ~CONF_ILI9341_CS_PIN;
}
 void ili9341_deselect_chip(void)
{
	PORT_ILI9361 |= CONF_ILI9341_CS_PIN;
}
 void ili9341_select_command_mode(void)
{
	PORT_ILI9361 &= ~CONF_ILI9341_DC_PIN;
}
 void ili9341_select_data_mode(void)
{
	PORT_ILI9361 |= CONF_ILI9341_DC_PIN;
}
 void ili9341_send_byte(uint8_t data)
{
	//ili9341_select_chip();
	spi_fast_shift(data);
}
 void ili9341_send_word(unsigned int data)
{
	spi_fast_shift(data >> 8);
	spi_fast_shift(data );
}
 unsigned char ili9341_read_byte(void)
{
	ili9341_select_chip();
	return spi_fast_shift(0);
}
void ili9341_send_command(unsigned char command)
{
    ili9341_select_command_mode();
    ili9341_select_chip();
    ili9341_send_byte(command);
    ili9341_select_data_mode();
}
void ili9341_interface_init(void)
{ 
	DDR_ILI9361 |= CONF_ILI9341_RESET_PIN | CONF_ILI9341_DC_PIN | CONF_ILI9341_CS_PIN;
	ili9341_deselect_chip();
	spi_init();
	spi_fast_shift(0);
}
void ili9341_write_buffer(char *buffer, unsigned char len)
{
unsigned char count, data1; //, data2;
ili9341_select_data_mode();
ili9341_select_chip();
for(count=0;count<len;count++)
	{
		data1 = buffer[count]; //>>8;
		//data2 = buffer[count] & 0xFF;
		spi_fast_shift( data1 );
		//spi_fast_shift( data2 );
	}
}

unsigned char ili9341_read_register(unsigned char addr, unsigned char xparam)
{
unsigned char data;
	ili9341_send_command(0xD9); /* ext command                  */
	ili9341_send_byte(0x10+xparam); /* 0x11 is the first Parameter  */
	ili9341_select_command_mode();
	ili9341_select_chip();
	ili9341_send_byte(addr);
	ili9341_select_data_mode();
	data=ili9341_read_byte();
	ili9341_deselect_chip();
	return data;
}

//INT8U TFT::readID(void)
unsigned char ili9341_read_ID(void)
{
unsigned char i;
unsigned char data[3];
unsigned char id[3]= {0x00, 0x93, 0x41};
unsigned char ToF=0xFF;
    for(i=0;i<3;i++)
    {
        data[i]=ili9341_read_register(0xd3,i+1);
        if(data[i] != id[i])
        {
            ToF=0;
        }
    }

    if(!ToF)                                                            /* data!=ID */
    {
		while(1){}
	}
   return ToF;
}

void ili9341_controller_init_registers(void)
{
    unsigned char i=0, TFTDriver=0;
    for(i=0;i<3;i++)
	{
		TFTDriver = ili9341_read_ID();
	}
	_delay_ms(100);

	ili9341_send_command(ILI9341_CMD_SOFTWARE_RESET);
	_delay_ms(200);	
	
	ili9341_send_command(ILI9341_CMD_SLEEP_OUT);	// Exit Sleep
	ili9341_deselect_chip();
    _delay_ms(220);

	ili9341_send_command(ILI9341_CMD_POWER_CONTROL_B);
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x8B); //83
    ili9341_send_byte(0X30);

    ili9341_send_command(0xED); //Power on sequence control
    ili9341_send_byte(0x67); //64
    ili9341_send_byte(0x03); 
    ili9341_send_byte(0X12);
    ili9341_send_byte(0X81);
    ili9341_send_command(ILI9341_CMD_DRIVER_TIMING_CONTROL_A);
    ili9341_send_byte(0x85);
    ili9341_send_byte(0x10);
    ili9341_send_byte(0x7A); //79

    ili9341_send_command(ILI9341_CMD_POWER_ON_SEQ_CONTROL);
    ili9341_send_byte(0x39);
    ili9341_send_byte(0x2C);
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x34);
    ili9341_send_byte(0x02);

    ili9341_send_command(ILI9341_CMD_PUMP_RATIO_CONTROL);
    ili9341_send_byte(0x20);

    ili9341_send_command(ILI9341_CMD_DRIVER_TIMING_CONTROL_B);
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x00);

    ili9341_send_command(ILI9341_CMD_POWER_CONTROL_1 );		// Power control                
    ili9341_send_byte(0x1B); //26								// VRH[5:0]       

    ili9341_send_command(ILI9341_CMD_POWER_CONTROL_2);		// Power control                
    ili9341_send_byte(0x10); //11							// SAP[2:0];BT[3:0]             

    ili9341_send_command(ILI9341_CMD_VCOM_CONTROL_1);		// VCM control                  
    ili9341_send_byte(0x3F); //35
    ili9341_send_byte(0x3C); //3e

    ili9341_send_command(ILI9341_CMD_VCOM_CONTROL_2);		// VCM control2                 
    ili9341_send_byte(0xB7); //be

    ili9341_send_command(ILI9341_CMD_MEMORY_ACCESS_CONTROL);	// Memory Access Control        
    ili9341_send_byte(0x08);

    ili9341_send_command(ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET);
    ili9341_send_byte(0x55);

    ili9341_send_command(ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL);
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x1B);

    ili9341_send_command(ILI9341_CMD_DISPLAY_FUNCTION_CONTROL);	// Display Function Control    
    ili9341_send_byte(0x0A);
    ili9341_send_byte(0xA2);

    ili9341_send_command(ILI9341_CMD_ENABLE_3_GAMMA_CONTROL);	// 3Gamma Function Disable     
    ili9341_send_byte(0x00);

    ili9341_send_command(ILI9341_CMD_GAMMA_SET); 				// Gamma curve selected         
    ili9341_send_byte(0x01);

    ili9341_send_command(ILI9341_CMD_POSITIVE_GAMMA_CORRECTION);	// Set Gamma                  
    ili9341_send_byte(0x0F);
    ili9341_send_byte(0x2A);
    ili9341_send_byte(0x28);
    ili9341_send_byte(0x08);
    ili9341_send_byte(0x0E);
    ili9341_send_byte(0x08);
    ili9341_send_byte(0x54);
    ili9341_send_byte(0XA9);
    ili9341_send_byte(0x43);
    ili9341_send_byte(0x0A);
    ili9341_send_byte(0x0F);
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x00);

    ili9341_send_command(ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION);	// Set Gamma         
    ili9341_send_byte(0x00);
    ili9341_send_byte(0x15);
    ili9341_send_byte(0x17);
    ili9341_send_byte(0x07);
    ili9341_send_byte(0x11);
    ili9341_send_byte(0x06);
    ili9341_send_byte(0x2B);
    ili9341_send_byte(0x56);
    ili9341_send_byte(0x3C);
    ili9341_send_byte(0x05);
    ili9341_send_byte(0x10);
    ili9341_send_byte(0x0F);
    ili9341_send_byte(0x3F);
    ili9341_send_byte(0x3F);
    ili9341_send_byte(0x0F);
}

void ili9341_reset_display(void)
{
	PORT_ILI9361 |= CONF_ILI9341_RESET_PIN;
	_delay_ms(20);
	PORT_ILI9361 &= ~CONF_ILI9341_RESET_PIN;
	_delay_ms(120);
	PORT_ILI9361 |= CONF_ILI9341_RESET_PIN;
	_delay_ms(250);
}

void ili9341_exit_standby(void)
{
    ili9341_send_command(ILI9341_CMD_SLEEP_OUT);
    _delay_ms(150);
    ili9341_send_command(ILI9341_CMD_DISPLAY_ON);
}
void ili9341_init(void)
{
    /* Initialize the communication interface */
    ili9341_interface_init();

    /* Harware Reset the display */
    ili9341_reset_display();

    /* Write all the controller registers with correct values */
    ili9341_controller_init_registers();
	    /* Send commands to exit standby mode */
    ili9341_exit_standby();
	ili9341_deselect_chip();
	
}



void ili9341_setCol(unsigned int x0,unsigned int x1)
{
    ili9341_send_command(0x2A);                                                      /* Column Command address       */
    ili9341_send_word(x0);
    ili9341_send_word(x1);
}

void ili9341_setPage(unsigned int y0,unsigned int y1)
{
    ili9341_send_command(0x2B);                                                      /* Column Command address       */
    ili9341_send_word(y0);
    ili9341_send_word(y1);
}

void ili9341_setXY(unsigned int x, unsigned int y)
{
    ili9341_setCol(x, x);
    ili9341_setPage(y, y);
}


/* ********************** common subs ***************** */

void lcdDrawPixel(unsigned int poX, unsigned int poY, unsigned int color)
{
    ili9341_setXY(poX, poY);
	ili9341_send_command(0x2c);
    ili9341_send_word(color);
}
/* ******** same as drawpixel, but bigger ***** */
void lcdFilledRec(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned int color)
{
unsigned long i;
unsigned long n=(unsigned long) w*h;
unsigned int x1=x+w-1;
unsigned int y1=y+h-1;

    ili9341_setCol( x, x1);
    ili9341_setPage(y, y1);
    ili9341_send_command(0x2c);
	for(i=0;i<n;i++) { ili9341_send_word(color); }
}
/* ********** Clear screen ************ */
void lcdFillRGB(uint16_t color)
{
	lcdFilledRec(0,0,LCD_HEIGHT,LCD_WIDTH,color);
}
/***********H Line*************************************/
void lcdDrawHLine(uint16_t x0, uint16_t y, uint16_t x1, uint16_t color)
{
  // Allows for slightly better performance than setting individual pixels
  uint16_t x, pixels;

  if (x1 < x0)
  {
    // Switch x1 and x0
    x = x1;
    x1 = x0;
    x0 = x;
  }

  // Check limits
  if (x1 >= LCD_WIDTH)
  {
    x1 = LCD_WIDTH ;
  }
  if (x0 >= LCD_WIDTH)
  {
    x0 = LCD_WIDTH;
  }
    ili9341_setCol( x0, x1);
    ili9341_setPage(y, y);
    ili9341_send_command(0x2c);
  
   for (pixels = 0; pixels < x1 - x0 ; pixels++)
  {
    ili9341_send_word(color);
  }
  
}
/***********V Line*************************************/
void lcdDrawVLine(uint16_t x, uint16_t y0, uint16_t y1, uint16_t color)
{
 // Allows for slightly better performance than setting individual pixels
  uint16_t y, pixels;

  if (y1 < y0)
  {
    // Switch y1 and y0
    y = y1;
    y1 = y0;
    y0 = y;
  }

  // Check limits
  if (y1 >= LCD_HEIGHT)
  {
    y1 = LCD_HEIGHT - 1;
  }
  if (y0 >= LCD_HEIGHT)
  {
    y0 = LCD_HEIGHT - 1;
  }

    ili9341_setCol( x, x);
    ili9341_setPage(y0, y1);
    ili9341_send_command(0x2c);
	
  for (pixels = 0; pixels < y1 - y0 ; pixels++)
  {
	ili9341_send_word(color);
  }

}

void lcdDrawBMP(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned char *buffer)
{
unsigned int i, color;
unsigned char r,g,b;
    ili9341_setCol( x, x+w-1);
    ili9341_setPage(y,y+h-1);
  
  ili9341_send_command(0x2c);  // write to RAM
  for(i=0;i < (w * h);i++)
  {
	r = *buffer++;
	g = *buffer++;
	b = *buffer++;
	color=ILI9341_COLOR(r,g,b);
	ili9341_send_word(color);
  }
  
}
/* ************** copy bitmap picture to screen (compressed RGB, 16bit) ********** */ 
void lcdDrawCBMP(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned char *buffer)
{
unsigned int i;
unsigned char c1,c2;
    ili9341_setCol( x, x+w-1);
    ili9341_setPage(y,y+h-1);
  ili9341_send_command(0x2c);  // write to RAM
  for(i=0;i < (w * h);i++)
  {
  c1 = *buffer++;
  c2 = *buffer++;
  ili9341_send_byte(c1 >> 8);  
  ili9341_send_byte(c2);
  }  
}

/* ********draw simple frame (rectangle ) *********** */
void lcdDrawRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color)
{
    lcdDrawHLine(poX, poY, poX + length-1, color);
    lcdDrawHLine(poX, poY + width-1, poX+length, color);
	
    lcdDrawVLine(poX, poY, poY + width-1, color);
    lcdDrawVLine(poX + length-1, poY, poY+width-1, color);
}

/* yeap, round circule */
void lcdDrawCircle(int poX, int poY, int r,uint16_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        lcdDrawPixel(poX-x, poY+y,color);
        lcdDrawPixel(poX+x, poY+y,color);
        lcdDrawPixel(poX+x, poY-y,color);
        lcdDrawPixel(poX-x, poY-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}
void lcdFillCircle(int poX, int poY, int r,uint16_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {

        lcdDrawVLine(poX-x, poY-y, poY-y+ 2*y, color);
        lcdDrawVLine(poX+x, poY-y, poY-y+ 2*y, color);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);

}

/* draw char or print text. Two versions of fonts- monospace and proportional */

void lcdDrawChar( unsigned char ascii, unsigned char poX, unsigned char poY,unsigned char size, unsigned int fgcolor)
{
    if((ascii>=32)&&(ascii<=127))
    {
    }
    else
    {
        ascii = '?'-32;
    }
    for (int i =0; i<FONT_X; i++ ) {
        unsigned char temp = pgm_read_byte(&simpleFont[ascii-0x20][i]);
        for(unsigned char f=0;f<8;f++)
        {
            if((temp>>f)&0x01)
            {      
				if(size==1) {lcdDrawPixel(poX+i*size, poY+f*size,fgcolor);}
				else {lcdFilledRec(poX+i*size, poY+f*size, size, size, fgcolor);}
            }
        }
    }
}

void lcdDrawString(const char *string,unsigned char poX, unsigned char poY, unsigned char size,unsigned int fgcolor)
/* print string on lcd (no auto linefeed) */
{
    unsigned char c;
    while ( (c = *string++) )
	{
		lcdDrawChar(c, poX, poY, size, fgcolor);
        if(poX < LCD_WIDTH)
        {
            poX += FONT_SPACE*size;                                     /* Move cursor right  */
        }
    }
}