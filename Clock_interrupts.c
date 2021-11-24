// Clock to count on 7-seg display from 00m:00s to 99:99s using 8-bit timer interrrupt. 

#define F_CPU 8000000UL //defines the Xtal frequency
#include <avr/io.h> //include file
#include <util/delay.h> //include file
#include <avr/interrupt.h> //include file
#include <math.h>


void seven_segment(int seg); //function, which controlling the 7egment
void init(void); //function for the inits
void LED_out (int LED);

volatile int extraTime = 0;
volatile int i = 0;
volatile int minute, clock = 0;

int main(void) //int main function

{
	init(); 
	Timer0();

	while (1) //while1 function
	{
		seven_segment(clock);
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

void seven_segment(int seg) {

	int num, temp1=0,temp2=0,num2=0,temp3=0,num3=0;
	num=seg;

	temp1=num/1000;
	num=num%1000;
	temp2=num/100;
	num2=num%100;
	temp3=num2/10;
	num3=num2%10;
	
	PORTA =(0xB0 | temp1);
	PORTA =(0xA0|temp2);
	PORTA =(0x90|temp3);
	PORTA =(0x80|num3);
}

void Timer0(void)
{
	TCCR0 = (1 << WGM01); //Set CTC Bit
	TIFR = 1;
	OCR0 = 10;
	TCNT0 = 0;
	TIMSK = 2;	
	TCCR0 = (1 << CS02) | (1 << CS01); //start at 256 prescaler
	sei();
}

 ISR(TIMER0_COMP_vect)
{
	extraTime++;
	
	if(extraTime == 122)  // 256 PRESCALER AND 122 VARIABLE EQUALS 1sec
	{
		i++;
	
		if (i > 59) {
		minute++; 
		i = 0;
		}

		clock = (minute*100 + i);
		extraTime = 0;
		LED_out(i);
		seven_segment(clock);

		(minute > 99) ? (minute = 0) : 1;
	}
	seven_segment(clock);
}



