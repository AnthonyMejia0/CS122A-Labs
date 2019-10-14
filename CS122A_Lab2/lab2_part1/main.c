/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 1 Leader
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"


unsigned char temp = 0x01;
enum Leader_States{SEND}state;
	
void LeaderTick() {
	switch (state) {
		case SEND:
			break;
			
		default:
			state = SEND;
			break;
	}	
	
	switch (state) {
		case SEND:
			if (USART_IsSendReady(0)) {
				temp ^= 0x01;
				PORTA = temp;
				USART_Send(temp, 0);
			}
			break;
	}
}

int main(void)
{	
    DDRA = 0xFF; PORTA = 0x00;
	initUSART(0);
	TimerSet(1000);
	TimerOn();

	state = SEND;
    while (1) 
    {
		LeaderTick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

