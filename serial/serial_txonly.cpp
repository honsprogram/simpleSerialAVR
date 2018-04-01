/*
 * serial.cpp
 *
 * Created: 2018-01-27 09:40:19
 *  Author: HZ
 * Serial simple Tx only
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
#include <avr/io.h>
  ISR (SIG_USART0_RECV) {
	//ReceivedByte = UDR0 ; // Fetch the received byte value into the variable " ByteReceived "
	//serial_flag=0;
 }
 //datat* new_data;
 //void USART_SendByte(uint8_t u8Data){
//
	 //// Wait until last byte has been transmitted
	 //while((UCSR0A &(1<<UDRE0)) == 0);
//
	 //// Transmit data
	 //UDR0 = u8Data;
 //}

char *word = "*";
int main(void)
{
	int i,k;

	DDRB= 0x40;
	PORTC=0x00;
	DDRC= 0xFF;
	DDRG&= 0b11110111;
	
	/* run uart */
	uart_init();
	uart_str = fdevopen(uart_putchar, uart_getchar); //send , receive functions
	stdout = uart_str;

    while(1)
    {
			PORTC |= 0X80;   _delay_ms(500);
			PORTC &= 0;_delay_ms(500);
	printf("########################################### \n");
	printf("Start \n");
	_delay_ms(600);
    }
}