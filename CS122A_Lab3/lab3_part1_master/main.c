/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 1 Master
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

enum MasterState {Wait, Send} state;
unsigned char data = 0x00;
	
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
			if (data == 0xFF){
				data = 0x00;
			}
			else{
				data++;
			}
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
	TimerSet(1000);
	TimerOn();
	
	state = Wait;
	while(1){
		MasterTick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

