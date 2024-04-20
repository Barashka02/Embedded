//position constants
int x_vel = 2;
int y_vel = -1;
int x_pos = 40;
int y_pos = 40;

//player 1/2 constants
#include <C8051F020.h>

char ball_cnt = 3;
int player = 1;

//setup constants

char speed = 80;
//char speed = 100;
char pot_avg = 0;
int score1;
int score2;
int high_score;

//player setup
char cur_player = 1;
char player_1_ball = 3;
char player_2_ball = 3;
int paddle_size_1;
int paddle_size_2;


//array for the bricks
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




int hit_location;
//miscelanout neede for computations
long data_out = 0;
char move_on = 1;
char run = 0;
char count = 0;
int pot_value = 0;
char end_game = 0;
char multi_player;
int zero_cnt = 0;
unsigned int duration = 0;





