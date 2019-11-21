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
	buttons = ~PINA & 0x0F;
	
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

enum States{Wait, Increment1, Decrement1, Increment2, Decrement2, Reset1, Reset2}state;
unsigned char value1;
unsigned char value1_2;
unsigned char value2;
unsigned char value2_2;
unsigned char value3;
unsigned char value3_2;
unsigned char display1 = 0;
unsigned char display2 = 0;

void change_display(){
	switch (state){
		case Wait:
			if (display1 && button_press == 1){
				state = Increment1;
			}
			else if (display1 && button_press == 2){
				state = Decrement1;
			}
			else if (display2 && button_press == 4){
				state = Increment2;
			}
			else if (display2 && button_press == 8){
				state = Decrement2;
			}
			else if (button_press == 3){
				state = Reset1;
			}
			else if (button_press == 12){
				state = Reset2;
			}
			break;
		
		case Increment1:
			state = Wait;
			break;
			
		case Increment2:
			state = Wait;
			break;
		
		case Decrement1:
			state = Wait;
			break;
			
		case Decrement2:
			state = Wait;
			break;
		
		case Reset1:
			state = Wait;
			break;
			
		case Reset2:
			state = Wait;
			break;
		
		default:
			state = Wait;
			break;
	}
	
	switch (state){
		case Wait:
			break;
		
		case Increment1:
			if (display1 < 3){
				display1++;
			}
			break;
			
		case Increment2:
			if (display2 < 3){
				display2++;
			}
			break;
		
		case Decrement1:
			if (display1 > 1){
				display1--;
			}
			break;
			
		case Decrement2:
			if (display2 > 1){
				display2--;
			}
			break;
		
		case Reset1:
			if (display1){
				display1 = 0;
			}
			else{
				display1 = 1;
			}
			break;
			
		case Reset2:
			if (display2){
				display2 = 0;
			}
			else{
				display2 = 1;
			}
			break;
	}
}


enum lightState1{init, forward, reverse};
unsigned char* value;

int lightShow1(int state, int reg_num){
	if (reg_num == 1){
		value = &value1;
	}
	else if (reg_num == 2){
		value = &value1_2;
	}
	
	switch (state){
		case init:
			*value = 0x01;
			transmit_data(*value, reg_num);
			state = forward;
			break;
			
		case forward:
			state = (*value == 0x80) ? reverse: forward;
			break;
			
		case reverse:
			state = (*value == 0x01) ? forward: reverse;
			break;
			
		default:
			state = init;
			break;
	}
	
	switch (state){
		case init:
			break;
			
		case forward:
			*value = *value<<1;
			transmit_data(*value, reg_num);
			break;
			
		case reverse:
			*value = *value>>1;
			transmit_data(*value, reg_num);
			break;
	}
	
	return state;
}

enum lightState2{toggle2_1, toggle2_2};
unsigned char* tempValue2;

int lightShow2(int state, int reg_num) {
	if (reg_num == 1){
		tempValue2 = &value2;
	}
	else if (reg_num == 2){
		tempValue2 = &value2_2;
	}
	
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
			*tempValue2 = 0xAA;
			transmit_data(*tempValue2, reg_num);
			break;
			
		case toggle2_2:
			*tempValue2 = 0x55;
			transmit_data(*tempValue2, reg_num);
			break;
	}
	
	return state;
}


enum lightState3{toggle3_1, toggle3_2};
unsigned char *tempValue3;
	
int lightShow3(int state, int reg_num) {
	if (reg_num == 1){
		tempValue3 = &value3;
	}
	else if (reg_num == 2){
		tempValue3 = &value3_2;
	}
	
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
			*tempValue3 = 0xC3;
			transmit_data(*tempValue3, reg_num);
			break;
		
		case toggle3_2:
			*tempValue3 = 0x3C;
			transmit_data(*tempValue3, reg_num);
			break;
	}
	
	return state;
}


int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(100);
	TimerOn();
	int light_state1 = init;
	int light_state2 = toggle2_1;
	int light_state3 = toggle3_1;
	
	int light_state1_2 = init;
	int light_state2_2 = toggle2_1;
	int light_state3_2 = toggle3_1;
	
	transmit_data(0x00, 1);
	transmit_data(0x00, 2);
	
    while (1) 
    {
		getInput();
		change_display();
		
		if (display1 == 1){
			light_state1 = lightShow1(light_state1, 1);
		}
		else if (display1 == 2){
			light_state2 = lightShow2(light_state2, 1);
		}
		else if (display1 == 3){
			light_state3 = lightShow3(light_state3, 1);
		}
		else if (display1 == 0){
			//value3 = 0;
			light_state1 = init;
			transmit_data(0, 1);
		}
		
		if (display2 == 1){
			light_state1_2 = lightShow1(light_state1_2, 2);
		}
		else if (display2 == 2){
			light_state2_2 = lightShow2(light_state2_2, 2);
		}
		else if (display2 == 3){
			light_state3_2 = lightShow3(light_state3_2, 2);
		}
		else if (display2 == 0){
			//value2 = 0;
			light_state1_2 = init;
			transmit_data(0, 2);
		}
		
		//light_state1 = lightShow1(light_state1, 1);
		//light_state2 = lightShow2(light_state2, 2);
		
		while (!TimerFlag);
		TimerFlag = 0;
    }
}

