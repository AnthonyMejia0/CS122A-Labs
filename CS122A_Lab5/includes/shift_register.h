#include <avr/io.h>
#include "timer.h"

#define SHIFT_DDR DDRB
#define SHIFT_PORT PORTB
#define DATA 0
#define SRCLCK 2
#define RCLCK1 1
#define RCLCK2 3

void transmit_data(unsigned char data, int reg_num)
{
	for (unsigned char i = 0; i < 8; i++)
	{	
			if (data & 0x80)
			{
				SHIFT_PORT = SHIFT_PORT | (0x01<<DATA);
			}
			else
			{
				SHIFT_PORT = SHIFT_PORT & ~(0x01<<DATA);
			}

		data = data << 1;
		// clock pin high
		SHIFT_PORT = SHIFT_PORT | (0x0001<<SRCLCK);
		// clock pin low
		SHIFT_PORT = SHIFT_PORT & ~(0x0001<<SRCLCK);
	}
	
	if (reg_num == 1){
		// load pin high
		SHIFT_PORT = SHIFT_PORT | (0x0001<<RCLCK1);
		// load pin low
		SHIFT_PORT = SHIFT_PORT & ~(0x0001<<RCLCK1);
	}
	else if (reg_num == 2){
		// load pin high
		SHIFT_PORT = SHIFT_PORT | (0x0001<<RCLCK2);
		// load pin low
		SHIFT_PORT = SHIFT_PORT & ~(0x0001<<RCLCK2);
	}
}