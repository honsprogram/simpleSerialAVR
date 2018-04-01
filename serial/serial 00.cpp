#define F_CPU 16000000UL  // 8 MHz

/*Very Important - change F_CPU to match target clock 
  Note: default AVR CLKSEL is 1MHz internal RC
  This program transmits continously on USART. Interrupt is used for 
	Receive charactor, which is then transmitted instead. LEDs are used 
	as a test. Normal RX routine is included but not used.
  Change USART_BAUDRATE constant to change Baud Rate
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define baud rate
#define USART_BAUDRATE 9600   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
int x=0xFF;
volatile unsigned char value;  
/* This variable is volatile so both main and RX interrupt can use it.
It could also be a uint8_t type */
void USART_SendByte(uint8_t u8Data){

	// Wait until last byte has been transmitted
	while((UCSR0A &(1<<UDRE0)) == 0);

	// Transmit data
	UDR0 = u8Data;
}
/* Interrupt Service Routine for Receive Complete 
NOTE: vector name changes with different AVRs see AVRStudio -
Help - AVR-Libc reference - Library Reference - <avr/interrupt.h>: Interrupts
for vector names other than USART_RXC_vect for ATmega32 */

ISR(USART_RXC_vect){
 
   value = UDR0;             //read UART register into value
   PORTC = ~PORTC;          // output inverted value on LEDs (0=on)
    // UCSR0B = ((1<<TXEN0)|(1<<RXEN0) | (1<<RXCIE0));
	//UCSR0A=0x00;
	value=5;
	//USART_SendByte(value); 

}



// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: On
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud Rate: 9600
//UCSR0A=0x00;
//UCSR0B=0x98;
//UCSR0C=0x06;
//UBRR0H=0x00;
//UBRR0L=0x67;

// USART1 initialization
// USART1 disabled
//UCSR1B=0x00;


void USART_Init(void){

   //UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
   UCSR0C=0x06;
   //UBRR0L = BAUPRESCALE;// Load lower 8-bits into the low byte of t
  // UBRR0H = (BAUD_PRESCALE >> 8); 
   UBRR0H=0x00;
   UBRR0L=0x67;
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/ 

  // Enable receiver and transmitter and receive complete interrupt 
 // UCSR0B = ((1<<TXEN0)|(1<<RXEN0) | (1<<RXCIE0));
  UCSR0B=0x98;
   
   UCSR0A=0x00;
   UCSR1B=0x00;
}





// not bng used but here for completeness
      // Wait until a byte has been received and return received data 
uint8_t USART_ReceiveByte(){
  while((UCSR0A &(1<<RXC0)) == 0);
  return UDR0;
}

void Led_init(void){
   //outputs, all off
	DDRC =0xFF;       
    PORTC = 0xFF;        
}

int main(void){
	
	

   USART_Init();  // Initialise USART
   
   sei();         // enable all interrupts
  
   Led_init();    // init LEDs for testing
   value = 'A'; //0x41;    
   PORTC = ~value; // 0 = LED on
   USART_SendByte(1);
   for(;;){    // Repeat indefinitely
     //lue=5;        
    USART_SendByte(value);  // send value 
	//SART_SendByte(0XF0);  // send value 
     _delay_ms(250);         
	PORTC = ~PORTC; 	         // delay just to stop Hyperterminal screen cluttering up    
			//	 PORTC = ~PORTC; 
   }
}
	