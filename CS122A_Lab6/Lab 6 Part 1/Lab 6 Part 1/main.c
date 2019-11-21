/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 6 Exercise # 1
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "lcd.h"
#include "scheduler.h"
#include "incDecSM.h"

enum LCDSTATES{lcd_Start, off, on};
	
int lcd_tick(int state){
	static unsigned long time;
	unsigned char button = (~PINA & 0x01);
	
	switch (state){
		case lcd_Start:
			time = 0;
			state = off;
			break;
			
		case off:
			state = (button) ? on: off;
			break;
			
		case on:
			if (time == 60){
				time = 0;
				state = off;
			}
			break;
			
		default:
			state = Start;
			break;
	}
	
	switch (state){
		case Start:
			break;
			
		case off:
			LCD_ClearScreen();
			tasks[0].active = 0x00;
			PORTB = 0x00;
			break;
			
		case on:
			tasks[0].active = 0x01;
			PORTB = 0x01;
			time++;
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
			break;
	}
	
	return state;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
	
	tasksNum = 2;
	task myTasks[2];
	tasks = myTasks;
	
	tasks[0].active = 0x01;
	tasks[0].state = -1;
	tasks[0].period = 100;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &IncDec_tick;
	
	tasks[1].active = 0x01;
	tasks[1].state = -1;
	tasks[1].period = 50;
	tasks[1].elapsedTime = tasks[1].period;
	tasks[1].TickFct = &lcd_tick;
	
	TimerSet(50);
	TimerOn();
	
    while (1) 
    {
    }
}

