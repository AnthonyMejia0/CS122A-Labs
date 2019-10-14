/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 1 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */


#include <avr/io.h>
#include "timer.h"
#define uc unsigned char
#define ul unsigned long

uc button;

enum ThreeLeds{Start, init, nextLed, pauseTL};

int ThreeLeds_tick(int state){
	switch (state){
		case Start:
			state = init;
			break;
			
		case init:
			if (button){
				state = pauseTL;
			}
			else {
				state = nextLed;
			}
			break;
			
		case nextLed:
			if (button){
				state = pauseTL;
			}
			else if ((PORTB & 0x07) >= 0x04){
				state = init;
			}
			break;
			
		case pauseTL:
			if (!button){
				state = init;
			}
			break;
			
		default:
			state = Start;
			break;
	}
	
	switch (state){
		case Start:
			break;
		
		case init:
			PORTB = 0x01;
			break;
		
		case nextLed:
			PORTB = PORTB<<1;
			break;
			
		case pauseTL:
			PORTB &= 0xFF;
			break;
	}
	
	return state;
}

enum B3_Led{blinkOn, blinkOff, pauseB3};

int B3_Led_tick(int state) {
	switch (state){
		case blinkOn:
			if (button){
				state = pauseB3;
			}
			break;
			
		case blinkOff:
			if (button){
				state = pauseB3;
			}
			break;
			
		case pauseB3:
			if (!button){
				state = blinkOn;
			}
			break;
			
		default:
			state = blinkOn;
			break;
	}
	
	switch (state){
		case blinkOn:
			PORTB |= 0x08;
			state = blinkOff;
			break;
			
		case blinkOff:
			PORTB &= 0xF7;
			state = blinkOn;
			break;
			
		case pauseB3:
			break;
	}
	
	return state;
}

typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

void checkInput() {
	static unsigned char count = 0;
	static unsigned char prev_state = 0;
	unsigned char current_state = (~PINA & 0x01);
	
	if (current_state != prev_state) {
		++count;
		
		if (count >= 4) {
			prev_state = current_state;

			if (current_state != 0) {
				button = !button;
			}
			count = 0;
		}
	}
	else {
		count = 0;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	
	ul ThreeLeds_tick_period = 500;
	ul B3_Led_tick_period = 1000;
	
	static task ThreeLeds_task;
	static task B3_Led_task;
	task *tasks[] = {&ThreeLeds_task, &B3_Led_task};
	const unsigned short numTasks = 2;
	
	ThreeLeds_task.state = -1;//Task initial state.
	ThreeLeds_task.period = ThreeLeds_tick_period;//Task Period.
	ThreeLeds_task.elapsedTime = ThreeLeds_tick_period;//Task current elapsed time.
	ThreeLeds_task.TickFct = &ThreeLeds_tick;//Function pointer for the tick.
	
	B3_Led_task.state = -1;//Task initial state.
	B3_Led_task.period = B3_Led_tick_period;//Task Period.
	B3_Led_task.elapsedTime = B3_Led_tick_period;//Task current elapsed time.
	B3_Led_task.TickFct = &B3_Led_tick;//Function pointer for the tick.
	
	TimerSet(2);
	TimerOn();
		   	
	button = 0;
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
	   // Scheduler code
	   checkInput();
	   
	   for ( i = 0; i < numTasks; i++ ) {
		   // Task is ready to tick
		   if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			   // Setting next state for task
			   tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			   // Reset the elapsed time for next tick.
			   tasks[i]->elapsedTime = 0;
		   }
		   tasks[i]->elapsedTime += 2;
	   }
	   while(!TimerFlag){}
	   TimerFlag = 0;
   }
   
   return 0;
}

