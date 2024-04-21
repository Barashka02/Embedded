
//-------------------------------------------------------------------------------
//THIS IS THE FILE THAT MAKES ALL THE GLOBAL VARIABLES ACCESSIBLE TO OTHER FILES
//-------------------------------------------------------------------------------

//-----------------------------------------------------------
//GLOBAL VARIABLES
//-----------------------------------------------------------

//position constants
//-----------------------------------------------------------
extern int x_vel;			//x movement of the ball (angle)
extern int y_vel;			//y movement of the ball (angle)
extern int x_pos;			//current x position of the ball
extern int y_pos;			//current y position of the ball

//player 1/2 constants
//-----------------------------------------------------------
extern int score1;				//player 1 score
extern int score2;				//player 2 score
extern char cur_player;			//flag for current player
extern char player_1_ball;		//amount of balls left for player 1
extern char player_2_ball;		//amoung of balls left for player 2
extern int paddle_size_1;		//paddle size for player 1
extern int paddle_size_2;		//paddle size for player 2

//arrays for the bricks for both players
//-----------------------------------------------------------

//player 1 brick
	//1 = brick showing
	//0 = brick disapeared
extern xdata char p1_bricks[13][5];

//player 2 bricks
	//1 = brick showing
	//0 = brick disapeared
extern xdata char p2_bricks[13][5];
//other miscelanout variables neede for computations
//-----------------------------------------------------------

extern long data_out; 			//unbounded value from POT
extern int pot_value; 			//value in bound of 0-left_edge of paddle
extern char pot_avg;  			//averaged value of the pot_output 
extern char count;    				//value for averaging loop (number of times before averaged)

extern unsigned int duration;		//value for duration of "beep" for the collisions
extern int high_score;				//value for the high score (doesn't matter which player)
extern int hit_location; 			//for hit detection in draw_ball function

extern char move_on; 				//flag to wait to draw in main loop
//0 = wait
//1 = move on

extern char run;	  				//flag to start the game (wait screen)	
//0 = stop
//1 = run

extern char end_game;				//flag to end the game (stop the main loop)
//0 = not end of game
//1 = game has ended

extern char multi_player;			//flag for multiplayer game 
//0 = one player (only player 1)
//1 = two player (player 1 & 2)

extern char speed	;				//value for speed (looping through the main loop)
//high value = slower speed
//low value = faster speed







