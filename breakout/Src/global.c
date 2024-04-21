
//-------------------------------------------------------------------------------
//THIS IS THE FILE THAT MAKES ALL THE GLOBAL VARIABLES ACCESSIBLE TO OTHER FILES
//-------------------------------------------------------------------------------

#include <C8051F020.h>


//position constants
//-----------------------------------------------------------
int x_vel = 2;			//x movement of the ball (angle)
int y_vel = -1;			//y movement of the ball (angle)
int x_pos = 40;			//current x position of the ball
int y_pos = 40;			//current y position of the ball

//player 1/2 constants
//-----------------------------------------------------------
int score1;				//player 1 score
int score2;				//player 2 score
char cur_player = 1;	//flag for current player
char player_1_ball = 3;	//amount of balls left for player 1
char player_2_ball = 3;	//amoung of balls left for player 2
int paddle_size_1;		//paddle size for player 1
int paddle_size_2;		//paddle size for player 2

//arrays for the bricks for both players
//-----------------------------------------------------------																	

//player 1 brick
	//1 = brick showing
	//0 = brick disapeared
xdata char p1_bricks[13][5] = {
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1}
};

//player 2 bricks
	//1 = brick showing
	//0 = brick disapeared
xdata char p2_bricks[13][5] = {
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1}
};

//other miscelanout variables neede for computations
//-----------------------------------------------------------

long data_out = 0; 			//unbounded value from POT
int pot_value = 0; 			//value in bound of 0-left_edge of paddle
char pot_avg = 0;  			//averaged value of the pot_output 
char count = 0;    			//value for averaging loop (number of times before averaged)

unsigned int duration = 0;	//value for duration of "beep" for the collisions
int high_score;				//value for the high score (doesn't matter which player)
int hit_location; 			//for hit detection in draw_ball function

char move_on = 1; 			//flag to wait to draw in main loop
//0 = wait
//1 = move on

char run = 0;	  			//flag to start the game (wait screen)	
//0 = stop
//1 = run

char end_game = 0;			//flag to end the game (stop the main loop)
//0 = not end of game
//1 = game has ended

char multi_player;			//flag for multiplayer game 
//0 = one player (only player 1)
//1 = two player (player 1 & 2)

char speed = 80;			//value for speed (looping through the main loop)
//high value = slower speed
//low value = faster speed







