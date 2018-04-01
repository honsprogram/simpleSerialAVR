/*
 * uartHZ.c
 *
 *  Author:  Hossein Zamanian (hznewresearch@gmail.com)
 */ 
#include <stdio.h>
#define F_CPU 16000000
#define BAUD 9600
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <stdlib.h>
extern volatile int uart_flag;
#include "uartHZ.h"
static int uart_putchar(char c, FILE *stream) ;
char ReceivedByte=0;
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
 




