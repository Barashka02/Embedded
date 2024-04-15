
#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/global.h"



//---------------------------------------------------------
//SUPPORT FUNCTIONS
//---------------------------------------------------------

//Draw ball for score
code unsigned char score_ball[] = {0x0e, 0x1f, 0x1f, 0x1f, 0x0e};
void draw_score_ball(int x, int y)
{	
	unsigned char row, col, shift, j, hit;
	int i;
	
	col = x-2;
	row = y-2;
	shift = row % 8;
	row = row / 8;
	hit = 0;

	//checking and writing the ball (no overflow on page)
	for(j=0, i=row*128+col; j<5; i++, j++)
	{
		int mask = (int)score_ball[j]<<shift;
		hit |= screen[i]&(unsigned char)mask;
		screen[i] = mask;
	}

}
		
//Display the character
void disp_char(unsigned char row, unsigned char column, char charachter)
{
	int i, j;
	unsigned char k;
	i = 128 * row + column;
	j = (charachter - 0x20)*5;
	for(k = 0; k < 5; ++k)
		{
		screen[i + k] |= font5x8[j+k];
		}
}
void draw_bricks()
{
    int i, j, k;
    for (i = 0; i < 11; i++)
    {
        for (j = 0; j < 5; j++)
        {
            int index = (j / 2) * 128 + i * 7 + 131;  // common index calculation
            int mask = (j % 2 == 0) ? 0x07 : 0x70;   // decide the mask based on even/odd row

            for (k = 0; k < 6; k++)
            {
                if (bricks[i][j] == 0)
                {
                    screen[index + k] &= ~mask;  // clear bits using negated mask
                }
                else if (bricks[i][j] == 1)
                {
                    screen[index + k] |= mask;  // set bits using mask
                }
            }
        }
    }
}


//---------------------------------------------------------
//GLOBAL FUNCTIONS
//---------------------------------------------------------

void draw_borders()
{
	int i, j;
	for(i=1; i<80; i++)
	{
		//drawing the top line
	screen[i] |= 0x03;

	}
	for(j=0; j<8; j++)
	{
		//drawing the left line
		
	screen[j*128] |= 0xff;
	screen[(j)*128+1] |= 0xff;
		//drawing the right line
	screen[(j)*128+78] |= 0xff;
	screen[(j)*128+79] |= 0xff;

	}
}

void draw_scores(int high_score, int cur_score, int cur_player, int player_ball)
{	
	//initialization
	int i = 85; //starting point for "HIGHI"
	int thousands, hundreds, tens, ones;
	
	//writing the "HIGH"
	disp_char(0, i, '-');
	disp_char(0, i+7, 'H');
	disp_char(0, i+14, 'I');
	disp_char(0, i+21, 'G');
	disp_char(0, i+28, 'H');
	disp_char(0, i+35, '-');

	//calculating the score placements
	thousands = high_score / 1000;
	hundreds = (high_score / 100) % 10; 
	tens = (high_score / 10) % 10; 
	ones = high_score % 10;
	
	//writing the high score
	i = 92; //"Starting point for the score"
	disp_char(1, i, '0' + thousands);
	disp_char(1, i+7, '0' + hundreds);
	disp_char(1, i+14, '0' + tens);
	disp_char(1, i+21, '0' + ones);
	
	for(i = 80; i < 126; i++)
	{
		disp_char(2, i, '-');
	}
//-----------------------------------------
//Writing the Current Score
	
	i= 82;
	disp_char(3, i, '-');
	disp_char(3, i+7, 'S');
	disp_char(3, i+14, 'C');
	disp_char(3, i+21, 'O');
	disp_char(3, i+28, 'R');
	disp_char(3, i+35, 'E');
	disp_char(3, i+42, '-');

	//calculating the score placements
	thousands = cur_score / 1000;
	hundreds = (cur_score / 100) % 10; 
	tens = (cur_score / 10) % 10; 
	ones = cur_score % 10;
	
	//writing the high score
	i = 92;
	disp_char(4, i, '0' + thousands);
	disp_char(4, i+7, '0' + hundreds);
	disp_char(4, i+14, '0' + tens);
	disp_char(4, i+21, '0' + ones);
	
	for(i = 80; i < 126; i++)
	{
		disp_char(5, i, '-');
	}
//-----------------------------------------
//Writing the players and their balls count
	i = 100;

	disp_char(6, i, 'P');
	disp_char(6, i+7, '0' + cur_player);

	//drawing the leftover balls
	i= 95;
	if(player_ball == 3)
	{
	//hit_no_care = draw_ball(int x, int y
	draw_score_ball(i, 60);
	draw_score_ball(i+11, 60);
	draw_score_ball(i+22, 60);
	
	}
	
	else if(player_ball == 2)
	{
	//hit_no_care = draw_ball(int x, int y
	draw_score_ball(i, 60);
	draw_score_ball(i+11, 60);
	}

	else if(player_ball == 1)
	{
	draw_score_ball(i, 60);
	}


}

//Drawing the end game state

void disp_end_game()
{
	int i = 30;
	
	//TR2 = 0;
	run = 0;

	disp_char(3, i, 'G');
	disp_char(3, i+7, 'A');
	disp_char(3, i+14, 'M');
	disp_char(3, i+21, 'E');

	disp_char(5, i, 'O');
	disp_char(5, i+7, 'V');
	disp_char(5, i+14, 'E');
	disp_char(5, i+21, 'R');
}

void draw_paddle()
{	
	
	int i;
	
	pot_avg = (int)pot_avg;
	paddle_size = (int)paddle_size;
	for(i = 898; i < (898 + paddle_size + 1); i++)
	{
		screen[i + pot_avg] |= 0xC0;
	}
	//data_out = (31*data_out) >> 12; // convert POT value to a temp value between 0-30
	//pot_value += 55 + data_out; // Desired range is 55-85, hence the 55 value
}
	
