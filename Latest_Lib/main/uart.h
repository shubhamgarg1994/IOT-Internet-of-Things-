#include <stdbool.h>
 
//Declaration of our functions
/*
void USART_init(int);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
void USART_putnumber(int number); 
bool USART_receive_timeout(uint8_t timeout,uint8_t data);
 */

void USART_init(int baudrate){
 
 UBRR0H = (uint8_t)(baudrate>>8);
 UBRR0L = (uint8_t)(baudrate);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);
}
 
unsigned char USART_receive(void)
{ 
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
}

bool USART_receive_timeout(uint8_t timeout,uint8_t data)
{
	uint8_t counter = 0;
	while ((timeout != counter))
	{
	if(!(UCSR0A & (1<<RXC0)))
		{
			_delay_ms(1);
			counter++;
		}	
		else
		{
		data = UDR0;
		return 1;
		}				
	}		
		return 0;
} 
 
void USART_send( unsigned char data){
 
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
 
}
 
void USART_putstring(char* StringPtr)
{ 
	while(*StringPtr != '\0')
	{
	 USART_send(*StringPtr);
	 StringPtr++;
	 }
}

void USART_putnumber(int number)
{
	char buf[10];
	itoa(number,buf,10);
	USART_putstring(buf);	
}