/*
 * Lab 7 Part 1-Controller2.c
 *
 * Created: 11/1/2019 6:46:24 PM
 * Author : Antho
 */ 

#include <avr/io.h>
#include "timer.h"
#include "spi.h"
#include "lcd.h"

enum states{Display}state;
unsigned char prevData;

void display_tick(){
	unsigned char data;
	
	switch(state){
		case Display:
			data = recievedData;
			if (data != prevData){
				LCD_ClearScreen();
			}
			
			unsigned char temp = data;
			if ((temp/100) != 0){
				LCD_Cursor(1);
				LCD_WriteData((temp/100) + '0');
				temp -= (100*(temp/100));
				LCD_Cursor(2);
				LCD_WriteData((temp/10) + '0');
				temp -= (10*(temp/10));
				LCD_Cursor(3);
				LCD_WriteData(temp + '0');
			}
			else if ((temp/10) != 0){
				LCD_Cursor(1);
				LCD_WriteData((temp/10) + '0');
				temp -= (10*(temp/10));
				LCD_Cursor(2);
				LCD_WriteData(temp + '0');
			}
			else {
				LCD_Cursor(1);
				LCD_WriteData(temp + '0');
			}
			prevData = data;
			state = Display;
			break;
			
		default:
			state = Display;
			break;
	}
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	SPI_ServantInit();
	LCD_init();
	TimerSet(100);
	TimerOn();
	
	state = Display;
	prevData = recievedData;
	
    while (1) 
    {
		display_tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

