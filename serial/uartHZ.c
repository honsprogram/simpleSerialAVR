/*
 * uartHZ.c
 *
 *  Author:  Hossein Zamanian (hznewresearch@gmail.com)
 */ 
#define F_CPU 16000000
#define BAUD 9600
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <stdlib.h>
#include "uartHZ.h"
// buffer of UART0
#define FRAMING_ERROR0 			(1<<FE0)
#define DATA_OVERRUN0 			(1<<DOR0)
#define DATA_REGISTER_EMPTY0 	(1<<UDRE0)
#define TX_COMPLETE0 	 		(1<<TXC0)
#define UART0_RX_BUFFER_SIZE 300
extern volatile int uart_flag;
bool volatile serial_flag=1;// This flag is set on USART Receiver buffer overflow

unsigned char rx_buffer_overflow={0},rx_packet_ready={0};
unsigned char rx_wr_index={0},rx_rd_index={0},rx_counter={0};
unsigned char recev_state={0};
unsigned char Packet_error={0};
FILE * uart_str;

typedef struct {
	unsigned char rx[UART0_RX_BUFFER_SIZE];
}__attribute((packed)) BUFFERTYPE0;
BUFFERTYPE0 bufferUART0;

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

unsigned char * hzserial::read(void){
	cli();
	serial_flag=1;
	sei();
	return bufferUART0.rx;
}

hzserial Serial = hzserial();

void uart_init(void) {
	UCSR0B =  (1 << RXEN0) | (1 << TXEN0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ00)|(1 << UCSZ01);
	UBRR0H=0x00;
	UBRR0L=0x67;
}

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
 /*------interrupt handler-- interrupt service routine (ISR)------
 -----------------------------------------------------------------
 */
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
 




