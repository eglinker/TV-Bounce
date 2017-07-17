/* main.c  -   TV Bounce, (C) Erich Glinker, July, 2017
 *
 * This is the main TV bounce game source file.
 * TV bounce is basicaly a rip off of the 1970s Supper
 * Breakout game. 
 *
 * This file is released undern the the GNU General 
 * Public License, see:
 *
 *    http://gnu.org/licenses/licenses.html for details.
 *
 * To build the game binary file you need to have
 * the following files, it can be compiled using avr-gcc:
 *
 *	main.c
 *	amvtg.C
 *	amvtg.h
 *	vgafont.h
 *
 * Type the lines below to compile, link and produce the .hex binary.
 *
 * avr-gcc.exe -g -Os -mmcu=atmega328p -c amvtg.c     -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
 * avr-gcc.exe -g -Os -mmcu=atmega328p -c main.c      -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
 * avr-gcc.exe -g -mmcu=atmega328p -o demo.elf amvtg.o main.o
 * avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex 
 *
 * Type the lines below to upload to board using avrdude.exe
 *
 * avrdude.exe -C "C:\avrdude\avrdude.conf" -p atmega328p -c arduino -P COM3 -b 115200 -U   flash:w:demo.hex
 *
 *
 * This file uses course code from the AMVTG project - ATmega NTSC
 * Video Text Generator to generate video out, written 
 * by Stian Soreng, http://jmp.no/atvideotext/
 *
 * (C)2012 released under the GNU General Public License, see
 *         http://gnu.org/licenses/licenses.html for details.
 *
 */

#include <avr/io.h>
#define 	F_CPU   1000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "amvtg.h"
#include <stdlib.h>
#include <string.h>


/* Ball motion data structure. */
typedef struct
{
   int x,y; /* Position */
   int vx,vy;  /* Velocity */
} ball;



int moveBall(ball *ballPtr, int *score)
{
	char eye = ' ';
	
	text[(int)ballPtr->y][(int)ballPtr->x] = ' ';
	
	// X-axis motion
	ballPtr->x += ballPtr->vx;
	// Y-axis motion
	ballPtr->y += ballPtr->vy;
	
	
	// bounce
	if(ballPtr->x > XSIZE-3){
		ballPtr->vx = -ballPtr->vx;
		ballPtr->x += ballPtr->vx;
	} else if(ballPtr->x < 1){
		ballPtr->vx = -ballPtr->vx;
		ballPtr->x += ballPtr->vx;
	}
	
	// bounce
	if(ballPtr->y > YSIZE-2){
		ballPtr->vy = -ballPtr->vy;
		ballPtr->y += ballPtr->vy;
	} else if(ballPtr->y < YTOP + 1){
		ballPtr->vy = -ballPtr->vy;
		ballPtr->y += ballPtr->vy;
	}
	
	if(ballPtr->y <= YTOP) ballPtr->y = YTOP+1;
	
	eye = text[(int)ballPtr->y][(int)ballPtr->x];
	if(eye==BOXNARROWCHAR || eye=='$' || eye == '@'  || eye == PADDLECHAR) // distroy what it touches.
	{
		if(ballPtr->y>YTOP) text[(int)ballPtr->y][(int)ballPtr->x] = ' ';
		ballPtr->vy = -ballPtr->vy;
		ballPtr->y += ballPtr->vy;
		(*score)++;
	}
	
	if(ballPtr->y <= YTOP) ballPtr->y = YTOP+1;
		
	text[(int)ballPtr->y][(int)ballPtr->x] = BALLCHAR;
	return(0);
}

int screenSetup()
{
	int i,j;
	
	for(i=1; i<YSIZE-1; i++)
	{
		for(j=1; j<XSIZE-1; j++) text[i][j] = ' '; /* Clear horizontal line. */
		text[i][0] = VRIGHTBARCHAR; /* Draw verticle bars. */
		text[i][35] = VLEFTBARCHAR;
	}
	
	text[YSIZE-1][35] = VLEFTBARUPCHAR; /* Draw corners. */
	text[0][35] = VLEFTBARDOWNCHAR;
	text[YSIZE-1][0] = VRIGHTBARUPCHAR;
	text[0][0] = VRIGHTBARDOWNCHAR;
	
	printStringXY(2, 1, "TV Bounce !!!"); /* Text in title block. */
	printStringXY(2, 2, "score:");


	for(i=1; i<XSIZE-2; i++) /* Draw horizontal bars. */
	{
		text[0][i] = HBARCHAR;
		text[16][i] = HBARCHAR;
		text[YTOP][i] = HBARCHAR;
	}

	for(i=1; i<XSIZE-2; i++) /* Draw blocks to be broken */
	{
		if(i>14){
			text[YTOP+3][i] = BOXNARROWCHAR;
			text[YTOP+4][i] = BOXNARROWCHAR;
		}
		if(i<26){
			text[YTOP+7][i] = BOXNARROWCHAR;
			text[YTOP+8][i] = BOXNARROWCHAR;
			text[YTOP+8][i] = BOXNARROWCHAR;
		}
	}
	return(0);
}

/* Update paddle position and redraw paddle. */ 
int movePaddle(int *paddleX)
{
	/* Erase paddle */
	text[(int)YSIZE-2][(*paddleX)] = ' ';
	text[(int)YSIZE-2][(*paddleX)+1] = ' ';
	text[(int)YSIZE-2][(*paddleX)+2] = ' ';
	text[(int)YSIZE-2][(*paddleX)+3] = ' ';
	text[(int)YSIZE-2][(*paddleX)+4] = ' ';
	text[(int)YSIZE-2][(*paddleX)+5] = ' ';
	
	/* Move the paddle left or right. */
	if(left()){
		(*paddleX)--;
	}
	if(right()){ 
		(*paddleX)++;
	}
	
	/* Make sure that paddle does not go off screen. */	
	if((*paddleX)<1) 
	{
		(*paddleX) = 1;
	}
	if((*paddleX)>XSIZE-8)
	{
		(*paddleX) = XSIZE-8;
	}
	
	/* Draw the paddle. */
	printCharXY((*paddleX)+0, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+1, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+2, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+3, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+4, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+5, (int)YSIZE-2, PADDLECHAR);

	return(1);
}

/* Automatical update paddle position and redraw paddle during demo. */
int autoMovePaddle(int *paddleX, ball *ballPtr )
{
	/* Erase paddle */
	text[(int)YSIZE-2][(*paddleX)] = ' ';
	text[(int)YSIZE-2][(*paddleX)+1] = ' ';
	text[(int)YSIZE-2][(*paddleX)+2] = ' ';
	text[(int)YSIZE-2][(*paddleX)+3] = ' ';
	text[(int)YSIZE-2][(*paddleX)+4] = ' ';
	text[(int)YSIZE-2][(*paddleX)+5] = ' ';
	
	/* Move the paddle left or right. */
	*paddleX = ballPtr->x-3; 
	
	/* Make sure that paddle does not go off screen. */	
	if((*paddleX)<1) 
	{
		(*paddleX) = 1;
	}
	if((*paddleX)>XSIZE-8)
	{
		(*paddleX) = XSIZE-8;
	}
	
	/* Draw the paddle. */
	printCharXY((*paddleX)+0, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+1, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+2, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+3, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+4, (int)YSIZE-2, PADDLECHAR);
	printCharXY((*paddleX)+5, (int)YSIZE-2, PADDLECHAR);

	return(1);
}




int main() {
	char buffer[20]; /* Text buffer for temp. use. */
	int paddleX = 18; /* Paddle position. */ 
	int score = 0; /* Player score. */

	ball ball0 = {6, 6, 1, 1};
	
	/* Do everything to start up video. */
	initVideo();

	/* Setup screen. */	
	screenSetup();	
	
	
	while(1){
	
		/* Draw start screen and show demo. */
		printStringXY(2, 1, "TV Bounce !!!");
		
		/* Setup screen. */	
		screenSetup();
		
		printStringXY(17, 1, "   Demo Mode");
		
		/* Initial conditions */
		score = 0;
		paddleX = 18; /* Initial paddle position. */
		ball0.y = 6;  /* Initial ball position. */
		ball0.x = 6; 
		ball0.vy = 1;
		ball0.vx = 1;
		
		/* Demo loop, loops here for the entir demo or */ 
		/* or until a button is pressed to start game. */
		for( ;; )
		{
			_delay_us(70000);
		
			if(ball0.y<(int)YSIZE-2)
			{
				moveBall(&ball0, &score);
					
				autoMovePaddle(&paddleX, &ball0);
			
				_delay_us(70000);
			
				autoMovePaddle(&paddleX, &ball0);
		
				itoa(score, buffer,10);
			
				printStringXY(11, 2, buffer);
				printStringXY(2, 2, "score:");
				
				if(left()||right()) break;
		
			}
			else
			{				
				break;
			}
		}
		
		/* Setup screen. */	
		screenSetup();
		
		score = 0;
		printStringXY(17, 1, "   Play Mode");
		
		/* Initial conditions */
		paddleX = 18;
		ball0.y = 6;
		ball0.x = 6; 
		ball0.vy = 1;
		ball0.vx = 1;
		
		/* Player game loop. */
		for( ;; )
		{
			_delay_us(100000);
		
			if(ball0.y<(int)YSIZE-2)
			{
				moveBall(&ball0, &score);
					
				movePaddle(&paddleX);
			
				_delay_us(100000);
			
				movePaddle(&paddleX);
		
				itoa(score, buffer,10);
			
				printStringXY(11, 2, buffer);
		
			}
			else
			{				
				break;
			}
		}
		
		printStringXY(17, 1, "!!! GAME OVER !!!");
		
		_delay_ms(5000);
		
		
		/* Wait for key press befor going to demo. */
		
		for(;;){
			_delay_ms(100);
			
			if(left()||right()) break;
		}
		
		_delay_ms(1000);
		
	}
}
