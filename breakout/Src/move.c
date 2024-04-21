#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/draw.h"
#include "../Inc/global.h"
#include <math.h>
#include <stdbool.h>

#define BRICK_ROWS 11
#define BRICK_COLS 5
#define BRICK_WIDTH 7
#define BRICK_HEIGHT 4
#define TOP_OFFSET 10 // Assuming bricks start 10 pixels from the top
#define LEFT_OFFSET 3 // Assuming bricks start 3 pixels from the left side
#define BRICK_ROWS 11
#define BRICK_COLS 5
#define BRICK_WIDTH 7
#define BRICK_HEIGHT 4
#define TOP_OFFSET 10 // Assuming bricks start 10 pixels from the top
#define LEFT_OFFSET 3 // Assuming bricks start 3 pixels from the left side



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

	if(x < 4 && x_vel < 0) // The ball has hit the right or left wall
	{
		x_vel = -x_vel;

		RCAP4H = -2354 >> 8;
		RCAP4L = -2354;
		duration = 40;
		T4CON = T4CON^0x04;
		return 0;
	}
	else if(x > 78 && x_vel > 0) 
	{
		x_vel = -x_vel;

		RCAP4H = -2354 >> 8;
		RCAP4L = -2354;
		duration = 40;
		T4CON = T4CON^0x04;
		return 0;
	}
	else if(y < 5 && y_vel < 0) 	//The ball has hit the top wall
	{
		y_vel = -y_vel;

		RCAP4H = -2354 >> 8;
		RCAP4L = -2354;
		duration = 40;
		T4CON = T4CON^0x04;
		return 0;
	}
	else if(y > 62) //The ball has gone off the botton 
	{ 
	//	if(duration == 0)
	//	{
	//		while(duration){};
	
		RCAP4H = -1868>> 8;
		RCAP4L = -1868;
		duration = 300;
		T4CON = T4CON^0x04;
		disp_end_game();
		return 0;
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
	int j, k;

	if(cur_player == 1)
	{
		if(score1 % 65 == 0 && y_pos > 30)
		{
				for (j = 0; j < 13; j++) {
	        		for (k = 0; k < 5; k++) {
	            		p1_bricks[j][k] = 1;  // Reset each brick to 1
	        	}
	    	}
		}
	}	
	else
	{
		if(score2 % 65 == 0 && y_pos > 30)
		{
			for (j = 0; j < 13; j++) {
        		for (k = 0; k < 5; k++) {
            		p2_bricks[j][k] = 1;  // Reset each brick to 1
        		}
    		}
		}
	}
	


	//setting new values of x and y;
	new_x = x_pos + x_vel;
	new_y = y_pos + y_vel;

	hit_code = draw_ball(new_x, new_y);
	if(run == 1)
	{
		x_pos = new_x;
		y_pos = new_y;
	}

	//code to bounch off paddle
	if(y_pos == 60 && hit_code > 0)
	{

		char col = x_pos - pot_avg -2;
		int div; 

		RCAP4H = -3142 >> 8;
		RCAP4L = -3142;
		duration = 25;
		T4CON = T4CON^0x04;

		if(cur_player == 1)
		{
			div = paddle_size_1 / 4;
		}
		else
		{
			div = paddle_size_2 / 4;
		}	
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
/**
    if (hit_code && y_pos < 30) { // Bricks are assumed to be within y < 30
        int x_index = (x_pos - 3) / 6; // Calculate which column of bricks
        int y_index = (y_pos - 10) / 4; // Calculate which row of bricks
		
		if(cur_player == 1)
		{
	        if (p1_bricks[x_index][y_index] == 1) { // Check if the brick is active
	            char collision_type = get_collision_type(x_pos, y_pos, x_index * 6 + 3, y_index * 4 + 10);
	            if (collision_type == 'L' || collision_type == 'R') 
				{
	                x_vel = -x_vel; // Reverse horizontal velocity for side hits
	            } 
				else 
				{
	                y_vel = -y_vel; // Reverse vertical velocity for top/bottom hits
	            }
	            p1_bricks[x_index][y_index] = 0; // Deactivate the brick
				score1++;
	        }
		else{
			 if (p2_bricks[x_index][y_index] == 1) { // Check if the brick is active
		            char collision_type = get_collision_type(x_pos, y_pos, x_index * 6 + 3, y_index * 4 + 10);
		            if (collision_type == 'L' || collision_type == 'R')
					{
		                x_vel = -x_vel; // Reverse horizontal velocity for side hits

		            }
					else 
					{
		                y_vel = -y_vel; // Reverse vertical velocity for top/bottom hits
		            }
		            p2_bricks[x_index][y_index] = 0; // Deactivate the brick
					score2++;
		        }
			}
		}		
   } 
**/
	else if (hit_code > 0 && y_pos < 30 && y_pos > 4) {

	    int x_index = ((x_pos-3) / 6); // Calculate which column of bricks the ball is in
	    int y_index = ((y_pos-10) / 4); // Calculate which row of bricks the ball is in
		
		if(cur_player == 1)
		{
		    if (p1_bricks[x_index][y_index] == 1) { // Check if the brick is active
		        p1_bricks[x_index][y_index] = 0; // Deactivate the brick

					RCAP4H = -1571>> 8;
					RCAP4L = -1571;
					duration = 50;
					T4CON = T4CON^0x04;
					  // Reset each brick to 1
		        y_vel = -y_vel; // Reverse the velocity
				//x_vel = -x_vel;
				score1++;
				if(y_index <= 2)
				{
					speed = 40;
				}
		    }
		}
		else
		{
		    if (p2_bricks[x_index][y_index] == 1) { // Check if the brick is active
		        p2_bricks[x_index][y_index] = 0; // Deactivate the brick
				
			
				RCAP4H = -1571>> 8;
				RCAP4L = -1571;
				duration = 50;
				T4CON = T4CON^0x04;
		        y_vel = -y_vel; // Reverse the velocity
				//x_vel = -x_vel;
				score2++;
				if(y_index <= 2)
				{
					speed = 40;
				}
		    }
		}
	}
}
