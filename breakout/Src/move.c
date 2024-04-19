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
/**
char get_collision_type(int ball_x, int ball_y, int brick_x, int brick_y) {
    // Approximate the ball as a point at its center for simplicity
//    int ball_center_x = ball_x + 2; // middle of ball assuming width 5
//    int ball_center_y = ball_y + 2; // middle of ball assuming height 5

    int ball_center_x = ball_x; // middle of ball assuming width 5
    int ball_center_y = ball_y; // middle of ball assuming height 5

    // Calculate relative positions
    int relative_x = ball_center_x - (brick_x + 5 / 2);
    int relative_y = ball_center_y - (brick_y + 3 / 2);

    // Determine the type of collision based on the relative positions
    if (abs(relative_x) > abs(relative_y)) {
        return (relative_x > 0) ? 'L' : 'R'; // Left or Right side hit
    } else {
        return (relative_y > 0) ? 'T' : 'B'; // Top or Bottom hit
    }
}
**/

void adjust_ball_velocity(int brick_x, int brick_y, int brick_width, int brick_height) {
    int ball_center_x = x_pos + 2; // Assuming the ball's width is about 5 pixels, adjust accordingly
    int ball_center_y = y_pos + 2; // Assuming the ball's height is about 5 pixels

    // Determine the center of the brick
    int brick_center_x = brick_x + brick_width / 2;
    int brick_center_y = brick_y + brick_height / 2;

    // Calculate differences
    int dx = ball_center_x - brick_center_x;
    int dy = ball_center_y - brick_center_y;

    if (abs(dy) >= abs(dx)) {
        y_vel = -y_vel; // Top or bottom hit
    } else {
        x_vel = -x_vel; // Side hit
    }
}

void process_collision(int x_index, int y_index, char bricks[][5], int* score) {
    int brick_width = 6;
    int brick_height = 4;
    int brick_x = x_index * brick_width + 3; // Adjust +3 if your coordinate system requires
    int brick_y = y_index * brick_height + 10; // Adjust +10 similarly

    if (bricks[x_index][y_index] == 1) { // Check if the brick is active
        bricks[x_index][y_index] = 0; // Deactivate the brick
        adjust_ball_velocity(brick_x, brick_y, brick_width, brick_height);
        (*score)++;
        if (y_index <= 2) {
            speed = 40; // Increase game speed
        }
    }
}

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
		return 0;
	}
	else if(x > 78 && x_vel > 0) 
	{
		x_vel = -x_vel;
		return 0;
	}
	else if(y < 5 && y_vel < 0) 	//The ball has hit the top wall
	{
		y_vel = -y_vel;
		return 0;
	}
	else if(y > 62) //The ball has gone off the botton 
	{
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
		if(score1 == 65 && y_pos > 35)
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
		if(score2 == 65 && y_pos > 35)
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
		        }
			}
		}		
   } 
   **/
   /**  Best one so far!!!!!
	else if (hit_code > 0 && y_pos < 30 && y_pos > 4) {
	    int x_index = ((x_pos-3) / 6); // Calculate which column of bricks the ball is in
	    int y_index = ((y_pos-10) / 4); // Calculate which row of bricks the ball is in
		if(cur_player == 1)
		{
		    if (p1_bricks[x_index][y_index] == 1) { // Check if the brick is active
		        p1_bricks[x_index][y_index] = 0; // Deactivate the brick
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
**/

	if (hit_code > 0 && y_pos < 30 && y_pos > 4) {
	    int x_index = ((x_pos-3) / 6);
	    int y_index = ((y_pos-10) / 4);
	    if(cur_player == 1) {
	        process_collision(x_index, y_index, p1_bricks, &score1);
	    } else {
	        process_collision(x_index, y_index, p2_bricks, &score2);
	    }
	}
}

