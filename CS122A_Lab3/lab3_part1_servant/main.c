/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 1 Servant
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "spi.h"
#include "timer.h"

unsigned char data;
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
			PORTA = recievedData;
			break;
	}
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	SPI_ServantInit();
	TimerSet(100);
	TimerOn();

	state = Wait;
    while (1) 
    {
		ServantTick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

