// GET the 10th prime number and send to UART 

#define F_CPU 8000000UL //defines the Xtal frequency
#include <avr/io.h> //include file
#include <util/delay.h> //include file
#include <avr/interrupt.h> //include file
#include <math.h>
#include <stdio.h>


void seven_segment(int seg); //function, which controlling the 7egment
void init(void); //function for the inits
void LED_out (int LED);

void Usart_send(uint8_t character);
void Usart_init();
void uartsendstring(char *data);
unsigned char UART0_Receive( void );

volatile int value = 0;
volatile int i = 0;

int main(void) //int main function
{
	init(); 
	Usart_init(); //init functionality
	int arr[30] = {};
	int prime = 0;
	int idx = 0;
	int flag = 0;
	char Output[50];

while (prime < 1000) 
{
      flag = 0;
      if (prime <= 1) 
	  {
         ++prime;
         continue;
      }
      for (i = 2; i <= prime / 2; ++i) 
	  {
         if (prime % i == 0) 
		 {
            flag = 1;
            break;
       	 }
      }

      if (flag == 0)
	  	{
          arr[idx] = prime;
		  idx++;
		  if (idx==10)
		  break;
		}
      ++prime;
   }
		int senditem = arr[9];
		sprintf(Output, "\r\n%s%d", "The 10th Prime sequence number is:", senditem);
		uartsendstring(Output);
}

void Usart_init()
{
	DDRD |= (1<<PD1); //initialize Tx as output
	DDRD &= (1<<PD0); //initialize Rx as input
	UCSR0A =0; //set the USCR0A register to 0
	UCSR0B |= (1<<TXEN) //Set UART Tx
	| (1<<RXEN) //Set UART Rx
	| (1<<RXCIE) //Set Rx interrupt
	| (1<<TXCIE); //Set Tx interrupt
	UCSR0C | (1<<UCSZ0) //Set 8bit char size
	| (1<<UCSZ1);
	UBRR0H=0; //baud rate set to 9600
	UBRR0L=51; //baud rate set to 9600
}
void Usart_send(uint8_t character)
{
	while ( !( UCSR0A & (1<<UDRE)) ); //wait till the transmitter is ready to send0
	UDR0 = character; //put the sending value to the sending puffer
}

unsigned char UART0_Receive( void )
{
	while ( !(UCSR0A & (1<<RXC)) ); //wait for the incomming data
	return UDR0; //save the date in the imput buffer so we can read it
}

void uartsendstring(char *data)
{
	while(*data)
	{
		Usart_send (*data);
		data++;
	}
}

void init(void) //initialization function
{
	DDRB=0xF0; //Data Direction set up for the LED
	DDRD=0xF0; //Data Direction set up for the LED
	DDRG=0x00; //Data Direction set up for the Buttons
	DDRA=0xFF; //Data Direction set up for the 7segment
	DDRC=0xF8; 
}

void LED_out (int LED) //LED function for display purposes, this function expects an LED input value in ()
{
	PORTD=LED;	
	PORTB=(LED<<4); 
}
