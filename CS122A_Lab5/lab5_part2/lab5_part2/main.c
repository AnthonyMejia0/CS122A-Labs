/*
 * lab5_part2.c
 *
 * Created: 10/23/2019 7:15:51 PM
 * Author : Antho
 */ 

#include <avr/io.h>
#include "shift_register.h"
#include "timer.h"

enum input_states{wait, press, depress}button_state;
unsigned char buttons = 0;
unsigned char button_press;

void getInput(){
	buttons = ~PINA & 0x03;
	
	switch (button_state){
		case wait:
			button_state = (buttons) ? press: wait;
			break;
			
		case press:
			button_state = depress;
			button_press = 0;
			break;
			
		case depress:
			button_state = (buttons) ? depress: wait;
			break; 
			
		default:
			button_state = wait;
			break;
	}
	
	switch (button_state){
		case wait:
			break;
			
		case press:
			button_press = buttons;
			break;
			
		case depress:
			break;
	}
}

enum States{Wait, Increment, Decrement, Reset}state;
unsigned char value;
unsigned char display = 1;

void change_display(){
	switch (state){
		case Wait:
			if (display && button_press == 1){
				state = Increment;
			}
			else if (display && button_press == 2){
				state = Decrement;
			}
			else if (button_press == 3){
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
		
		default:
			state = Wait;
			break;
	}
	
	switch (state){
		case Wait:
			break;
		
		case Increment:
			if (display < 3){
				display++;
			}
			break;
		
		case Decrement:
			if (display > 1){
				display--;
				transmit_data(value);
			}
			break;
		
		case Reset:
			if (display){
				display = 0;
			}
			else{
				display = 1;
			}
			break;
	}
}


enum lightState1{init, forward, reverse};
	
int lightShow1(int state){
	switch (state){
		case init:
			value = 0x01;
			transmit_data(value);
			state = forward;
			break;
			
		case forward:
			state = (value == 0x80) ? reverse: forward;
			break;
			
		case reverse:
			state = (value == 0x01) ? forward: reverse;
			break;
			
		default:
			state = init;
			break;
	}
	
	switch (state){
		case init:
			break;
			
		case forward:
			value = value<<1;
			transmit_data(value);
			break;
			
		case reverse:
			value = value>>1;
			transmit_data(value);
			break;
	}
	
	return state;
}

enum lightState2{toggle2_1, toggle2_2};
int lightShow2(int state) {
	switch (state){
		case toggle2_1:
			state = toggle2_2;
			break;
		
		case toggle2_2:
			state = toggle2_1;
			break;
			
		default:
			state = toggle2_1;
			break;
	}
	
	switch (state){
		case toggle2_1:
			value = 0xAA;
			transmit_data(value);
			break;
			
		case toggle2_2:
			value = 0x55;
			transmit_data(value);
			break;
	}
	
	return state;
}


enum lightState3{toggle3_1, toggle3_2};
int lightShow3(int state) {
	switch (state){
		case toggle3_1:
			state = toggle3_2;
			break;
		
		case toggle3_2:
			state = toggle3_1;
			break;
		
		default:
			state = toggle3_1;
			break;
	}
	
	switch (state){
		case toggle3_1:
			value = 0xC3;
			transmit_data(value);
			break;
		
		case toggle3_2:
			value = 0x3C;
			transmit_data(value);
			break;
	}
	
	return state;
}


int main(void)
{
	DDRA = 0xFC; PORTA = 0x03;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(100);
	TimerOn();
	int light_state1 = init;
	int light_state2 = toggle2_1;
	int light_state3 = toggle3_1;
	
    while (1) 
    {
		getInput();
		change_display();
		PORTD = display;
		
		if (display == 1){
			light_state1 = lightShow1(light_state1);
		}
		else if (display == 2){
			light_state2 = lightShow2(light_state2);
		}
		else if (display == 3){
			light_state3 = lightShow3(light_state3);
		}
		else if (display == 0){
			value = 0;
			light_state1 = init;
			transmit_data(value);
		}
		
		while (!TimerFlag);
		TimerFlag = 0;
    }
}

