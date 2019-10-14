/*
 * lab3_part1.c
 *
 * Created: 10/9/2019 7:01:23 PM
 * Author : Antho
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		PORTA = 0xFF;
    }
}

