/*
 * AMVTG - ATmega NTSC Video Text Generator
 * Written by Stian Soreng, http://jmp.no/atvideotext/
 *
 * (C)2012 released under the GNU General Public License, see
 *         http://gnu.org/licenses/licenses.html for details.
 *
 * Tested on Philips 32" LCD
 *           Zate 2.5" mini LCD
 *
 * Modified by Erich Glinker(eglinker@gmail.com), July 2017,
 * so that it can be linked against other C sourse
 * files in other projects that need video output.
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "vgafont.h"
#include "amvtg.h"

#include <stdlib.h>
#include <string.h>


/* This is our text buffer.  Populate this as you please. */

//char *text[17] =
//	{
//		"************************************",
//		"*                                  *",
//		"* ATmega NTSC Video Text Generator *",
//		"* Version 0.9                      *",
//		"*                                  *",
//		"* For more info please visit       *",
//		"* http://jmp.no/amvtg/             *",
//		"*                                  *",
//		"************************************",
//		"                                   *",
//		"                                   *",
//		"                                   *",
//		"                                   *",
//		"                                   *",
//		"                                   *",
//		"                                   *",
//		"                               Cool!",
//	};

char text[17][37];


/* Timers and PWM initialization */

void timer_init(void){
	DDRB = 0b11111111;
//	DDRB = 0b00000101;
	TCCR1B |= (1<<WGM12);
//	TIMSk |= (1<<OCIE1A);
	TIMSK1 |= (1<<OCIE1A);
//	OCR1A = 64*16;
	OCR1A = 64*16 - 15;
	TCCR1B |= (1<<CS10);
	SPSR = (1<<SPI2X);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA);
}

/* Timer interrupt */

ISR (TIMER1_COMPA_vect) {
	static uint16_t rasterline=0;
	static uint8_t textrow = 0;
	static uint8_t fontline = 0;

	HSYNC;

	if( rasterline > 31 && rasterline <= (30+((FONTHEIGHT+1)*(208/FONTHEIGHT))) ) {
	//if( rasterline > 31 && rasterline <= (30+((FONTHEIGHT+0)*(208/FONTHEIGHT))) ) {
		_delay_us(5);
		BLACK;
		_delay_us(8);
		COLUMN(0);
		COLUMN(1);
		COLUMN(2);
		COLUMN(3);
		COLUMN(4);
		COLUMN(5);
		COLUMN(6);
		COLUMN(7);
		COLUMN(8);
		COLUMN(9);
		COLUMN(10);
		COLUMN(11);
		COLUMN(12);
		COLUMN(13);
		COLUMN(14);
		COLUMN(15);
		COLUMN(16);
		COLUMN(17);
		COLUMN(18);
		COLUMN(19);
		COLUMN(20);
		COLUMN(21);
		COLUMN(22);
		COLUMN(23);
		COLUMN(24);
		COLUMN(25);
		COLUMN(26);
		COLUMN(27);
		COLUMN(28);
		COLUMN(29);
		COLUMN(30);
		COLUMN(31);
		COLUMN(32);
		COLUMN(33);
		COLUMN(34);
		COLUMN(35);
		COLUMN(36);
		while(!(SPSR&(1<<SPIF)));
		BLACK;
	} else if( rasterline >= 0 && rasterline < 5 ) {
		_delay_us(27);
		BLACK;
		_delay_us(5);
		HSYNC;
		_delay_us(27);
		BLACK;
	} else if( (rasterline >= 5 && rasterline <= 10) ) {
		_delay_us(2);
		BLACK;
		_delay_us(28);
		HSYNC;
		_delay_us(2);
		BLACK;
	} else if( rasterline > 10 && rasterline <= 31 ) {
		_delay_us(4);
		BLACK;
		textrow = 0;
		fontline = -1;
	} else if( rasterline >262-6 ) {
		_delay_us(2);
		BLACK;
		_delay_us(28);
		HSYNC;
		_delay_us(2);
		BLACK;
	} else {
		_delay_us(5);
		BLACK;
	}

	rasterline++;
	if( rasterline > 262 )
		rasterline = 0;

	fontline++;
	if( fontline > FONTHEIGHT ) {
		fontline=0;
		textrow++;
	}
}

void initVideo(void)
{
	timer_init();
	sei();
	DDRC = 0x00;
	return;
}

int left(void)
{
	return(!(PIND & 4));
}

int right(void)
{
	return(!(PIND & 8));
}

int up(void)
{
	return(!(PIND & 16));
}

int down(void)
{
	return(!(PIND & 32));
}


