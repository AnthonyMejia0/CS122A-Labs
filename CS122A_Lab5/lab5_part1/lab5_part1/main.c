/*
 * lab5_part1.c
 *
 * Created: 10/23/2019 6:27:06 PM
 * Author : Antho
 */ 

#include <avr/io.h>
#include "shift_register.h"
#include "timer.h"

enum States{Wait, Increment, Decrement, Reset}state;
unsigned char value;
	
void change_value(){
	unsigned char inc_button = ~PINA & 0x01;
	unsigned char dec_button = ~PINA & 0x02;
	switch (state){
		case Wait:
			if (inc_button && !dec_button){
				state = Increment;
			}
			else if (dec_button && !inc_button){
				state = Decrement;
			}
			else if (inc_button && dec_button){
				state = Reset;
			}
			break;
			
		case Increment:
			state = Wait;
			break;
			
		case Decrement:
			state = Wait;
			break;
			
		case Reset:
			state = Wait;
			break;
	}
	
	switch (state){
		case Wait:
			break;
			
		case Increment:
			if (value < 0xFF){
				value++;
				transmit_data(value);
			}
			break;
			
		case Decrement:
			if (value > 0x00){
				value--;
				transmit_data(value);
			}
			break;
			
		case Reset:
			value = 0x00;
			transmit_data(value);
			break;
	}
}


int main(void)
{
	DDRA = 0xFC; PORTA = 0x03;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	
	value = 0x00;
	transmit_data(value);
	
    while (1) 
    {
		change_value();
					
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

