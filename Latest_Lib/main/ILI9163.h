
#include <string.h>
#include <stdio.h>
#include "font5x8.h"
#include "color.h"
#include "spi.h"

// define size
#define ILI9163_HEIGHT 						128
#define ILI9163_WIDTH 						128

// ILI9163 LCD Controller Commands
#define ILI9163_CMD_NOP                     0x00
#define ILI9163_CMD_SOFT_RESET              0x01
#define ILI9163_CMD_GET_RED_CHANNEL         0x06
#define ILI9163_CMD_GET_GREEN_CHANNEL       0x07
#define ILI9163_CMD_GET_BLUE_CHANNEL        0x08
#define ILI9163_CMD_GET_PIXEL_FORMAT        0x0C
#define ILI9163_CMD_GET_POWER_MODE          0x0A
#define ILI9163_CMD_GET_ADDRESS_MODE        0x0B
#define ILI9163_CMD_GET_DISPLAY_MODE        0x0D
#define ILI9163_CMD_GET_SIGNAL_MODE         0x0E
#define ILI9163_CMD_GET_DIAGNOSTIC_RESULT   0x0F
#define ILI9163_CMD_ENTER_SLEEP_MODE        0x10
#define ILI9163_CMD_EXIT_SLEEP_MODE         0x11
#define ILI9163_CMD_ENTER_PARTIAL_MODE      0x12
#define ILI9163_CMD_ENTER_NORMAL_MODE       0x13
#define ILI9163_CMD_EXIT_INVERT_MODE        0x20
#define ILI9163_CMD_ENTER_INVERT_MODE       0x21
#define ILI9163_CMD_SET_GAMMA_CURVE         0x26
#define ILI9163_CMD_SET_DISPLAY_OFF         0x28
#define ILI9163_CMD_SET_DISPLAY_ON          0x29
#define ILI9163_CMD_SET_COLUMN_ADDRESS      0x2A
#define ILI9163_CMD_SET_PAGE_ADDRESS        0x2B
#define ILI9163_CMD_WRITE_MEMORY_START      0x2C
#define ILI9163_CMD_WRITE_LUT               0x2D
#define ILI9163_CMD_READ_MEMORY_START       0x2E
#define ILI9163_CMD_SET_PARTIAL_AREA        0x30
#define ILI9163_CMD_SET_SCROLL_AREA         0x33
#define ILI9163_CMD_SET_TEAR_OFF            0x34
#define ILI9163_CMD_SET_TEAR_ON             0x35
#define ILI9163_CMD_SET_ADDRESS_MODE        0x36
#define ILI9163_CMD_SET_SCROLL_START        0X37
#define ILI9163_CMD_EXIT_IDLE_MODE          0x38
#define ILI9163_CMD_ENTER_IDLE_MODE         0x39
#define ILI9163_CMD_SET_PIXEL_FORMAT        0x3A
#define ILI9163_CMD_WRITE_MEMORY_CONTINUE   0x3C
#define ILI9163_CMD_READ_MEMORY_CONTINUE    0x3E
#define ILI9163_CMD_SET_TEAR_SCANLINE       0x44
#define ILI9163_CMD_GET_SCANLINE            0x45
#define ILI9163_CMD_READ_ID1                0xDA
#define ILI9163_CMD_READ_ID2                0xDB
#define ILI9163_CMD_READ_ID3                0xDC
#define ILI9163_CMD_FRAME_RATE_CONTROL1     0xB1
#define ILI9163_CMD_FRAME_RATE_CONTROL2     0xB2
#define ILI9163_CMD_FRAME_RATE_CONTROL3     0xB3
#define ILI9163_CMD_DISPLAY_INVERSION       0xB4
#define ILI9163_CMD_SOURCE_DRIVER_DIRECTION 0xB7
#define ILI9163_CMD_GATE_DRIVER_DIRECTION   0xB8
#define ILI9163_CMD_POWER_CONTROL1          0xC0
#define ILI9163_CMD_POWER_CONTROL2          0xC1
#define ILI9163_CMD_POWER_CONTROL3          0xC2
#define ILI9163_CMD_POWER_CONTROL4          0xC3
#define ILI9163_CMD_POWER_CONTROL5          0xC4
#define ILI9163_CMD_VCOM_CONTROL1           0xC5
#define ILI9163_CMD_VCOM_CONTROL2           0xC6
#define ILI9163_CMD_VCOM_OFFSET_CONTROL     0xC7
#define ILI9163_CMD_WRITE_ID4_VALUE         0xD3
#define ILI9163_CMD_NV_MEMORY_FUNCTION1     0xD7
#define ILI9163_CMD_NV_MEMORY_FUNCTION2     0xDE
#define ILI9163_CMD_POSITIVE_GAMMA_CORRECT  0xE0
#define ILI9163_CMD_NEGATIVE_GAMMA_CORRECT  0xE1
#define ILI9163_CMD_GAM_R_SEL               0xF2


#define PORT_ILI9163    PORTD
#define DDR_ILI9163     DDRD
#define DD_CS     DDB2
#define DD_AO    DDB3
#define DD_RESET       DDB4


// RGB24 to RGB16: Translates a 3 byte RGB value into a 2 byte value for the LCD (values should be 0-31)

inline uint16_t rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
	return ((b>>3) << 11) | ((g>>2) << 5) | (r>>3);
}

// This routine takes a row number from 0 to 20 and returns the x coordinate on the screen (0-127)
inline uint16_t textX(uint8_t x, int size) { return x * size * FONT_WIDTH; }

// This routine takes a column number from 0 to 20 and returns the y coordinate on the screen (0-127)
inline uint16_t textY(uint8_t y, int size) { return y * size * FONT_HEIGHT; }

#define CENTERX -1
#define CENTERY -1


#define	ROT0  0,	// portrait
#define	ROT90  96,	// landscape
#define	ROT180  160,	// flipped portrait
#define	ROT270  192	// flipped landscape

uint16_t width, height;

	// the A0 is D/C: 0 for command, 1 for data
/*
extern	void init(int rotation);
extern	void writeCommand(uint8_t address);
extern	void writeData(uint8_t data);
extern	void writeData16(uint16_t word);
extern	void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
extern	void reset(void);
extern	void drawPixel(uint8_t x, uint8_t y, uint8_t size, uint16_t colour);
extern	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour);
extern	void drawRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour);
extern	void drawRectFilled(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour);
extern	void drawCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour);
extern	void drawClear(uint16_t colour);
extern	void setRotation(int rotation);
extern	void drawChar( uint8_t x, uint8_t y, char c, uint8_t size, uint16_t colour, uint16_t bg);
extern	void drawString(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg,  const char *string);
extern	void drawStringF(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg, char *szFormat, ...);
*/

// Initialise the display with the require screen orientation
void init(int rotation) {

	width = ILI9163_WIDTH;
	height = ILI9163_HEIGHT;
	
	DDR_ILI9163 |= ((1<<DD_CS)|(1<<DD_RESET)|(1<<DD_AO));
    
	PORT_ILI9163|= (1<<DD_CS);
    PORT_ILI9163|= (1<<DD_RESET);
   
    // Hardware reset the LCD
    reset();
    
    writeCommand(ILI9163_CMD_EXIT_SLEEP_MODE);
    _delay_ms(5); // Wait for the screen to wake up
    
    writeCommand(ILI9163_CMD_SET_PIXEL_FORMAT);
    writeData(0x05); // 16 bits per pixel
   
    writeCommand(ILI9163_CMD_SET_GAMMA_CURVE);
    writeData(0x04); // Select gamma curve 3
    
    writeCommand(ILI9163_CMD_GAM_R_SEL);
    writeData(0x01); // Gamma adjustment enabled
    
    writeCommand(ILI9163_CMD_POSITIVE_GAMMA_CORRECT);
    writeData(0x3f); // 1st Parameter
    writeData(0x25); // 2nd Parameter
    writeData(0x1c); // 3rd Parameter
    writeData(0x1e); // 4th Parameter
    writeData(0x20); // 5th Parameter
    writeData(0x12); // 6th Parameter
    writeData(0x2a); // 7th Parameter
    writeData(0x90); // 8th Parameter
    writeData(0x24); // 9th Parameter
    writeData(0x11); // 10th Parameter
    writeData(0x00); // 11th Parameter
    writeData(0x00); // 12th Parameter
    writeData(0x00); // 13th Parameter
    writeData(0x00); // 14th Parameter
    writeData(0x00); // 15th Parameter
     
    writeCommand(ILI9163_CMD_NEGATIVE_GAMMA_CORRECT);
    writeData(0x20); // 1st Parameter
    writeData(0x20); // 2nd Parameter
    writeData(0x20); // 3rd Parameter
    writeData(0x20); // 4th Parameter
    writeData(0x05); // 5th Parameter
    writeData(0x00); // 6th Parameter
    writeData(0x15); // 7th Parameter
    writeData(0xa7); // 8th Parameter
    writeData(0x3d); // 9th Parameter
    writeData(0x18); // 10th Parameter
    writeData(0x25); // 11th Parameter
    writeData(0x2a); // 12th Parameter
    writeData(0x2b); // 13th Parameter
    writeData(0x2b); // 14th Parameter
    writeData(0x3a); // 15th Parameter
    
    writeCommand(ILI9163_CMD_FRAME_RATE_CONTROL1);
    writeData(0x08); // DIVA = 8
    writeData(0x08); // VPA = 8
    
    writeCommand(ILI9163_CMD_DISPLAY_INVERSION);
    writeData(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)
   
    writeCommand(ILI9163_CMD_POWER_CONTROL1);
    writeData(0x0a); // VRH = 10:  GVDD = 4.30
    writeData(0x02); // VC = 2: VCI1 = 2.65
      
    writeCommand(ILI9163_CMD_POWER_CONTROL2);
    writeData(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    writeCommand(ILI9163_CMD_VCOM_CONTROL1);
    writeData(0x50); // VMH = 80: VCOMH voltage = 4.5
    writeData(0x5b); // VML = 91: VCOML voltage = -0.225
    
    writeCommand(ILI9163_CMD_VCOM_OFFSET_CONTROL);
    writeData(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML
    
    writeCommand(ILI9163_CMD_SET_COLUMN_ADDRESS);
    writeData(0x00); // XSH
    writeData(0x00); // XSL
    writeData(0x00); // XEH
    writeData(0x7f); // XEL (128 pixels x)
   
    writeCommand(ILI9163_CMD_SET_PAGE_ADDRESS);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0x7f); // 128 pixels y

    setRotation(rotation);

    // Set the display to on
    writeCommand(ILI9163_CMD_SET_DISPLAY_ON);
    writeCommand(ILI9163_CMD_WRITE_MEMORY_START);
}

void writeCommand(uint8_t address) {

    PORT_ILI9163 &= ~(1<<DD_CS);
    // set D/C pin for command
	
   PORT_ILI9163 &= ~(1<<DD_AO);
    // send data by SPI
    
	spi_fast_shift(address);
    PORT_ILI9163|= (1<<DD_CS);
}

void writeData(uint8_t data) 
{
    PORT_ILI9163 &= ~(1<<DD_CS);
    // set D/C pin for data
  
   PORT_ILI9163 &= ~(1<<DD_AO);
    // send data by SPI
    
	spi_fast_shift(data);
   PORT_ILI9163|= (1<<DD_CS);
}

void writeData16(uint16_t word)
 {
	PORT_ILI9163 &= ~(1<<DD_CS);
	// set D/C pin for data
	
	PORT_ILI9163|= (1<<DD_AO);
	
	// send data by SPI
	spi_fast_shift((word >> 8) & 0x00FF);
	spi_fast_shift(word & 0x00FF);
	PORT_ILI9163|= (1<<DD_CS);
}

//set coordinate for print or other function
void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {
	writeCommand(ILI9163_CMD_SET_COLUMN_ADDRESS);
	writeData16(x1);
	writeData16(x2);

	writeCommand(ILI9163_CMD_SET_PAGE_ADDRESS);
	writeData16(y1);
	writeData16(y2);
	// memory write
	writeCommand(ILI9163_CMD_WRITE_MEMORY_START);
}

// Reset the LCD hardware
void reset(void)
 {

    // Reset pin is active low (0 = reset, 1 = ready)
	PORT_ILI9163 &= ~(1<<DD_RESET);
    _delay_ms(50);

    PORT_ILI9163|= (1<<DD_RESET);
    _delay_ms(120);
}

void drawPixel(uint8_t x, uint8_t y, uint8_t size, uint16_t colour)
 {
	if (size == 1)// default size or big size
	{
		if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) return;
	setAddress(x, y, x + 1, y + 1);
	writeData16(colour);
	}
		
	else
		drawRectFilled(x, y, size, size, colour);
}

// Draw a line
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour) {
    int16_t dy = y1 - y0;
    int16_t dx = x1 - x0;
    int16_t stepx, stepy;

    if (dy < 0) {
        dy = -dy; stepy = -1; 
    }
    else stepy = 1; 

    if (dx < 0) {
        dx = -dx; stepx = -1; 
    }
    else stepx = 1; 

    dy <<= 1;                           // dy is now 2*dy
    dx <<= 1;                           // dx is now 2*dx
 
    drawPixel(x0, y0,1,colour);

    if (dx > dy) {
        int fraction = dy - (dx >> 1);  // same as 2*dy - dx
        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepy;
                fraction -= dx;         // same as fraction -= 2*dx
            }

            x0 += stepx;
            fraction += dy;                 // same as fraction -= 2*dy
            drawPixel(x0, y0,1, colour);
        }
    } else {
        int fraction = dx - (dy >> 1);
        while (y0 != y1) {
            if (fraction >= 0) {
                x0 += stepx;
                fraction -= dy;
            }

            y0 += stepy;
            fraction += dx;
            drawPixel(x0, y0,1,colour);
        }
    }
}

// Draw a rectangle between x0, y0 and x1, y1
void drawRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour) {
	// two horizontal lines
	drawLine(x, y, x + w, y, colour);
	drawLine(x, y + h, x + w, y + h, colour);
	// two vertical lines
	drawLine(x, y, x, y + h, colour);
	drawLine(x + w, y, x + w, y + h, colour);
}

//draw colour filled rectangle
void drawRectFilled(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour) {
	if ((x >= width) || (y >= height)) return;
	if ((x + w - 1) >= width) w = width - x;
	if ((y + h - 1) >= height) h = height - y;
	setAddress(x, y, x + w - 1, y + h - 1);
	for (y = h; y > 0; y--)
		for (x = w; x > 0; x--)
			writeData16(colour);
}

// Draw a circle
void drawCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour) {
    int16_t x = 0, y = radius;
    int16_t d = 3 - (2 * radius);
 
    while(x <= y) {
        drawPixel(xCentre + x, yCentre + y,1, colour);
        drawPixel(xCentre + y, yCentre + x,1, colour);
        drawPixel(xCentre - x, yCentre + y,1, colour);
        drawPixel(xCentre + y, yCentre - x,1, colour);
        drawPixel(xCentre - x, yCentre - y,1, colour);
        drawPixel(xCentre - y, yCentre - x,1, colour);
        drawPixel(xCentre + x, yCentre - y,1, colour);
        drawPixel(xCentre - y, yCentre + x,1, colour);

        if (d < 0)
        	d += (4 * x) + 6;
        else {
            d += (4 * (x - y)) + 10;
            y -= 1;
        }

        x++;
    }
}

//rotate screen at desired orientation
void setRotation(int rotation) {
	writeCommand(ILI9163_CMD_SET_ADDRESS_MODE);
	writeData(rotation);
}

// LCD graphics functions -----------------------------------------------------------------------------------

void drawClear(uint16_t colour) {
	drawRectFilled(0,0,width, height, colour);
}


// LCD text manipulation functions --------------------------------------------------------------------------
void drawChar( uint8_t x, uint8_t y, char c, uint8_t size, uint16_t colour, uint16_t bg) {
	
	for (int8_t i=0; i < FONT_WIDTH; i++ ) {
		uint8_t line = (i == FONT_WIDTH-1)? 0 : pgm_read_byte(font5x8 + (c * (FONT_WIDTH - 1)) + i);
		for (int8_t j = 0; j < FONT_HEIGHT; j++) {
			if (line & 0x1) {
				if (colour != TRANSPARENT) drawPixel(x + i*size, y + j*size, size, colour);
			}
			else {
				if (bg != TRANSPARENT) drawPixel(x + i*size, y + j*size, size, bg);
			}
			line >>= 1;
		}
	}
}

// Plot a string of characters to the LCD: with the current font the screen allows up to 21x16 characters
void drawString(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg,  const char *string) {
	if (x == CENTERX) {
		x = (width - size * FONT_WIDTH * strlen(string)) / 2;
	}
	if (y == CENTERY) {
		y = (height - size * FONT_HEIGHT) / 2;
	}
    unsigned char c;
    while ( (c = *string++) ) {
		if (c == '\r') continue;
		if (c != '\n') drawChar(x, y, c, size, colour, bg);
		// new line check
        if(c == '\n' || x > width - 2 * FONT_WIDTH * size) {
        	x = 0;
			y += size * FONT_HEIGHT;
        } else {
        	x += size * FONT_WIDTH;
        }
    }
}

void drawStringF(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg, char *szFormat, ...) {
    char szBuffer[256]; //in this buffer we form the message
    int NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    int LASTCHAR = NUMCHARS - 1;
    va_list pArgs;
    va_start(pArgs, szFormat);
    vsnprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    va_end(pArgs);
    drawString(x, y, size, colour, bg, szBuffer);
}


