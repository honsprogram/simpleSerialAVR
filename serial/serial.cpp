/*
 * serial.cpp
 *
 * Created: 2018-01-27 09:40:19
 *  Author: HZ
 * Serial simple Tx only
 */ 
#define F_CPU 16000000
#define BAUD 9600
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include "uartHZ.c"
#include "uartHZ.h"
#include <avr/interrupt.h>
#include <string.h>
bool volatile serial_flag=1;
char ReceivedByte=0;


// This flag is set on USART Receiver buffer overflow
unsigned char rx_buffer_overflow[2]={0},rx_packet_ready[2]={0};
unsigned char rx_wr_index[2]={0},rx_rd_index[2]={0},rx_counter[2]={0};
unsigned char recev_state[2]={0};
unsigned char Packet_error[2]={0};
#define FRAMING_ERROR0 			(1<<FE0)
#define DATA_OVERRUN0 			(1<<DOR0)
#define DATA_REGISTER_EMPTY0 	(1<<UDRE0)
#define TX_COMPLETE0 	 		(1<<TXC0)
// USART Receiver buffer
#define UART0_RX_BUFFER_SIZE 300

	typedef struct {
		//	union{
		unsigned char rx[UART0_RX_BUFFER_SIZE];
		
		//	};
	}__attribute((packed)) BUFFERTYPE0;

	BUFFERTYPE0 bufferUART0;

 ISR (USART0_RX_vect)
 {
	 unsigned char status,data;
	
	 status=UCSR0A;
	 data=UDR0;
    //UDR0=data;
	 ReceivedByte=data;
	if ((status & (FRAMING_ERROR0 | DATA_OVERRUN0 ))==0)
	{
		bufferUART0.rx[rx_wr_index[0]]=data;
		if (++rx_wr_index[0] == UART0_RX_BUFFER_SIZE) rx_wr_index[0]=0;
		if (++rx_counter[0] == UART0_RX_BUFFER_SIZE)
		{
			rx_counter[0]=0;
			rx_buffer_overflow[0]=1;
		}
	}
	PORTC=0x00;
serial_flag=0;
 }

void check_receive(unsigned char returnData[2]){

	cli();
	if(rx_counter[0]){
		rx_counter[0]--;
		returnData[0]=1;
		returnData[1] = bufferUART0.rx[rx_rd_index[0]];
		if (++rx_rd_index[0] == UART0_RX_BUFFER_SIZE) rx_rd_index[0]=0;
		}else{
		returnData[0]=0;
	}
	sei();
}
 void uart_init(void) {
	 UCSR0B =  (1 << RXEN0) | (1 << TXEN0) | (1<<RXCIE0);
	 UCSR0C = (1<<UCSZ00)|(1 << UCSZ01);
	 UBRR0H=0x00;
	 UBRR0L=0x67;
 }
char *word = "*";
int main(void)
{
	
	int i=0;
	PORTC |= 0X80; 
	DDRC= 0xFF;
	/* run uart */
	sei();
	uart_init();
	uart_str = fdevopen(uart_putchar, uart_getchar); //send , receive functions
	stdout = uart_str;
   

	printf("serial\n");
    while(1)
    {
    
	//	printf("serial\n");
	if(!serial_flag) {
		printf("serial= %c \n",ReceivedByte);
		printf("sret= %s \n",bufferUART0.rx);
		serial_flag=1;
		PORTC=0x80;
	}
	//PORTC=~PORTC;
	_delay_ms(500);
    }
}