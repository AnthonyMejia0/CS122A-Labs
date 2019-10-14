/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 2 Leader
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

enum Follower_States{Recieve}state2;

void FollowerTick() {
	switch (state2) {
		case Recieve:
		break;
		
		default:
		state2 = Recieve;
		break;
	}
	
	switch (state2) {
		case Recieve:
		if (USART_HasReceived(0)){
			temp = USART_Receive(0);
			PORTA = temp;
			USART_Flush(0);
		}
		break;
	}
}

int main(void)
{	
    DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	initUSART(0);
	TimerOn();

	state = SEND;
	state2 = Recieve;
    while (1) 
    {
		if (~PINB & 0x01){
			PORTC = 0x01;
			TimerSet(1000);
			LeaderTick();
		}
		else {
			PORTC = 0x00;
			TimerSet(10);
			FollowerTick();
		}
		
		while (!TimerFlag);
		TimerFlag = 0;
	}
}