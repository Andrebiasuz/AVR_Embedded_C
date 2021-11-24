// Read from Temperature sensor and show values in the 7-seg display

#define F_CPU 8000000 //defines the Xtal frequency
#define DATA_DDR DDRE //data ddr
#define DATA_PORT PORTE // data port upper 4bit
#define COMMAND_DDR DDRF //command ddr
#define COMMAND_PORT PORTF // command port
#define CMD_RS PF1 //Register Select 0 command send, 1 data send
#define CMD_RW PF2 //1 Read / 0 Write
#define CMD_EN PF3 //Enable

#include <avr/io.h> //include file
#include <util/delay.h> //include file
#include <avr/interrupt.h> //include file
#include <inttypes.h>

void seven_segment(int seg); //function, which controlling the 7egment
void init(void); //function for the inits
void ADC_init(); //initialize the AD
int ADC_read(int ch); // read out functionality from AD channel

volatile double celsius = 0;
volatile double scale = 0;

int main(void) //int main function
{
	init(); 
	Timer0();
	ADC_init();

	while (1) //while1 function
	{

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

void ADC_init()
{
	ADCSRA =(1<<ADEN) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADPS0);
	ADMUX = (1<<REFS0); //Set the admux register to reserved so 5V

}

int ADC_read(int ch)
{
	ADMUX &= 0xE0; //set reference,set to store the aad result to left adjustment result
	ADMUX |= ch&0x1F; //select the chanel from where we want to read
	ADCSRA |= (1<<ADSC); //set to start the conversion
	while(!(ADCSRA&(1<<ADIF))); //wait till the interrupt flag set
	return ADCW; //return with the read out value / result of AD conversion, its a 16 bit value
}

void seven_segment(int seg) 
{
	int num, temp1=0,temp2=0,num2=0,temp3=0,num3=0;
	num=seg;

	temp1=num/1000;
	num=num%1000;
	temp2=num/100;
	num2=num%100;
	temp3=num2/10;
	num3=num2%10;
	
	PORTA =(0xB0 | temp1);
	_delay_ms(1);
	PORTA =(0xA0|temp2);
	_delay_ms(1);
	PORTA =(0x90|temp3);
	_delay_ms(1);
	PORTA =(0x80|num3);
	_delay_ms(1);
}

void Timer0(void)
{
	TCCR0 = (1 << WGM01); //Set CTC Bit
	OCR0 = 10;
	TIMSK = 2;	
	TCCR0 = (1 << CS00); //start at 8 prescaler

	sei();
}

ISR(TIMER0_COMP_vect)
{
	scale = ADC_read(0); //read out the adc value from the channel 0
	celsius = scale *0.488; //convert the the voltage value to celsius
	seven_segment(celsius*100);
	_delay_ms(10);

}













