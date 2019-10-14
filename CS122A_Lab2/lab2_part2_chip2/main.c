/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 2 Follower
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"


unsigned char temp = 0x01;
enum Leader_States{SEND}state2;

void LeaderTick() {
	switch (state2) {
		case SEND:
		break;
		
		default:
		state2 = SEND;
		break;
	}
	
	switch (state2) {
		case SEND:
		if (USART_IsSendReady(1)) {
			temp ^= 0x01;
			PORTA = temp;
			USART_Send(temp, 1);
		}
		break;
	}
}


enum Follower_States{Recieve}state;
	
void FollowerTick() {
	switch (state) {
		case Recieve:
			break;
			
		default:
			state = Recieve;
			break;
	}	
	
	switch (state) {
		case Recieve:
			if (USART_HasReceived(1)){
				temp = USART_Receive(1);
				PORTA = temp;
				USART_Flush(1);
			}
			break;
	}	
}

int main(void)
{	
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
	initUSART(1);
	USART_Flush(1);
	TimerOn();

	state = Recieve;
	state2 = SEND;
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
		
		FollowerTick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

