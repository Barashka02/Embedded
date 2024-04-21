
//------------------------------------------------------------------------------------
// THIS IS THE FILE THAT HAS ALL THE FUNCTIONS TO DRAW EVERYTHING NEEDED ON THE SCREEN
//------------------------------------------------------------------------------------

#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/global.h"
#include <math.h>
sbit BUTTON = P2^6;  // Define the button input using a specific bit in port 2


//---------------------------------------------------------
// SUPPORT FUNCTIONS
//---------------------------------------------------------

/**
 * Draw a score ball on the LCD at specified coordinates.
 *
 * @param x The x-coordinate center of the ball.
 * @param y The y-coordinate center of the ball.
 */
code unsigned char score_ball[] = {0x0e, 0x1f, 0x1f, 0x1f, 0x0e};
void draw_score_ball(int x, int y) {

	//initailizng variables
    unsigned char row, col, shift, j, hit;
    int i, mask;

    col = x - 2;
    row = y - 2;
    shift = row % 8;	
    row /= 8;

    for (j = 0, i = row * 128 + col; j < 5; i++, j++) {
        mask = (int)score_ball[j] << shift;
        screen[i] = mask;
    }
}

/**
 * Display a single character on the LCD at a specific row and column.
 *
 * @param row The row number on the LCD where the character will be displayed.
 * @param column The column number on the LCD where the character will be displayed.
 * @param character The ASCII character to be displayed.
 */
void disp_char(unsigned char row, unsigned char column, char character) {
    int i = 128 * row + column;
    int j = (character - 0x20) * 5;
	unsigned char k;
    for (k = 0; k < 5; ++k) {
        screen[i + k] |= font5x8[j + k];
    }
}

/**
 * Draw bricks on the screen based on the game's current state.
 */
void draw_bricks() {
    int i, j, k;
    for (i = 0; i < 13; i++) {							//13 bricks wide
        for (j = 0; j < 5; j++) {						//5  bricks tall
            int index = (j / 2) * 128 + i * 6 + 131;	//index for figuring out what index to draw bricks at (needed to draw 2 bricks per page)
            int mask = (j % 2 == 0) ? 0x07 : 0x70;		//make for drawing a brick on the top or botton of a page without erasing what's already there
			
            for (k = 0; k < 5; k++) {					//drawing the bricks
                if (cur_player == 1) {					//player 1 bricks states
                    if (p1_bricks[i][j] == 0) {			//erasing player 1 bricks
                        screen[index + k] &= ~mask;		
                    } else if (p1_bricks[i][j] == 1) {	//drawing player 1 bricks
                        screen[index + k] |= mask;
                    }
                } else {								//player 2 brick states
                    if (p2_bricks[i][j] == 0) {			//erasing player 2 bricks
                        screen[index + k] &= ~mask;
                    } else if (p2_bricks[i][j] == 1) {	//drawing player 2 bricks
                        screen[index + k] |= mask;
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------
// GLOBAL FUNCTIONS
//---------------------------------------------------------

/** DISCLAIMER: 
	When drawing values on the screen, i is used in every row position (i + #).  This was used for easy centering of values. 
	The "magic numbers" that i is set to gives the centered version of text on screen
**/


/**
 * Draw borders around the game area on the LCD.
 */
void draw_borders() {
	//initializtion 
	int i, j;
    for (i = 1; i < 82; i++) {
        screen[i] |= 0x03;				//drawing the top line
    }
    for (j = 0; j < 8; j++) {
        screen[j * 128] |= 0xff;		//drawing the left line
        screen[j * 128 + 1] |= 0xff;
        screen[j * 128 + 81] |= 0xff;	//drwaing the right line
        screen[j * 128 + 82] |= 0xff;
    }
}

/**
 * Display current and high scores on the LCD.
 */
void draw_scores() {
	//initialization 
    int i = 86;
	int thousands, hundreds, tens, ones;

    disp_char(0, i, '-');				//"-HIGH- high score
    disp_char(0, i + 7, 'H');
    disp_char(0, i + 14, 'I');
    disp_char(0, i + 21, 'G');
    disp_char(0, i + 28, 'H');
    disp_char(0, i + 35, '-');
	
    thousands = high_score / 1000;		//calculating the place values of the high score
    hundreds = (high_score / 100) % 10;
    tens = (high_score / 10) % 10;
    ones = high_score % 10;

    i = 93;								//writing the scores itself
    disp_char(1, i, '0' + thousands);
    disp_char(1, i + 7, '0' + hundreds);
    disp_char(1, i + 14, '0' + tens);
    disp_char(1, i + 21, '0' + ones);

    for (i = 81; i < 126; i++) {
        disp_char(2, i, '-');
    }

    i = 83;
    disp_char(3, i, '-');				//"-SCORE-" current score
    disp_char(3, i + 7, 'S');
    disp_char(3, i + 14, 'C');
    disp_char(3, i + 21, 'O');
    disp_char(3, i + 28, 'R');
    disp_char(3, i + 35, 'E');
    disp_char(3, i + 42, '-');

    if (cur_player == 1) {				//calculating the place values of the current score (player 1)
        thousands = score1 / 1000;
        hundreds = (score1 / 100) % 10;
        tens = (score1 / 10) % 10;
        ones = score1 % 10;
    } else {							//calculating the place values of the current score (player 2)
        thousands = score2 / 1000;
        hundreds = (score2 / 100) % 10;
        tens = (score2 / 10) % 10;
        ones = score2 % 10;
    }

    i = 93;
    disp_char(4, i, '0' + thousands);	//writing the score itself
    disp_char(4, i + 7, '0' + hundreds);
    disp_char(4, i + 14, '0' + tens);
    disp_char(4, i + 21, '0' + ones);

    for (i = 81; i < 126; i++) {		//line for clarity
        disp_char(5, i, '-');
    }

    i = 101;
    disp_char(6, i, 'P');				//displaying current player
    disp_char(6, i + 7, '0' + cur_player);

    i = 96;
    if (cur_player == 1) {				//displaying the amount of balls player 1 has left
        if (player_1_ball == 3) {		//3 balls left
            draw_score_ball(i, 60);
            draw_score_ball(i + 11, 60);
            draw_score_ball(i + 22, 60);
        } else if (player_1_ball == 2) {//2 balls left
            draw_score_ball(i, 60);
            draw_score_ball(i + 11, 60);
        } else if (player_1_ball == 1) {//1 ball left
            draw_score_ball(i, 60);
        }
    } else {							//diplaying the amount of balls player 2 has left
        if (player_2_ball == 3) {		//3 balls left
            draw_score_ball(i, 60);
            draw_score_ball(i + 11, 60);
            draw_score_ball(i + 22, 60);
        } else if (player_2_ball == 2) {//2 balls left
            draw_score_ball(i, 60);
            draw_score_ball(i + 11, 60);
        } else if (player_2_ball == 1) {//1 ball left
            draw_score_ball(i, 60);
        }
    }
}

/**
 * Manage the display of the game screen when a player is ready or the game starts.
 */
void display_player_ready() {
    int i = 20;
	
	//setup screen (draw everything)
    blank_screen();
    draw_borders();
    draw_scores();
    draw_borders();

	//single player mode
    if (cur_player == 1 && multi_player == 0) {

        disp_char(1, i, 'S');			//"SINGLE PLAYER READY"
        disp_char(1, i + 7, 'i');
        disp_char(1, i + 14, 'n');
        disp_char(1, i + 21, 'g');
        disp_char(1, i + 28, 'l');
        disp_char(1, i + 35, 'e');

        disp_char(2, i, 'P');
        disp_char(2, i + 7, 'l');
        disp_char(2, i + 14, 'a');
        disp_char(2, i + 21, 'y');
        disp_char(2, i + 28, 'e');
        disp_char(2, i + 35, 'r');

        disp_char(3, i, 'R');
        disp_char(3, i + 7, 'e');
        disp_char(3, i + 14, 'a');
        disp_char(3, i + 21, 'd');
        disp_char(3, i + 28, 'y');
	
        disp_char(6, i, 'p');			//"PRESS TO START"
        disp_char(6, i + 7, 'r');
        disp_char(6, i + 14, 'e');
        disp_char(6, i + 21, 's');
        disp_char(6, i + 28, 's');

        disp_char(6, i + 42, 't');
        disp_char(6, i + 49, 'o');

        disp_char(7, i + 7, 's');
        disp_char(7, i + 14, 't');
        disp_char(7, i + 21, 'a');
        disp_char(7, i + 28, 'r');
        disp_char(7, i + 35, 't');

        disp_char(5, 3, '|');			//Arrow pointing to start button
        disp_char(5, 5, '|');
        disp_char(6, 3, '|');
        disp_char(6, 5, '|');
        disp_char(7, 4, 'v');

	//double player mode
    } else if (cur_player == 1 && multi_player == 1) {
        disp_char(2, i, 'P');			//"PLAYER 1 READY"
        disp_char(2, i + 7, 'l');
        disp_char(2, i + 14, 'a');
        disp_char(2, i + 21, 'y');
        disp_char(2, i + 28, 'e');
        disp_char(2, i + 35, 'r');
        disp_char(2, i + 42, ' ');
        disp_char(2, i + 49, '1');

        disp_char(3, i + 7, 'R');
        disp_char(3, i + 14, 'e');
        disp_char(3, i + 21, 'a');
        disp_char(3, i + 28, 'd');
        disp_char(3, i + 35, 'y');

        disp_char(6, i, 'p');			//"PRESS TO START"
        disp_char(6, i + 7, 'r');
        disp_char(6, i + 14, 'e');
        disp_char(6, i + 21, 's');
        disp_char(6, i + 28, 's');

        disp_char(6, i + 42, 't');
        disp_char(6, i + 49, 'o');

        disp_char(7, i + 7, 's');
        disp_char(7, i + 14, 't');
        disp_char(7, i + 21, 'a');
        disp_char(7, i + 28, 'r');
        disp_char(7, i + 35, 't');

        disp_char(5, 3, '|');			//Arrow pointing to start button
        disp_char(5, 5, '|');
        disp_char(6, 3, '|');
        disp_char(6, 5, '|');
        disp_char(7, 4, 'v');
    } else {
        disp_char(3, i, 'P');			//"PLAYER 2 READY"
        disp_char(3, i + 7, 'l');
        disp_char(3, i + 14, 'a');
        disp_char(3, i + 21, 'y');
        disp_char(3, i + 28, 'e');
        disp_char(3, i + 35, 'r');
        disp_char(3, i + 42, ' ');
        disp_char(3, i + 49, '2');

        disp_char(5, i + 7, 'R');
        disp_char(5, i + 14, 'e');
        disp_char(5, i + 21, 'a');
        disp_char(5, i + 28, 'd');
        disp_char(5, i + 35, 'y');

        disp_char(6, i, 'p');			//"PRESS TO START"
        disp_char(6, i + 7, 'r');
        disp_char(6, i + 14, 'e');
        disp_char(6, i + 21, 's');
        disp_char(6, i + 28, 's');

        disp_char(6, i + 42, 't');
        disp_char(6, i + 49, 'o');

        disp_char(7, i + 7, 's');
        disp_char(7, i + 14, 't');
        disp_char(7, i + 21, 'a');
        disp_char(7, i + 28, 'r');
        disp_char(7, i + 35, 't');

        disp_char(5, 3, '|');			//Arrow pointing to start button
        disp_char(5, 5, '|');
        disp_char(6, 3, '|');
        disp_char(6, 5, '|');
        disp_char(7, 4, 'v');
    }
}

/**
 * Handle end game conditions and display appropriate messages or effects.
 */
void disp_end_game() {
    int i = 30;
    run = 0;							//pause game

	//Single player mode
    if (cur_player == 1 && multi_player == 0) {
        if (player_1_ball != 0) {		//check if player 1 still have balls available
            player_1_ball--;			//remove 1 ball
        } else {
            blank_screen();
            draw_borders();		
			disp_char(2, i, 'W');		//"WOMP WOMP"
            disp_char(2, i + 7, 'O');
            disp_char(2, i + 14, 'M');
            disp_char(2, i + 21, 'P');

            disp_char(3, i, 'W');
            disp_char(3, i + 7, 'O');
            disp_char(3, i + 14, 'M');
            disp_char(3, i + 21, 'P');


            disp_char(5, i, 'G');		//"GAME OVER"
            disp_char(5, i + 7, 'A');
            disp_char(5, i + 14, 'M');
            disp_char(5, i + 21, 'E');

            disp_char(6, i, 'O');
            disp_char(6, i + 7, 'V');
            disp_char(6, i + 14, 'E');
            disp_char(6, i + 21, 'R');

            end_game = 1;				//set the end_game flag to prevent main loop from advancing
        }
	//multiplayer mode
    } else if (cur_player == 1 && multi_player == 1) {
        if (player_1_ball != 0) {		//check if player 1 still have balls available
            player_1_ball--;			//remove 1 ball
            cur_player = 2;
        } else {
            blank_screen();
            draw_borders();
 			disp_char(2, i, 'W');		//"WOMP WOMP"
            disp_char(2, i + 7, 'O');
            disp_char(2, i + 14, 'M');
            disp_char(2, i + 21, 'P');

            disp_char(3, i, 'W');
            disp_char(3, i + 7, 'O');
            disp_char(3, i + 14, 'M');
            disp_char(3, i + 21, 'P');


            disp_char(5, i, 'G');		//"GAME OVER"
            disp_char(5, i + 7, 'A');
            disp_char(5, i + 14, 'M');
            disp_char(5, i + 21, 'E');

            disp_char(6, i, 'O');
            disp_char(6, i + 7, 'V');
            disp_char(6, i + 14, 'E');
            disp_char(6, i + 21, 'R');

            end_game = 1;
        }
    } else {
        if (player_2_ball != 0) {		//check if player 1 still have balls available
            player_2_ball--;			//remove 1 ball
            cur_player = 1;
        } else {
            blank_screen();
            draw_borders();

			disp_char(2, i, 'W');		//"WOMP WOMP"	
            disp_char(2, i + 7, 'O');
            disp_char(2, i + 14, 'M');
            disp_char(2, i + 21, 'P');

            disp_char(3, i, 'W');
            disp_char(3, i + 7, 'O');
            disp_char(3, i + 14, 'M');
            disp_char(3, i + 21, 'P');


            disp_char(5, i, 'G');		//"GAME OVER"
            disp_char(5, i + 7, 'A');
            disp_char(5, i + 14, 'M');
            disp_char(5, i + 21, 'E');

            disp_char(6, i, 'O');
            disp_char(6, i + 7, 'V');
            disp_char(6, i + 14, 'E');
            disp_char(6, i + 21, 'R');

            end_game = 1;
        }
    }
	

    refresh_screen();					//draw everything
    x_pos = 40;							//reset x and y position
    y_pos = 40;
    x_vel = pow((-1), pot_value) * 2;	//randomly alternate the starting x velocity
    y_vel = -1;
}

/**
 * Draw the paddle on the screen based on the potentiometer's value.
 */
void draw_paddle() {
    int i;
    pot_avg = (int)pot_avg;					//convert to int so overflow doesn't happen in calculation
    if (cur_player == 1) {
        paddle_size_1 = (int)paddle_size_1; //also convert so overflow doesn't occur
        for (i = 898; i < (898 + paddle_size_1 + 1); i++) {
            screen[i + pot_avg] |= 0xC0;	//draw paddle from start positon all the way through the width of the paddle (player 1)
        }
    } else {
        paddle_size_2 = (int)paddle_size_2;//also convert so overflow doesn't occur
        for (i = 898; i < (898 + paddle_size_2 + 1); i++) {
            screen[i + pot_avg] |= 0xC0;	//draw paddle from start positon all the way through the width of the paddle (player 2)
        }
    }
}
