/*
lcdpcf8574 lib 0x01

copyright (c) Shubham Garg, 2015
*/


#ifndef LCD_H
#define LCD_H
#define F_CPU 16000000
#include <inttypes.h>
#include <avr/pgmspace.h>
#include "pcf8574.h"

#define LCD_PCF8574_INIT 1 //init pcf8574

#define LCD_PCF8574_DEVICEID 7 //device id, addr = pcf8574 base addr + LCD_PCF8574_DEVICEID


/** 
 *  @name  Definitions for Display Size 
 *  Change these definitions to adapt setting to your display
 */
#define LCD_LINES           2     /**< number of visible lines of the display */
#define LCD_DISP_LENGTH    16     /**< visibles characters per line of the display */
#define LCD_LINE_LENGTH  0x40     /**< internal line length of the display    */
#define LCD_START_LINE1  0x00     /**< DDRAM address of first char of line 1 */
#define LCD_START_LINE2  0x40     /**< DDRAM address of first char of line 2 */
#define LCD_START_LINE3  0x14     /**< DDRAM address of first char of line 3 */
#define LCD_START_LINE4  0x54     /**< DDRAM address of first char of line 4 */
#define LCD_WRAP_LINES      1     /**< 0: no wrap, 1: wrap at end of visibile line */



#define LCD_DATA0_PIN    4            /**< pin for 4bit data bit 0  */
#define LCD_DATA1_PIN    5            /**< pin for 4bit data bit 1  */
#define LCD_DATA2_PIN    6            /**< pin for 4bit data bit 2  */
#define LCD_DATA3_PIN    7            /**< pin for 4bit data bit 3  */
#define LCD_RS_PIN       0            /**< pin  for RS line         */
#define LCD_RW_PIN       1            /**< pin  for RW line         */
#define LCD_E_PIN        2            /**< pin  for Enable line     */
#define LCD_LED_PIN      3            /**< pin  for Led             */


/**
 *  @name Definitions for LCD command instructions
 *  The constants define the various LCD controller instructions which can be passed to the 
 *  function lcd_command(), see HD44780 data sheet for a complete description.
 */

/* instruction register bit positions, see HD44780U data sheet */
#define LCD_CLR               0      /* DB0: clear display                  */
#define LCD_HOME              1      /* DB1: return to home position        */
#define LCD_ENTRY_MODE        2      /* DB2: set entry mode                 */
#define LCD_ENTRY_INC         1      /*   DB1: 1=increment, 0=decrement     */
#define LCD_ENTRY_SHIFT       0      /*   DB2: 1=display shift on           */
#define LCD_ON                3      /* DB3: turn lcd/cursor on             */
#define LCD_ON_DISPLAY        2      /*   DB2: turn display on              */
#define LCD_ON_CURSOR         1      /*   DB1: turn cursor on               */
#define LCD_ON_BLINK          0      /*     DB0: blinking cursor ?          */
#define LCD_MOVE              4      /* DB4: move cursor/display            */
#define LCD_MOVE_DISP         3      /*   DB3: move display (0-> cursor) ?  */
#define LCD_MOVE_RIGHT        2      /*   DB2: move right (0-> left) ?      */
#define LCD_FUNCTION          5      /* DB5: function set                   */
#define LCD_FUNCTION_8BIT     4      /*   DB4: set 8BIT mode (0->4BIT mode) */
#define LCD_FUNCTION_2LINES   3      /*   DB3: two lines (0->one line)      */
#define LCD_FUNCTION_10DOTS   2      /*   DB2: 5x10 font (0->5x7 font)      */
#define LCD_CGRAM             6      /* DB6: set CG RAM address             */
#define LCD_DDRAM             7      /* DB7: set DD RAM address             */
#define LCD_BUSY              7      /* DB7: LCD is busy                    */

/* set entry mode: display shift on/off, dec/inc cursor move direction */
#define LCD_ENTRY_DEC            0x04   /* display shift off, dec cursor move dir */
#define LCD_ENTRY_DEC_SHIFT      0x05   /* display shift on,  dec cursor move dir */
#define LCD_ENTRY_INC_           0x06   /* display shift off, inc cursor move dir */
#define LCD_ENTRY_INC_SHIFT      0x07   /* display shift on,  inc cursor move dir */

/* display on/off, cursor on/off, blinking char at cursor position */
#define LCD_DISP_OFF             0x08   /* display off                            */
#define LCD_DISP_ON              0x0C   /* display on, cursor off                 */
#define LCD_DISP_ON_BLINK        0x0D   /* display on, cursor off, blink char     */
#define LCD_DISP_ON_CURSOR       0x0E   /* display on, cursor on                  */
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   /* display on, cursor on, blink char      */

/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     0x10   /* move cursor left  (decrement)          */
#define LCD_MOVE_CURSOR_RIGHT    0x14   /* move cursor right (increment)          */
#define LCD_MOVE_DISP_LEFT       0x18   /* shift display left                     */
#define LCD_MOVE_DISP_RIGHT      0x1C   /* shift display right                    */

/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  0x20   /* 4-bit interface, single line, 5x7 dots */
#define LCD_FUNCTION_4BIT_2LINES 0x28   /* 4-bit interface, dual line,   5x7 dots */
#define LCD_FUNCTION_8BIT_1LINE  0x30   /* 8-bit interface, single line, 5x7 dots */
#define LCD_FUNCTION_8BIT_2LINES 0x38   /* 8-bit interface, dual line,   5x7 dots */


#define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )



/** 
 *  @name Functions
 */


/**
 @brief    Initialize display and select type of cursor
 @param    dispAttr \b LCD_DISP_OFF display off\n
                    \b LCD_DISP_ON display on, cursor off\n
                    \b LCD_DISP_ON_CURSOR display on, cursor on\n
                    \b LCD_DISP_ON_CURSOR_BLINK display on, cursor on flashing             
 @return  none
*/
void lcd_init(uint8_t dispAttr);


/**
 @brief    Clear display and set cursor to home position
 @param    void                                        
 @return   none
*/
extern void lcd_clrscr(void);


/**
 @brief    Set cursor to home position
 @param    void                                        
 @return   none
*/
extern void lcd_home(void);


/**
 @brief    Set cursor to specified position
 
 @param    x horizontal position\n (0: left most position)
 @param    y vertical position\n   (0: first line)
 @return   none
*/
extern void lcd_gotoxy(uint8_t x, uint8_t y);


/**
 @brief    Set illumination pin
 @param    void
 @return   none
*/
extern void lcd_led(uint8_t onoff);


/**
 @brief    Display character at current cursor position
 @param    c character to be displayed                                       
 @return   none
*/
extern void lcd_putc(char c);


/**
 @brief    Display string without auto linefeed
 @param    s string to be displayed                                        
 @return   none
*/
extern void lcd_puts(const char *s);


/**
 @brief    Display string from program memory without auto linefeed
 @param    s string from program memory be be displayed                                        
 @return   none
 @see      lcd_puts_P
*/
extern void lcd_puts_p(const char *progmem_s);


/**
 @brief    Send LCD controller instruction command
 @param    cmd instruction to send to LCD controller, see HD44780 data sheet
 @return   none
*/
extern void lcd_command(uint8_t cmd);


/**
 @brief    Send data byte to LCD controller 
 
 Similar to lcd_putc(), but without interpreting LF
 @param    data byte to send to LCD controller, see HD44780 data sheet
 @return   none
*/
extern void lcd_data(uint8_t data);


/**
 @brief macros for automatically storing string constant in program memory
*/
#define lcd_puts_P(__s)         lcd_puts_p(PSTR(__s))


#define lcd_e_delay()   __asm__ __volatile__( "rjmp 1f\n 1:" );
#define lcd_e_toggle()  toggle_e()

#if LCD_LINES==1
#define LCD_FUNCTION_DEFAULT    LCD_FUNCTION_4BIT_1LINE 
#else
#define LCD_FUNCTION_DEFAULT    LCD_FUNCTION_4BIT_2LINES 
#endif

volatile uint8_t dataport = 0;

/* 
** function prototypes 
*/
static void toggle_e(void);

/*
** local functions
*


/*************************************************************************
 delay loop for small accurate delays: 16-bit counter, 4 cycles/loop
*************************************************************************/
static inline void _delayFourCycles(unsigned int __count)
{
    if ( __count == 0 )    
        __asm__ __volatile__( "rjmp 1f\n 1:" );    // 2 cycles
    else
        __asm__ __volatile__ (
    	    "1: sbiw %0,1" "\n\t"                  
    	    "brne 1b"                              // 4 cycles/loop
    	    : "=w" (__count)
    	    : "0" (__count)
    	   );
}


/************************************************************************* 
delay for a minimum of <us> microseconds
the number of loops is calculated at compile-time from MCU clock frequency
*************************************************************************/
#define delay(us)  _delayFourCycles( ( ( 1*(F_CPU/4000) )*us)/1000 )


/* toggle Enable Pin to initiate write */
static void toggle_e(void)
{
	pcf8574_setoutputpinhigh(LCD_PCF8574_DEVICEID, LCD_E_PIN);
    lcd_e_delay();
    pcf8574_setoutputpinlow(LCD_PCF8574_DEVICEID, LCD_E_PIN);
}


/*************************************************************************
Low-level function to write byte to LCD controller
Input:    data   byte to write to LCD
          rs     1: write data    
                 0: write instruction
Returns:  none
*************************************************************************/
static void lcd_write(uint8_t data,uint8_t rs) 
{
	if (rs) /* write data        (RS=1, RW=0) */
		dataport |= _BV(LCD_RS_PIN);
	else /* write instruction (RS=0, RW=0) */
		dataport &= ~_BV(LCD_RS_PIN);
	dataport &= ~_BV(LCD_RW_PIN);
	pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

	/* output high nibble first */
    dataport &= ~_BV(LCD_DATA3_PIN);
    dataport &= ~_BV(LCD_DATA2_PIN);
    dataport &= ~_BV(LCD_DATA1_PIN);
    dataport &= ~_BV(LCD_DATA0_PIN);
	if(data & 0x80) dataport |= _BV(LCD_DATA3_PIN);
	if(data & 0x40) dataport |= _BV(LCD_DATA2_PIN);
	if(data & 0x20) dataport |= _BV(LCD_DATA1_PIN);
	if(data & 0x10) dataport |= _BV(LCD_DATA0_PIN);
	pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
	lcd_e_toggle();

	/* output low nibble */
	dataport &= ~_BV(LCD_DATA3_PIN);
	dataport &= ~_BV(LCD_DATA2_PIN);
	dataport &= ~_BV(LCD_DATA1_PIN);
	dataport &= ~_BV(LCD_DATA0_PIN);
	if(data & 0x08) dataport |= _BV(LCD_DATA3_PIN);
	if(data & 0x04) dataport |= _BV(LCD_DATA2_PIN);
	if(data & 0x02) dataport |= _BV(LCD_DATA1_PIN);
	if(data & 0x01) dataport |= _BV(LCD_DATA0_PIN);
	pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
	lcd_e_toggle();

	/* all data pins high (inactive) */
	dataport |= _BV(LCD_DATA0_PIN);
	dataport |= _BV(LCD_DATA1_PIN);
	dataport |= _BV(LCD_DATA2_PIN);
	dataport |= _BV(LCD_DATA3_PIN);
	pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
}


/*************************************************************************
Low-level function to read byte from LCD controller
Input:    rs     1: read data    
                 0: read busy flag / address counter
Returns:  byte read from LCD controller
*************************************************************************/
static uint8_t lcd_read(uint8_t rs) 
{
    uint8_t data;

    if (rs) /* write data        (RS=1, RW=0) */
    	dataport |= _BV(LCD_RS_PIN);
    else /* write instruction (RS=0, RW=0) */
    	dataport &= ~_BV(LCD_RS_PIN);
    dataport |= _BV(LCD_RW_PIN);
    pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

    pcf8574_setoutputpinhigh(LCD_PCF8574_DEVICEID, LCD_E_PIN);
	lcd_e_delay();
	data = pcf8574_getoutputpin(LCD_PCF8574_DEVICEID, LCD_DATA0_PIN) << 4;     /* read high nibble first */
	pcf8574_setoutputpinlow(LCD_PCF8574_DEVICEID, LCD_E_PIN);

	lcd_e_delay();                       /* Enable 500ns low       */

	pcf8574_setoutputpinhigh(LCD_PCF8574_DEVICEID, LCD_E_PIN);
	lcd_e_delay();
	data |= pcf8574_getoutputpin(LCD_PCF8574_DEVICEID, LCD_DATA0_PIN) &0x0F;    /* read low nibble        */
	pcf8574_setoutputpinlow(LCD_PCF8574_DEVICEID, LCD_E_PIN);

    return data;
}


/*************************************************************************
loops while lcd is busy, returns address counter
*************************************************************************/
static uint8_t lcd_waitbusy(void)

{
    register uint8_t c;
    
    /* wait until busy flag is cleared */
    while ( (c=lcd_read(0)) & (1<<LCD_BUSY)) {}
    
    /* the address counter is updated 4us after the busy flag is cleared */
    delay(2);

    /* now read the address counter */
    return (lcd_read(0));  // return address counter
    
}/* lcd_waitbusy */


/*************************************************************************
Move cursor to the start of next line or to the first line if the cursor 
is already on the last line.
*************************************************************************/
static inline void lcd_newline(uint8_t pos)
{
    register uint8_t addressCounter;


#if LCD_LINES==1
    addressCounter = 0;
#endif
#if LCD_LINES==2
    if ( pos < (LCD_START_LINE2) )
        addressCounter = LCD_START_LINE2;
    else
        addressCounter = LCD_START_LINE1;
#endif
#if LCD_LINES==4
    if ( pos < LCD_START_LINE3 )
        addressCounter = LCD_START_LINE2;
    else if ( (pos >= LCD_START_LINE2) && (pos < LCD_START_LINE4) )
        addressCounter = LCD_START_LINE3;
    else if ( (pos >= LCD_START_LINE3) && (pos < LCD_START_LINE2) )
        addressCounter = LCD_START_LINE4;
    else 
        addressCounter = LCD_START_LINE1;
#endif
    lcd_command((1<<LCD_DDRAM)+addressCounter);

}/* lcd_newline */


/*
** PUBLIC FUNCTIONS 
*/

/*************************************************************************
Send LCD controller instruction command
Input:   instruction to send to LCD controller, see HD44780 data sheet
Returns: none
*************************************************************************/
void lcd_command(uint8_t cmd)
{
    lcd_waitbusy();
    lcd_write(cmd,0);
}


/*************************************************************************
Send data byte to LCD controller 
Input:   data to send to LCD controller, see HD44780 data sheet
Returns: none
*************************************************************************/
void lcd_data(uint8_t data)
{
    lcd_waitbusy();
    lcd_write(data,1);
}



/*************************************************************************
Set cursor to specified position
Input:    x  horizontal position  (0: left most position)
          y  vertical position    (0: first line)
Returns:  none
*************************************************************************/
void lcd_gotoxy(uint8_t x, uint8_t y)
{
#if LCD_LINES==1
    lcd_command((1<<LCD_DDRAM)+LCD_START_LINE1+x);
#endif
#if LCD_LINES==2
    if ( y==0 ) 
        lcd_command((1<<LCD_DDRAM)+LCD_START_LINE1+x);
    else
        lcd_command((1<<LCD_DDRAM)+LCD_START_LINE2+x);
#endif
#if LCD_LINES==4
    if ( y==0 )
        lcd_command((1<<LCD_DDRAM)+LCD_START_LINE1+x);
    else if ( y==1)
        lcd_command((1<<LCD_DDRAM)+LCD_START_LINE2+x);
    else if ( y==2)
        lcd_command((1<<LCD_DDRAM)+LCD_START_LINE3+x);
    else /* y==3 */
        lcd_command((1<<LCD_DDRAM)+LCD_START_LINE4+x);
#endif

}/* lcd_gotoxy */


/*************************************************************************
*************************************************************************/
int lcd_getxy(void)
{
    return lcd_waitbusy();
}


/*************************************************************************
Clear display and set cursor to home position
*************************************************************************/
void lcd_clrscr(void)
{
    lcd_command(1<<LCD_CLR);
}


/*************************************************************************
Set illumination pin
*************************************************************************/
void lcd_led(uint8_t onoff)
{
	if(onoff)
		dataport &= ~_BV(LCD_LED_PIN);
	else
		dataport |= _BV(LCD_LED_PIN);
	pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
}


/*************************************************************************
Set cursor to home position
*************************************************************************/
void lcd_home(void)
{
    lcd_command(1<<LCD_HOME);
}


/*************************************************************************
Display character at current cursor position 
Input:    character to be displayed                                       
Returns:  none
*************************************************************************/
void lcd_putc(char c)
{
    uint8_t pos;

    pos = lcd_waitbusy();   // read busy-flag and address counter
    if (c=='\n')
    {
        lcd_newline(pos);
    }
    else
    {
#if LCD_WRAP_LINES==1
#if LCD_LINES==1
        if ( pos == LCD_START_LINE1+LCD_DISP_LENGTH ) {
            lcd_write((1<<LCD_DDRAM)+LCD_START_LINE1,0);
        }
#elif LCD_LINES==2
        if ( pos == LCD_START_LINE1+LCD_DISP_LENGTH ) {
            lcd_write((1<<LCD_DDRAM)+LCD_START_LINE2,0);    
        }else if ( pos == LCD_START_LINE2+LCD_DISP_LENGTH ){
            lcd_write((1<<LCD_DDRAM)+LCD_START_LINE1,0);
        }
#elif LCD_LINES==4
        if ( pos == LCD_START_LINE1+LCD_DISP_LENGTH ) {
            lcd_write((1<<LCD_DDRAM)+LCD_START_LINE2,0);    
        }else if ( pos == LCD_START_LINE2+LCD_DISP_LENGTH ) {
            lcd_write((1<<LCD_DDRAM)+LCD_START_LINE3,0);
        }else if ( pos == LCD_START_LINE3+LCD_DISP_LENGTH ) {
            lcd_write((1<<LCD_DDRAM)+LCD_START_LINE4,0);
        }else if ( pos == LCD_START_LINE4+LCD_DISP_LENGTH ) {
            lcd_write((1<<LCD_DDRAM)+LCD_START_LINE1,0);
        }
#endif
        lcd_waitbusy();
#endif
        lcd_write(c, 1);
    }

}/* lcd_putc */


/*************************************************************************
Display string without auto linefeed 
Input:    string to be displayed
Returns:  none
*************************************************************************/
void lcd_puts(const char *s)
/* print string on lcd (no auto linefeed) */
{
    register char c;

    while ( (c = *s++) ) {
        lcd_putc(c);
    }

}/* lcd_puts */


/*************************************************************************
Display string from program memory without auto linefeed 
Input:     string from program memory be be displayed                                        
Returns:   none
*************************************************************************/
void lcd_puts_p(const char *progmem_s)
/* print string from program memory on lcd (no auto linefeed) */
{
    register char c;

    while ( (c = pgm_read_byte(progmem_s++)) ) {
        lcd_putc(c);
    }

}/* lcd_puts_p */


/*************************************************************************
Initialize display and select type of cursor 
Input:    dispAttr LCD_DISP_OFF            display off
                   LCD_DISP_ON             display on, cursor off
                   LCD_DISP_ON_CURSOR      display on, cursor on
                   LCD_DISP_CURSOR_BLINK   display on, cursor on flashing
Returns:  none
*************************************************************************/
void lcd_init(uint8_t dispAttr)
{
	#if LCD_PCF8574_INIT == 1
	//init pcf8574
	pcf8574_init();
	#endif

	dataport = 0;
	pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

    delay(16000);        /* wait 16ms or more after power-on       */

    /* initial write to lcd is 8bit */
    dataport |= _BV(LCD_DATA1_PIN);  // _BV(LCD_FUNCTION)>>4;
    dataport |= _BV(LCD_DATA0_PIN);  // _BV(LCD_FUNCTION_8BIT)>>4;
    pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

    lcd_e_toggle();
    delay(4992);         /* delay, busy flag can't be checked here */
   
    /* repeat last command */ 
    lcd_e_toggle();      
    delay(64);           /* delay, busy flag can't be checked here */
    
    /* repeat last command a third time */
    lcd_e_toggle();      
    delay(64);           /* delay, busy flag can't be checked here */

    /* now configure for 4bit mode */
    dataport &= ~_BV(LCD_DATA0_PIN);
    pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
    lcd_e_toggle();
    delay(64);           /* some displays need this additional delay */
    
    /* from now the LCD only accepts 4 bit I/O, we can use lcd_command() */    

    lcd_command(LCD_FUNCTION_DEFAULT);      /* function set: display lines  */

    lcd_command(LCD_DISP_OFF);              /* display off                  */
    lcd_clrscr();                           /* display clear                */
    lcd_command(LCD_MODE_DEFAULT);          /* set entry mode               */
    lcd_command(dispAttr);                  /* display/cursor control       */

}/* lcd_init */

/*@}*/
#endif //LCD_H
