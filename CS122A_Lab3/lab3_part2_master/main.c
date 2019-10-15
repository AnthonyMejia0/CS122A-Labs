/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 2 Master
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "bit.h"
#include "lcd.h"
#include "keypad.h"
#include "timer.h"
#include "spi.h"

unsigned char currentSetup;
unsigned char data = 0x11;

enum Keypad_States { KP_wait, KP_press }key_state;
void keyTick() {
	unsigned char key_press;
	currentSetup = data;

	switch (key_state) {
		case KP_wait:
			key_press = GetKeypadKey();
			key_state = (key_press != '\0') ? KP_press: KP_wait;
			break;
		
		case KP_press:
			key_state = KP_wait;
			break;
		
		default:
			key_state = KP_wait;
			break;
	}
	

	switch(key_state) {
		case KP_wait:
			key_press = GetKeypadKey();
			break;
		
		case KP_press:
		switch (key_press) {
			case '1': data = (data & 0xF0) | 0x01; break; // hex equivalent
			case '2': data = (data & 0xF0) | 0x02; break;
			case '3': data = (data & 0xF0) | 0x03; break;
			case '4': data = (data & 0xF0) | 0x04; break;
			case '5': data = (data & 0xF0) | 0x05; break;
			case '6': data = (data & 0xF0) | 0x06; break;
			case 'A': data = (data & 0x0F) | 0x10; break;
			case 'B': data = (data & 0x0F) | 0x20; break;
			case 'C': data = (data & 0x0F) | 0x30; break;
			case 'D': data = (data & 0x0F) | 0x40; break;
		}
		break;
	}
}


enum MasterState {Wait, Send} state;
void MasterTick(){
	switch(state){
		case Wait:
			state = Send;
			break;
			
		case Send:
			state = Wait;
			break;
			
		default:
			state = Wait;
			break;
	}
	
	switch(state){
		case Wait:
			break;
		
		case Send:
			SPI_MasterTransmit(data);
			break;
	}
}

int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	SPI_MasterInit();
	LCD_init();
	TimerSet(50);
	TimerOn();
	
	state = Wait;
	key_state = KP_wait;
	while(1){
		keyTick();
		MasterTick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

