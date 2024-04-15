//position constants
int x_vel = -2;
int y_vel = -1;
int x_pos = 40;
int y_pos = 32;

//player 1/2 constants
char ball_cnt = 3;
int player = 1;

//setup constants
int paddle_size = 16;
char speed = 49;
//char speed = 100;
char pot_avg = 0;

//array for the bricks
xdata char bricks[11][5] = {
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


//miscelanout neede for computations
long data_out = 0;
char move_on = 1;
char run = 1;
char count = 0;
int pot_value = 0;



