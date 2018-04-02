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

int main(void)
{
	PORTC |= 0X80; 
	DDRC= 0xFF;
	/* run uart */
	sei();
	uart_init();
	uart_str = fdevopen(uart_putchar, uart_getchar); //send , receive functions
	stdout = uart_str;
	printf("serial simple avr\n");
    while(1)
    {
	if(!Serial.available()) {
		printf("%s ",Serial.read());
	}
	PORTC=~PORTC;
	_delay_ms(500);
    }
}