#ifndef _ESP8266_AP_H_
#define _ESP8266_AP_H_

#include "uart.h"
#include <stdbool.h>

#define BUFFER_SIZE 512

char buffer[BUFFER_SIZE];

#define SSID "default"
#define PASS ""

#define WIFI_MODE_STATION			0x01
#define WIFI_MODE_AP				0x02
#define WIFI_MODE_BOTH				0x03

#define WIFI_MUX_SINGLE				0x00
#define WIFI_MUX_MULTI				0x01

#define WIFI_TCP_DISABLE			0x00
#define WIFI_TCP_ENABLE				0x01

#define WIFI_CLIENT					0x00
#define WIFI_SERVER					0x01

#define WIFI_STATE_IDLE				0x00
#define WIFI_STATE_UNAVAILABLE		0x01
#define WIFI_STATE_SEND				0x02
#define WIFI_STATE_CONNECT			0x03

#define WIFI_NEW_NONE				0x00
#define WIFI_NEW_MESSAGE			0x01
#define WIFI_NEW_CONNECTED			0x02
#define WIFI_NEW_DISCONNECTED		0x03
#define WIFI_NEW_SEND_OK			0x04
#define WIFI_NEW_SEND_ERROR			0x05
#define WIFI_NEW_RESET				0x06
#define WIFI_NEW_ALREADY_CONNECT	0x07
#define WIFI_NEW_ETC				0x08

extern void connectTCP(char * ip, int port,int id);
extern void wait_for_OK();
extern bool getMessage();
extern void connectAccessPoint(char * ssid, char * pass,char *ip);
extern void openAccessPoint(char * ssid, char * pass, int channel);
extern int isNewDataComing();
extern bool send(int id, char * message,int len);
extern void waitingForJoin(char * ip);
extern void waitingForTCPConnection();
extern bool setMode(int mode);
extern bool setMux(int mux);
extern bool enableTCPServer(int port);
extern bool setAP(char * ssid, char * pass, int channel);
extern bool isNewAPSetting(char * ssid,char * pass, int channel);

bool TCPConnected;
bool TCPEnable;
bool isDebug;
int pinReset;	
int clientId;
char * clientMessage;
	
int runningState;

void connectAccessPoint(char * ssid, char * pass,char *ip)
{
	setMode(WIFI_MODE_STATION);
	
	USART_putstring("AT+CWJAP=\"");
	USART_putstring(SSID);
	USART_putstring("\",\"");
	USART_putstring(PASS);
	USART_putstring("\"\n");
	
	_delay_ms(2000);
	
	wait_for_OK();
	
	wait_for_OK();
	
	waitingForJoin(ip);
}

 void waitingForJoin(char * ip) 

{
	char * buffer;
	char i;
	
	USART_putstring("AT+CIFSR\n");
			
	while(buffer[i] == '\n')
	{
		buffer[i] = USART_receive();
			i++;	
	}			
	
	wait_for_OK();

}

bool setMode(int mode) 
{
	USART_putstring("AT+CWMODE=");
	USART_putnumber(mode);
	USART_putstring("\n");
	
	_delay_ms(500);
	
	wait_for_OK();	
	return 1;
}

bool setAP(char * ssid, char * pass, int channel)
{
	USART_putstring("AT+CWSAP=\""); 
	USART_putstring(SSID);
	USART_putstring("\",\"");
	USART_putstring(PASS);
	USART_putstring("\",\"");
	USART_putnumber(channel);
	USART_putstring(",4\n");
	
	_delay_ms(4000);
	wait_for_OK();
	return 1;
}

bool isNewAPSetting(char * ssid, char * pass, int channel)
 {
	char i;
	USART_putstring("AT+CWSAP?");
	
	i = USART_receive();
	while(i != '"');
	
	i = 0;
	
	while(ssid[i] != '"')
	{
		ssid[i] = USART_receive();
		i++;
	}
	
	i = USART_receive();
	while(i != '"');
	i = 0 ;
	
	while (pass[i] != '"')
	{
		pass[i] = USART_receive();
		i++;
	}
	
	wait_for_OK();
	return 1;
}

bool setMux(int mux)
 {
	USART_putstring("AT+CIPMUX=");
	USART_putnumber(mux);
	USART_putstring("\n");
	
	wait_for_OK();
	return 1;
}

bool enableTCPServer(int port)
 {
	USART_putstring("AT+CIPSERVER=1,");
	USART_putnumber(port);
	
	wait_for_OK();
	return 1; 
}

void openAccessPoint(char * ssid,char * pass, int channel)
 {
	if(!isNewAPSetting(ssid, pass, channel))
		setAP(ssid, pass, channel);
		
	setMode(WIFI_MODE_AP);
}

int isNewDataComing() 
{
	static int i=0;
	while(1)
	{
    buffer[i++] = USART_receive();
    if(i==BUFFER_SIZE)  
	i=0;
    if(i>1 && buffer[i-2]==13 && buffer[i-1]==10)
	 {
      buffer[i]='\0';
      i=0;
      return 1;
    }
  }
  return 0;
}

bool send(int id, char * message,int length)
 {
	 char i;
	 
	if(1)//getRunningState() == WIFI_STATE_IDLE) 
	{
		USART_putstring("AT+CIPSEND=");
		USART_putnumber(id);
		USART_putstring(",");
		USART_putnumber(length);
		USART_putstring("\n");
		//setRunningState(WIFI_STATE_SEND);
		
		i = USART_receive();
		while(i != '>' );
		
		USART_putstring(message);		
		
		_delay_ms(1000);
		
		wait_for_OK();
		return 1;
	}
	return 0 ;
	
}
void connectTCP(char * ip, int port,int id) 
{
	USART_putstring("AT+CIPSTART=");
	USART_putnumber(id);
	
	USART_putstring("\"TCP\"");
	USART_putstring(",\"");
	
	USART_putstring(ip);
	
	USART_putstring("\",");
	
	USART_putnumber(port);
	
	wait_for_OK();
	
	//setRunningState(WIFI_STATE_CONNECT);
}

bool getMessage(char * msg)
{
	int ch_id, packet_len;
	char *pb; 
	char i = 0;
		
	 if(strncmp(buffer, "+IPD,", 5)==0) 
	{
      // request: +IPD,ch,len:data
      sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
	  
      if (packet_len > 0)
	   {
        // read serial until packet_len character received
        // start from :
        pb = buffer+5;
        while(*pb!=':') 
		pb++;
        
		while(i < packet_len)
		{
			msg = USART_receive();
			i++;
		}
		msg[i] = '\0';
		return 1;
	   }
	   else
	   return 0;		
}

void wait_for_OK()
{
	char c;	
      while(c != 'O')
	  c = USART_receive();

	  while(c != 'K')
	  c = USART_receive();
}
#endif