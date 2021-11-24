// Use UART to set GUI for user to control ATMEGA64 I/Os using written commands

#define F_CPU 8000000UL //defines the Xtal frequency
#include <avr/io.h> //include file
#include <util/delay.h> //include file
#include <avr/interrupt.h> //include file
#include <math.h>
#include <stdio.h>
#include <string.h>

void seven_segment(int seg); //function, which controlling the 7egment
void init(void); //function for the inits
void LED_out (int LED);

void Usart_send(uint8_t character);
void Usart_init();
void uartsendstring(char *data);
unsigned char UART0_Receive( void );

volatile int value = 0;
int aux = 0;
int i = 0;
int result,result2, result3,result4,result5,result6 ,result7,result8 = 1;

char receive = 0;
char arr[5] = {};

int main(void) //int main function
{	
	init(); 
	Usart_init(); //init functionality

	while (1) //while1 function
	{
		uartsendstring("\r\nInput command:");
		for (i=0; i<=7; i++)
		{	
			receive = UART0_Receive();
			if (receive == '\r')
			{
				break;
			}
			else
			{
				arr[i] = receive;
			}
		}
		char comp1[] = "L1_ON";
		char comp2[] = "L1_OFF";
		char comp3[] = "L2_ON";
		char comp4[] = "L2_OFF";
		char comp5[] = "L3_ON";
		char comp6[] = "L3_OFF";
		char comp7[] = "L4_ON";
		char comp8[] = "L4_OFF";
		result = strncmp(arr,comp1,strlen(comp1));
		result2 = strncmp(arr,comp2,strlen(comp2));
		result3 = strncmp(arr,comp3,strlen(comp3));
		result4 = strncmp(arr,comp4,strlen(comp4));
		result5 = strncmp(arr,comp5,strlen(comp5));
		result6 = strncmp(arr,comp6,strlen(comp6));
		result7 = strncmp(arr,comp7,strlen(comp7));
		result8 = strncmp(arr,comp8,strlen(comp8));
		if (result == 0)
		{
			aux= aux+1;
			LED_out(aux);
			result= -32;
		}	
		if (result2 == 0)
		{
			aux = aux-1;
			LED_out(aux);;
			result2 = -32;
		}
		if (result3 == 0)
		{
			aux = aux+2;
			LED_out(aux);
			result3 = -32;
		}
		if (result4 == 0)
		{
			aux = aux-2;
			LED_out(aux);
			result4 = -32;
		}
		if (result5 == 0)
		{
			aux = aux+4;
			LED_out(aux);
			result5 = -32;
		}
		if (result6 == 0)
		{
			aux = aux-4;
			LED_out(aux);
			result6 = -32;
		}
		if (result7 == 0)
		{
			aux = aux+8;
			LED_out(aux);
			result7 = -32;
		}
		if (result8 == 0)
		{
			aux = aux-8;
			LED_out(aux);
			result8 = -32;
		}
		memset(arr, 0, 5);
	}
}

void Usart_init()
{
	DDRD |= (1<<PD1); //initialize Tx as output
	DDRD &= (1<<PD0); //initialize Rx as input
	UCSR0A = 0; //set the USCR0A register to 0
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
	DDRC=0xFF;
	DDRE=0XFF; 

}

void LED_out (int LED) //LED function for display purposes, this function expects an LED input value in ()
{
	PORTD=LED;	
	PORTB=(LED<<4); 
}

















