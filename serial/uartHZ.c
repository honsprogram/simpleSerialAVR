/*
 * uartHZ.c
 *
 *  Author: HZ
 */ 
#include <stdio.h>
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
extern volatile int uart_flag;
 FILE * uart_str;



 static int uart_putchar(char c, FILE *stream) {
	 if (c == '\n') {
		 uart_putchar('\r', stream);
	 }
	 loop_until_bit_is_set(UCSR0A, UDRE0);
	 UDR0 = c;
 }

 static int uart_getchar(FILE *stream) {
	 loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	 return UDR0;
 }
 // create a string with the bits contained in this byte (not thread safe!)
 void print_bits(size_t size, void const *const ptr) {
	 unsigned char *b = (unsigned char *) ptr;
	 unsigned char byte;
	 int i, j;

	 for (i = size - 1; i >= 0; i--) {
		 for (j = 7; j >= 0; j--) {
			 byte = (unsigned char) (b[i] & (1 << j));
			 byte >>= j;
			 printf("%u", byte);
		 }
	 }
 }
 
 


//ISR(SIG_USART_RECV)
//{
	//char ReceivedByte;
	//ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	//UDR0 = ReceivedByte; // Echo back the received byte back to the computer
	//uart_flag =~ uart_flag;
//}
