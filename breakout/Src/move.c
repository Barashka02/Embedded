#include "../Inc/lcd.h"
#include "../Inc/draw.h"


code unsigned char ball[] = {0x0e, 0x1f, 0x1f, 0x1f, 0x0e};
char draw_ball(int x, int y)
{	
	unsigned char row, col, shift, j, hit;
	int i;
	
	//checking for hits
	//if(x <= 4 || x >= 76 || y <= 2 || y >= 59) return 1;

	if(x <= 4 || x >= 76) return 1; // The ball has hit the right or left wall
	else if(y >= 59) return 2; 		//The ball has hit the top wall
	else if(y <= 2) return 3;		//The ball has gone off the botton 
	
	//shifting to measure top left of ball
	col = x-2;
	row = y-2;
	shift = row % 8;
	row = row / 8;
	hit = 0;

	//checking and writing the ball (no overflow on page)
	for(j=0, i=row*128+col; j<5; i++, j++)
	{
		int mask = (int)ball[j]<<shift;
		hit |= screen[i]&(unsigned char)mask;
		screen[i] = mask;

		//checking and writing the ball (overflow on page)
		if(mask & 0xff00)
		{
			hit |= screen[i+128]&(unsigned char)(mask>>8);
			screen[i+128] |= (unsigned char)(mask >> 8);
		}
		
		}
		return hit;
}

void move_ball(x_move, y_move)
{
	char hit=0;


}

		   // setting up timer 2
   T2CON = 0x00;

   //RCAP2H = 0xF9;  // This overflows 512 times in 400 milisecond. 
   //RCAP2L = 0x40;

