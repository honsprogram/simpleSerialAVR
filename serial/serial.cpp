/*
 * serial.cpp
 *
 * Created: 2018-01-27 09:40:19
 *  Author: Hossein Zamanian (hznewresearch@gmail.com) 
 * Serial simple RX by using interrupt only
 */ 
/*
Copyright (c) [2018] [Hossein Zamanian #% ]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#define F_CPU 16000000
#define BAUD 9600
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>
#include "uartHZ.c" 
#include "uartHZ.h" 
  FILE * uart_str;
bool volatile serial_flag=1;


// This flag is set on USART Receiver buffer overflow
unsigned char rx_buffer_overflow={0},rx_packet_ready={0};
unsigned char rx_wr_index={0},rx_rd_index={0},rx_counter={0};
unsigned char recev_state={0};
unsigned char Packet_error={0};
#define FRAMING_ERROR0 			(1<<FE0)
#define DATA_OVERRUN0 			(1<<DOR0)
#define DATA_REGISTER_EMPTY0 	(1<<UDRE0)
#define TX_COMPLETE0 	 		(1<<TXC0)
// USART Receiver buffer
#define UART0_RX_BUFFER_SIZE 300

	typedef struct {
		unsigned char rx[UART0_RX_BUFFER_SIZE];
	}__attribute((packed)) BUFFERTYPE0;
	
	BUFFERTYPE0 bufferUART0;
// Interrupt serial ISR 
 ISR (USART0_RX_vect)
 {
	 unsigned char status,data;
	 status=UCSR0A;
	 data=UDR0;
	 ReceivedByte=data;
	if ((status & (FRAMING_ERROR0 | DATA_OVERRUN0 ))==0)
	{
		if(data=='\r')
		{
			bufferUART0.rx[rx_wr_index++]='\0';
			rx_wr_index=0;
			serial_flag=0;
		}
		else
		{
			bufferUART0.rx[rx_wr_index]=data;
			if (++rx_wr_index == UART0_RX_BUFFER_SIZE) rx_wr_index=0;
			if (++rx_counter == UART0_RX_BUFFER_SIZE)
			{
				rx_counter=0;
				rx_buffer_overflow=1;
			}
		}



	}
	
 
 }
 // Serial lib
 class hzserial {
	public:
	 hzserial(void);
    uint8_t * read(void);
	bool available(void);
 };
 hzserial::hzserial() {
 }
 
bool hzserial::available(void){
	return serial_flag;	
}

uint8_t * hzserial::read(void){
		uint8_t * buff;
		cli();
		buff=bufferUART0.rx;
		//bufferUART0.rx[0]='\0';
		serial_flag=1;
		sei();
		return buff;
		bufferUART0.rx[0]='\0';
}

hzserial Serial = hzserial();

 void uart_init(void) {
	 UCSR0B =  (1 << RXEN0) | (1 << TXEN0) | (1<<RXCIE0);
	 UCSR0C = (1<<UCSZ00)|(1 << UCSZ01);
	 UBRR0H=0x00;
	 UBRR0L=0x67;
 }

int main(void)
{
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
	//printf("sret= %d \n",Serial.available());
	if(!Serial.available()) {
		printf("%s ",Serial.read());
		PORTC=0x80;
	}
	//PORTC=~PORTC;
	_delay_ms(500);
    }
}