#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/draw.h"
#include "../Inc/global.h"
#include <math.h>
#include <stdbool.h>
//------------------------------------------------------------------
//Helper Functions
//------------------------------------------------------------------
// Function returns the index of the brick that was hit, or -1 if no collision occurred.

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

	if(x <= 4 || x >= 78) // The ball has hit the right or left wall
	{
		x_vel = -x_vel;
	}
	else if(y <= 4) 	//The ball has hit the top wall
	{
		y_vel = -y_vel;
	}
	else if(y >= 61) //The ball has gone off the botton 
	{
		disp_end_game();
	}
	
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
		hit_location = i;
		screen[i] |= mask;

		//checking and writing the ball (overflow on page)
		if(mask & 0xff00)
		{
			hit |= screen[i+128]&(unsigned char)(mask>>8);
			hit_location = i+128;
			screen[i+128] |= (unsigned char)(mask >> 8);
		}
		
		}
		return hit;
}


void move_ball()
{	
	char hit_code;	
	int new_x, new_y;
    int brick_width = 6;
    int brick_height = 3;

	//setting new values of x and y;
	new_x = x_pos + x_vel;
	new_y = y_pos + y_vel;

	hit_code = draw_ball(new_x, new_y);

	x_pos = new_x;
	y_pos = new_y;

	//code to bounch off paddle
	if(y_pos == 60 && hit_code > 0)
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

      
	if (hit_code > 0 && y_pos < 30) {
	    int x_index = ((x_pos-3) / 6); // Calculate which column of bricks the ball is in
	    int y_index = ((y_pos-10) / 4); // Calculate which row of bricks the ball is in

	    if (bricks[x_index][y_index] == 1) { // Check if the brick is active
	        bricks[x_index][y_index] = 0; // Deactivate the brick
	        y_vel = -y_vel; // Reverse the velocity
			if(cur_player == 1)
			{
				score1++;
			}
			else
			{
				score2++;
			}
			if(y_index <= 3)
			{
				speed = 40;
			}
	    }
	}
}