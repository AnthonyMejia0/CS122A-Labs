/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 3 Servant
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "spi.h"
#include "timer.h"

enum Pattern1States {p1_1, p1_2}p1_state;
void Pattern1Tick(){
	switch(p1_state){
		case p1_1:
			p1_state = p1_2;
			break;
			
		case p1_2:
			p1_state = p1_1;
			break;
			
		default:
			p1_state = p1_1;
			break;
	}
	
	switch(p1_state){
		case p1_1:
			PORTA = 0xF0;
			break;
			
		case p1_2:
			PORTA = 0x0F;
			break;
	}
}	


enum Pattern2States {p2_1, p2_2}p2_state;
void Pattern2Tick(){
	switch(p2_state){
		case p2_1:
			p2_state = p2_2;
			break;
		
		case p2_2:
			p2_state = p2_1;
			break;
		
		default:
			p2_state = p2_1;
			break;
	}
	
	switch(p2_state){
		case p2_1:
			PORTA = 0xAA;
			break;
		
		case p2_2:
			PORTA = 0x55;
			break;
	}
}


enum Pattern3States {p3_start, p3_forward, p3_reverse}p3_state;
void Pattern3Tick(){
	switch(p3_state){
		case p3_start:
			PORTA = 0x80;
			p3_state = p3_forward;
			break;
		
		case p3_forward:
			p3_state = (PORTA == 0x01) ? p3_reverse: p3_forward;
			break;
			
		case p3_reverse:
			p3_state = (PORTA == 0x80) ? p3_forward: p3_reverse;
			break;
		
		default:
			p3_state = p3_start;
			break;
	}
	
	switch(p3_state){
		case p3_start:
			break;
		
		case p3_forward:
			PORTA = (PORTA>>1);
			break;
			
		case p3_reverse:
			PORTA = (PORTA<<1);
			break;
	}
}


enum Pattern4States {p4_1, p4_2}p4_state;
void Pattern4Tick(){
	switch(p4_state){
		case p4_1:
			p4_state = p4_2;
			break;
		
		case p4_2:
			p4_state = p4_1;
			break;
		
		default:
			p4_state = p4_1;
			break;
	}
	
	switch(p4_state){
		case p4_1:
			PORTA = 0xC3;
			break;
		
		case p4_2:
			PORTA = 0x3C;
			break;
	}
}

unsigned char currentSetup;
unsigned char data = 0x11;
enum ServantStates {Wait, Recieve}state;
	
void ServantTick(){
	switch(state){
		case Wait:
			state = Recieve;
			break;
			
		case Recieve:
			state = Wait;
			break;
			
		default:
			state = Wait;
			break;
	}
	
	switch(state){
		case Wait:
			break;
			
		case Recieve:
			currentSetup = data;
			data = recievedData;
			break;
	}
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	SPI_ServantInit();
	TimerSet(10);
	TimerOn();

	unsigned long int speeds[6] = {2000, 1000, 500, 250, 100, 50};
	void (*patterns[4])(void);
	patterns[0] = Pattern1Tick;
	patterns[1] = Pattern2Tick;
	patterns[2] = Pattern3Tick;
	patterns[3] = Pattern4Tick;
	
	state = Wait;
	p1_state = p1_1;
	p2_state = p2_2;
	p3_state = p3_start;
	p4_state = p4_1;
	unsigned char lowerNibble;
	unsigned char upperNibble;
    while (1) 
    {
		ServantTick();
		if (data != currentSetup){
			lowerNibble = data & 0x0F;
			upperNibble = (data & 0xF0)>>4;
			
			if (lowerNibble >= 1 && lowerNibble <= 6){
				TimerSet(speeds[lowerNibble-1]);
			}
		}
		
		if (upperNibble >= 1 && upperNibble <= 4){
			patterns[upperNibble-1]();
		}
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

