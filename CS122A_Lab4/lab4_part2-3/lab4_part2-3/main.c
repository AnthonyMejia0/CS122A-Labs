/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 4 Exercise # 2 part 3 & 4
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

enum LEDState {INIT, FORWARD, REVERSE} led_state;
	
void LEDS_init(){
	led_state = INIT;
}

unsigned char button = 0;
void LEDS_TICK(){
	button = ~PINA & 0x01;
	switch(led_state){
		case INIT:
			PORTD = 1;
			led_state = FORWARD;
			break;
			
		case FORWARD:
			led_state = (PORTD == 0x80 || button) ? REVERSE: FORWARD;
			break;
			
		case REVERSE:
			led_state = (PORTD == 0x01) ? FORWARD: REVERSE;
			break;
			
		default:
			led_state = INIT;
			break;
	}
	
	switch(led_state){
		case INIT:
			break;
			
		case FORWARD:
			PORTD = PORTD<<1;
			break;
			
		case REVERSE:
			PORTD = PORTD>>1;
			break;
	}
}

void LedSecTask(){
	LEDS_init();
	for(;;){
		LEDS_TICK();
		vTaskDelay(75);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask,
	(signed portCHAR *)"LedSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
    DDRD = 0xFF; PORTD = 0x00;
	StartSecPulse(1);
	vTaskStartScheduler();
	return 0;
}

