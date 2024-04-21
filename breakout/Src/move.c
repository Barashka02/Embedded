
//----------------------------------------------------------------------------
// THIS IS THE FILE THAT TAKES CARE OF ALL THE MOVEMENT HAPPENING IN THE GAME
//----------------------------------------------------------------------------

#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/draw.h"
#include "../Inc/global.h"
#include <math.h>
#include <stdbool.h>

//------------------------------------------------------------------
// SETTING UP BALL 
//------------------------------------------------------------------

code unsigned char ball[] = {0x0e, 0x1f, 0x1f, 0x1f, 0x0e};				//array of the ball shape (to draw)

/**
 * Draw the ball on the screen and handle wall collisions.
 *
 * @param x The x-coordinate of the ball's center.
 * @param y The y-coordinate of the ball's center.
 * @return int Non-zero if the ball hits any screen edges or zero otherwise.
 */
char draw_ball(int x, int y) {
    unsigned char row, col, shift, j, hit;
    int i;

    // Check for boundary collisions and reverse velocity if necessary.
    if (x < 4 && x_vel < 0) {  // Left wall collision
        x_vel = -x_vel;
        RCAP4H = -2354 >> 8;   // D note
        RCAP4L = -2354;
        duration = 40;
        T4CON = T4CON^0x04;
        return 0;
    } else if (x > 78 && x_vel > 0) {  // Right wall collision
        x_vel = -x_vel;
        RCAP4H = -2354 >> 8;	// D note
        RCAP4L = -2354;
        duration = 40;
        T4CON = T4CON^0x04;
        return 0;
    } else if (y < 5 && y_vel < 0) {  // Top wall collision
        y_vel = -y_vel;
        RCAP4H = -2354 >> 8;   // D note
        RCAP4L = -2354;
        duration = 40;
        T4CON = T4CON^0x04;
        return 0;
    } else if (y > 62) {  // Bottom boundary, game over scenario
        RCAP4H = -1868 >> 8;   // F# note
        RCAP4L = -1868;
        duration = 300;	       // Longer duration to show current game over
        T4CON = T4CON^0x04;
        disp_end_game();
        return 0;
    }
    
    // Calculate position for drawing the ball.
    col = x - 2;
    row = y - 2;
    shift = row % 8;
    row = row / 8;
    hit = 0;

    // Draw ball on the screen, managing potential overflow.
    for (j = 0, i = row * 128 + col; j < 5; i++, j++) {
        int mask = (int)ball[j] << shift;
        hit |= screen[i] & (unsigned char)mask;
        hit_location = i;
        screen[i] |= mask;

        if (mask & 0xff00) {
            hit |= screen[i + 128] & (unsigned char)(mask >> 8);
            hit_location = i + 128;
            screen[i + 128] |= (unsigned char)(mask >> 8);
        }
    }
    return hit;
}

/**
 * Update ball position based on velocity and handle brick and paddle collisions.
 */
void move_ball() {
    char hit_code;    
    int new_x, new_y, j, k;

    // Check if it's time to reset bricks for either player.
    if (cur_player == 1) {
        if (score1 % 65 == 0 && y_pos > 30) {	//redraw the bricks if all bricks have dissapeared for player 1
            for (j = 0; j < 13; j++) {
                for (k = 0; k < 5; k++) {
                    p1_bricks[j][k] = 1;  // Reset each brick to 1
                }
            }
        }
    } else {
        if (score2 % 65 == 0 && y_pos > 30) {	//redraw the bricks if all bricks have dissapeared for player 2
            for (j = 0; j < 13; j++) {
                for (k = 0; k < 5; k++) {
                    p2_bricks[j][k] = 1;  // Reset each brick to 1
                }
            }
        }
    }

    // Calculate new position based on current velocity.
    new_x = x_pos + x_vel;
    new_y = y_pos + y_vel;

    hit_code = draw_ball(new_x, new_y);
    if (run == 1) {
        x_pos = new_x;
        y_pos = new_y;
    }

    // Handle paddle bounce logic.
    if (y_pos == 60 && hit_code > 0) {
        char col = x_pos - pot_avg - 2;
        int div; 

        RCAP4H = -3142 >> 8;		//A note
        RCAP4L = -3142;
        duration = 25;
        T4CON = T4CON^0x04;

		 // Determine the section of the paddle impacted based on the current player's paddle size.
		if (cur_player == 1) {
		    div = paddle_size_1 / 4; // Divide Player 1's paddle into four segments.
		} else {
		    div = paddle_size_2 / 4; // Divide Player 2's paddle into four segments.
		}

		// Determine the bounce behavior based on the ball's collision point on the paddle.
		if (col < div || col > 3 * div) { // Hits outer edges of the paddle
		    // Steeper angle bounce on outer segments
		    x_vel = 2;  // Increase horizontal velocity
		    y_vel = -1; // Decrease vertical velocity
		} else { // Hits inner segments of the paddle
		    // Shallower angle bounce on inner segments
		    x_vel = 1;  // Decrease horizontal velocity
		    y_vel = -2; // Increase vertical velocity
		}
		// Reverse direction if ball hits the left half of the paddle
		if (col < div * 2) {
		    x_vel = -1 * x_vel;
		}
   } else if (hit_code > 0 && y_pos < 30 && y_pos > 4) {  
        int x_index = ((x_pos-3) / 6); // Column of bricks
        int y_index = ((y_pos-10) / 4); // Row of bricks
        
        if (cur_player == 1) {
            if (p1_bricks[x_index][y_index] == 1) { // Check if the brick is active
                p1_bricks[x_index][y_index] = 0; // Deactivate the brick
                RCAP4H = -1571 >> 8;
                RCAP4L = -1571;
                duration = 50;
                T4CON = T4CON^0x04;
                y_vel = -y_vel; // Reverse the velocity
                score1++;
                if (y_index <= 2) {
                    speed = 40;  // Increase game speed
                }
            }
        } else {
            if (p2_bricks[x_index][y_index] == 1) { // Check if the brick is active
                p2_bricks[x_index][y_index] = 0; // Deactivate the brick
                RCAP4H = -1571 >> 8;
                RCAP4L = -1571;
                duration = 50;
                T4CON = T4CON^0x04;
                y_vel = -y_vel; // Reverse the velocity
                score2++;
                if (y_index <= 2) {
                    speed = 40;  // Increase game speed
                }
            }
        }
    }
}
