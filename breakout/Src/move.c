#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/draw.h"
#include "../Inc/global.h"


//------------------------------------------------------------------
//SETTING UP BALL 
//------------------------------------------------------------------

code unsigned char ball[] = {0x0e, 0x1f, 0x1f, 0x1f, 0x0e};
char draw_ball(int x, int y)
{	
	unsigned char row, col, shift, j, hit;
	int i;
	
	//checking for hits
	//if(x <= 4 || x >= 76 || y <= 2 || y >= 59) return 1;

	if(x <= 4 || x >= 76) return 1; // The ball has hit the right or left wall
	else if(y <= 2) return 2;		//The ball has hit the top wall
	else if(y >= 61) return 3; 	//The ball has gone off the botton 
	
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
		screen[i] |= mask;

		//checking and writing the ball (overflow on page)
		if(mask & 0xff00)
		{
			hit |= screen[i+128]&(unsigned char)(mask>>8);
			screen[i+128] |= (unsigned char)(mask >> 8);
		}
		
		}
		return hit;
}


void move_ball()
{	
	char hit_code;	
	int new_x, new_y;
	//setting new values of x and y;
	new_x = x_pos + x_vel;
	new_y = y_pos + y_vel;

	hit_code = draw_ball(new_x, new_y);
	
	x_pos = new_x;
	y_pos = new_y;
	
	//if the ball hits the walls
	if(hit_code == 1)  
	{
		x_vel = -1* x_vel;
	}

	//if the ball hits the top
	else if(hit_code == 2)
	{
		y_vel = -1* y_vel;

	}

	//if the ball goes out
	else if(hit_code == 3)
	{	
		
		ball_cnt = ball_cnt -1;  //remove 1 ball from player

		if(ball_cnt == 0)
		{
			if(player == 1)
			{
				player = 2;
			}
			else
			{
				player = 1;
			}
			disp_end_game();
		}
		
	}

	else if(y_pos == 60 && hit_code > 0)
	{
		char col = x_pos - pot_avg -2;
		int div = paddle_size / 4;
		if(col < div || col > 3 * div)
		{
			x_vel = 2;
			y_vel = -1;
		}

		else
		{
			x_vel = 1;
			y_vel = -2;
		}
		if(col < div * 2)
		{
			x_vel = -1 * x_vel;
		}
	}


	
}


	   // setting up timer 2
//   T2CON = 0x00;

   //RCAP2H = 0xF9;  // This overflows 512 times in 400 milisecond. 
   //RCAP2L = 0x40;


//------------------------------------------------------------------
//SETTING UP THE PADDLE
//------------------------------------------------------------------
