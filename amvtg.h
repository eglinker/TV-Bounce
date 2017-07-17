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
 * A few extra functions and macros are added to
 * simplify push buttons, printing to screen and 
 * placing characters and sprites.
 */

#define HSYNC PORTB=0;
#define BLACK PORTB=1;
#define COLUMN(r) SPDR=pgm_read_byte(&font[(uint8_t)text[textrow][r]][fontline]);

#define XSIZE	37  // Screen dimensions.
#define YSIZE	17  
#define FONTHEIGHT	12 // Font height in Pixels. 

#define YTOP	3 // Vertical positon of play field top.


// Custom sprites codes defined.
#define BOXCHAR		128
#define BOXRIGHTCHAR	130	
#define BOXNARROWCHAR	131
#define BALLCHAR	129
#define PADDLECHAR	132
#define HBARCHAR	133
#define VRIGHTBARCHAR	135
#define VLEFTBARCHAR	137
#define VLEFTBARUPCHAR	139
#define VLEFTBARDOWNCHAR	141
#define VRIGHTBARDOWNCHAR	143
#define VRIGHTBARUPCHAR	145

/* Text display area. */
extern char text[17][37];

/* Timers and PWM initialization */
void timer_init(void);

/*
 * The extra functions and macros below are
 * there to make printing onto the screen,
 * place single char/sprirte and detect pushbuttions.
 */

/* Setup video. */
void initVideo(void);

/* Print text to screen. */
#define   printStringXY(X, Y, S)   strcpy(&text[Y][X], S);

/* Print char/sprite to screen. */
#define   printCharXY(X, Y, S) text[(int)Y][(int)X] = S;

/* Detect button functions */
int left(void);
int right(void);
int up(void);
int down(void);
