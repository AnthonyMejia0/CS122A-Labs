/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"

unsigned long period;
unsigned long t = 0;
void (*tick)(void);

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
				temp ^= 0x01;
				PORTA = temp;
				USART_Send(temp, 1);
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

unsigned char isFollower;
enum Switch_States{Start, Lead, Follow}Sw_State;

void Switch_States_Tick() {
	switch(Sw_State){
		case Start:
			tick = &FollowerTick;
			Sw_State = Follow;
			break;
		
		case Lead:
			if (USART_HasReceived(0)){
				Sw_State = Follow;
				tick = &FollowerTick;
			}
			break;
		
		case Follow:
			if (t >= 5000){
				Sw_State = Lead;
				tick = &LeaderTick;
			}
			else{
				t += period;
			}
			break;
		
		default:
			Sw_State = Start;
			break;
	}
	
	switch(Sw_State){
		case Start:
			break;
		
		case Lead:
			if (period != 1000){
				period = 1000;
			}
			isFollower = 0;
			t = 0;
			break;
		
		case Follow:
			if (period != 10){
				period = 10;
			}
			isFollower = 1;
			break;
	}
	
	tick();
}

int main(void)
{	
    DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	initUSART(0);
	initUSART(1);
	TimerSet(10);
	TimerOn();

	Sw_State = Start;
	state = SEND;
	state2 = Recieve;

    while (1) 
    {
		Switch_States_Tick();
		TimerSet(period);
		PORTC = (isFollower) ? 0x00: 0x01;
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}