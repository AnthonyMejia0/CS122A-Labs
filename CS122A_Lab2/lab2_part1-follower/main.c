/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 1 Follower
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"


unsigned char temp;
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
	initUSART(1);
	USART_Flush(1);
	TimerSet(10);
	TimerOn();

	state = Recieve;
    while (1) 
    {
		FollowerTick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

