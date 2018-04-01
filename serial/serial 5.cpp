/*
 * serial.cpp
 *
 * Created: 2018-01-27 09:40:19
 *  Author: Admin
 */ 
#define F_CPU 16000000
#define BAUD 9600
#include <avr/pgmspace.h>
#include <math.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/wdt.h> // watchdog header file
#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <avr/io.h>
#include <stdlib.h>
#include "uartHZ.c"
#include "uartHZ.h"
#include <avr/interrupt.h>
#include <string.h>



volatile unsigned char ReceivedByte; 
volatile bool serial_flag=1;
 typedef struct _msg_update_data {
	 float temperature;
	 float set_point;
	 unsigned char mmode;
	 unsigned char fan;
	 unsigned char pump;
	 unsigned char scenario;
	 unsigned char timer_hours;
	 unsigned char timer_minutes;
	 unsigned char rccb;
 } msg_update_data;
  typedef struct _datat {
	  char temperature;
	  char set_point;
  } datat;
#include <avr/io.h>
  ISR (USART0_RXC_vect) {
	ReceivedByte = UDR0 ; // Fetch the received byte value into the variable " ByteReceived "
	serial_flag=0;
 }
 datat* new_data;
 void USART_SendByte(uint8_t u8Data){

	 // Wait until last byte has been transmitted
	 while((UCSR0A &(1<<UDRE0)) == 0);

	 // Transmit data
	 UDR0 = u8Data;
 }

char *word = "*";
int main(void)
{
	int i,k;

	DDRB= 0x40;
	PORTC=0x00;
	DDRC= 0xFF;
	DDRG&= 0b11110111;
	
	uart_init();
	uart_str = fdevopen(uart_putchar, uart_getchar); //send , receive functions
	stdout = uart_str;
	
	printf("You wrote $$$$$ \n");
	 //SART_SendByte(1); 
	 	sei(); 
		 ReceivedByte='A';
		 
    while(1)
    {
//	printf("hi\n");
        //TODO:: Please write your application code 
		//PORTC |= 0x80;
		
		//PORTB &= 0xBF;
		//PORTC &= 0x7F;
		_delay_ms(1000);
		//****************************
		//while (1)
		//{
			//char strtest[7]="*1234#";
				//_delay_ms(2000);
				//new_data->set_point=10;
				//new_data->temperature=20;
				////printf("strtest %s",strtest);
				//for(k=0;k<=7;k++) printf("%c",strtest[k]);
				//printf("here");
		//}
		// USART_SendByte(1); 
		if((PING & 0b00001000) ==0) {serial_flag=0;}
		else {serial_flag=1;}
		//printf("%d\n",serial_flag);
		//printf("%d",14);
		char str[250],c;
		if((PING & 0b00001000) ==0){
			printf("%c",14);
			c= 0;	
			i=0;
			str[0]='S';
			while(str[i]!=35){//#
				i++;
				 str[i]= getchar();
				
			//printf("i%d",i);
			//printf("%c",str[i]);
			}
			str[i]='\0';
			
			//str[0]='a';str[1]='b';str[2]='c';str[3]='\0';
			//char label[10] = "Single";
			//printf("h %s",label);
			 //std::string str ("There are two needles in  needles.");
			 //std::string str2 ("needle");
			  // different member versions of find in the same order as above:
           // sprintf(str,"#*2345");
			printf("h %s",str);
			new_data = (datat*) str;
			printf("here2");
			//************************** Find the * 
			char * pch=strchr(str, '*');
			char  formatint[5],formatfloat[5];
			if(pch != NULL) {
				 int pos=(int)(pch-str+1);
				 printf ("found at %d\n",pos);
				 //printf("start");
				 //for(i=pos;i<=pos+14;i++) 	printf("%c",str[i]);
				 //printf("end");
				 memcpy( formatint, &str[pos+2], 2 );
				 printf ("tempint %2d\n",atoi (formatint));
				 memcpy( formatint, &str[pos+5], 2 );
				 printf ("tempf %d\n",atoi (formatint));
				 memcpy( formatint, &str[pos+9], 2 );
				 printf ("spint %2d\n",atoi (formatint));
				 memcpy( formatint, &str[pos+12], 2 );
				 printf ("spf %d\n",atoi (formatint));
				 
				// sprintf(format,"%c%c%c%c",str[pos+2],str[pos+3],str[pos+5],str[pos+6]);
				// memcpy( formatint, &str[pos+2], 1 );
				 //memcpy( formatfloat, &str[pos+5], 1 );
	
				 //memcpy( formatint, &str[pos+4], 1 );
				 //memcpy( formatfloat, &str[pos+6], 1 );
				 //printf ("tempint %s\n",formatint);
				 //printf ("tempf %s\n",formatfloat);
				 //int temp=atoi (formatint);
				 //printf ("tempintok %d \n",temp);
			}
			
			//for(k=0;k<=10;k++) printf("%c",label[k]);
			for(i=0;i<=250;i++) {//sizeof(str)
			//	printf("%c",str[i]);
				str[i]='\0';
				};
			//_delay_ms(1000);
			 }
		//while(!(PING & 0b00001000) ==0){scanf ("%s",str); }
		//printf("%d\n",(PING & 0b00001000) );
		
		//_delay_ms(1000);
		if(!serial_flag){
			//printf("pin %d \n",11);  
			//printf("%c",14);  
			//scanf ("%s",str);
			//gets(str);
			//printf("%s ",str);
		}
    }
}