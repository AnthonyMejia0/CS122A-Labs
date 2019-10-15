#ifndef SPI_H
#define SPI_H

#include <avr/interrupt.h>
#include "bit.h"

unsigned char recievedData;

void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	DDRB = 0xBF; PORTB = 0x40;
	
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	//   of fosc/16  (pg. 168)
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}

void SPI_MasterTransmit(unsigned char cData) {
	// data in SPDR will be transmitted, e.g. SPDR = cData
	// set SS low
	PORTB = SetBit(PORTB, 4, 0);
	
	SPDR = cData;
	
	while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		;
	}
	
	// set SS high
	PORTB = SetBit(PORTB, 4, 1);
}

void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = 0x40; PORTB = 0xBF;
	
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= (1<<SPE) | (1<<SPIE);
	
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}

ISR(SPI_STC_vect) { // this is enabled in with the SPCR register's "SPI
					// Interrupt Enable"
	// SPDR contains the received data, e.g. unsigned char receivedData =
	// SPDR;
	recievedData = SPDR;
}

#endif //SPI_H