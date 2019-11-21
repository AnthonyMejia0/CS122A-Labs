/*
 * Lab 7 Part 1-Controller1.c
 *
 * Created: 11/1/2019 6:19:15 PM
 * Author : Antho
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include "timer.h"
#include "spi.h"


void ADC_init()
{
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADSC);
}

//adc_read function found on http://maxembedded.com/2011/06/the-adc-of-the-avr/
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	ch &= 0x07;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}



enum states{GetADC}state;
	
unsigned short data;
void getADC_Tick(){	
	switch(state) {
		case GetADC:
			data = adc_read(0);
			//unsigned char newData = (data>>4 & 0xFF);
			//SPDR = newData;
			if (recievedData == 0x10){
				SPDR = (data & 0xFF);
			}
			else if (recievedData == 0x20 || recievedData == 0x40){
				SPDR = recievedData;
			}

			state = GetADC;
			break;
			
		default:
			state = GetADC;
			break;
	}
}



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	SPI_ServantInit();
	ADC_init();
	TimerSet(1000);
	TimerOn();
	
	state = GetADC;
	
	while (1)
	{
		getADC_Tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

