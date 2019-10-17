/*	Partner(s) Name & E-mail: Anthony Mejia, ameji012@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab # 4 Exercise # 2 part 1 & 2
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

enum LEDState1 {INIT1, TOGGLE1} led_state_1;
	
void LEDS1_init(){
	led_state_1 = INIT1;
}

void LEDS1_TICK(){
	switch(led_state_1){
		case INIT1:
			led_state_1 = TOGGLE1;
			break;
			
		case TOGGLE1:
			PORTD ^= 0x01;
			break;
			
		default:
			led_state_1 = INIT1;
			break;
	}
}


enum LEDState2 {INIT2, TOGGLE2} led_state_2;

void LEDS2_init(){
	led_state_2 = INIT2;
}

void LEDS2_TICK(){
	switch(led_state_2){
		case INIT2:
			led_state_2 = TOGGLE1;
			break;
		
		case TOGGLE2:
			PORTD ^= 0x04;
			break;
		
		default:
			led_state_2 = INIT2;
			break;
	}
}


enum LEDState3 {INIT3, TOGGLE3} led_state_3;
void LEDS3_init(){
	led_state_3 = INIT3;
}

void LEDS3_TICK(){
	switch(led_state_3){
		case INIT3:
			led_state_3 = TOGGLE3;
			break;
		
		case TOGGLE3:
			PORTD ^= 0x10;
			break;
		
		default:
			led_state_3 = INIT3;
			break;
	}
}

void LedSecTask(){
	LEDS1_init();
	for(;;){
		LEDS1_TICK();
		vTaskDelay(500);
	}
}

void LedSecTask2(){
	LEDS2_init();
	for(;;){
		LEDS2_TICK();
		vTaskDelay(1000);
	}
}

void LedSecTask3(){
	LEDS3_init();
	for(;;){
		LEDS3_TICK();
		vTaskDelay(2500);
	}
}


void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask,
	(signed portCHAR *)"LedSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
	
	xTaskCreate(LedSecTask2,
	(signed portCHAR *)"LedSecTask2",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
	
	xTaskCreate(LedSecTask3,
	(signed portCHAR *)"LedSecTask3",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}


int main(void)
{
    DDRD = 0xFF; PORTD = 0x00;
	StartSecPulse(1);
	vTaskStartScheduler();
	return 0;
}

