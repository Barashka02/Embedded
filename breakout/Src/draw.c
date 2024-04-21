
#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/global.h"
#include <math.h>
sbit BUTTON = P2^6;


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
    for (i = 0; i < 13; i++)
    {
        for (j = 0; j < 5; j++)
        {
            // Adjusted index calculation to fit bricks of width 5 with 1 pixel gap
            int index = (j / 2) * 128 + i * 6 + 131;  // Now `i * 6` instead of `i * 7`

            int mask = (j % 2 == 0) ? 0x07 : 0x70;   // decide the mask based on even/odd row

            for (k = 0; k < 5; k++)  // Draw each brick 5 pixels wide
            {	
				if(cur_player == 1)
				{
	                if (p1_bricks[i][j] == 0)
	                {
	                    screen[index + k] &= ~mask;  // clear bits using negated mask
	                }
	                else if (p1_bricks[i][j] == 1)
	                {
	                    screen[index + k] |= mask;  // set bits using mask
	                }
				}
				else
				{
					if (p2_bricks[i][j] == 0)
	                {
	                    screen[index + k] &= ~mask;  // clear bits using negated mask
	                }
	                else if (p2_bricks[i][j] == 1)
	                {
	                    screen[index + k] |= mask;  // set bits using mask
	                }
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
	for(i=1; i<82; i++)
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
	screen[(j)*128+81] |= 0xff;
	screen[(j)*128+82] |= 0xff;

	}
}

void draw_scores()
{	
	//initialization
	int i = 86; //starting point for "HIGHI"
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
	i = 93; //"Starting point for the score"
	disp_char(1, i, '0' + thousands);
	disp_char(1, i+7, '0' + hundreds);
	disp_char(1, i+14, '0' + tens);
	disp_char(1, i+21, '0' + ones);
	
	for(i = 81; i < 126; i++)
	{
		disp_char(2, i, '-');
	}
//-----------------------------------------
//Writing the Current Score
	
	i= 83;
	disp_char(3, i, '-');
	disp_char(3, i+7, 'S');
	disp_char(3, i+14, 'C');
	disp_char(3, i+21, 'O');
	disp_char(3, i+28, 'R');
	disp_char(3, i+35, 'E');
	disp_char(3, i+42, '-');
	
	if(cur_player == 1)
	{
		//calculating the score placements
		thousands = score1 / 1000;
		hundreds = (score1 / 100) % 10; 
		tens = (score1 / 10) % 10; 
		ones = score1 % 10;
	}
	else 
	{
		//calculating the score placements
		thousands = score2 / 1000;
		hundreds = (score2 / 100) % 10; 
		tens = (score2 / 10) % 10; 
		ones = score2 % 10;
	}
	
	//writing the high score
	i = 93;
	disp_char(4, i, '0' + thousands);
	disp_char(4, i+7, '0' + hundreds);
	disp_char(4, i+14, '0' + tens);
	disp_char(4, i+21, '0' + ones);
	
	for(i = 81; i < 126; i++)
	{
		disp_char(5, i, '-');
	}
//-----------------------------------------
//Writing the players and their balls count
	i = 101;

	disp_char(6, i, 'P');
	disp_char(6, i+7, '0' + cur_player);

	//drawing the leftover balls
	i= 96;
	if(cur_player == 1)
	{
		if(player_1_ball == 3)
		{
		//hit_no_care = draw_ball(int x, int y
		draw_score_ball(i, 60);
		draw_score_ball(i+11, 60);
		draw_score_ball(i+22, 60);
	
		}
	
		else if(player_1_ball == 2)
		{
		//hit_no_care = draw_ball(int x, int y
		draw_score_ball(i, 60);
		draw_score_ball(i+11, 60);
		}

		else if(player_1_ball == 1)
		{
		draw_score_ball(i, 60);
		}
	}
	else
	{
		if(player_2_ball == 3)
		{
		//hit_no_care = draw_ball(int x, int y
		draw_score_ball(i, 60);
		draw_score_ball(i+11, 60);
		draw_score_ball(i+22, 60);
	
		}
	
		else if(player_2_ball == 2)
		{
		//hit_no_care = draw_ball(int x, int y
		draw_score_ball(i, 60);
		draw_score_ball(i+11, 60);
		}

		else if(player_2_ball == 1)
		{
		//run = 0;
		draw_score_ball(i, 60);
		}
	

	}


}

//Drawing the end game state

void display_player_ready() {
    int i = 20;

	blank_screen();
	draw_borders();
	draw_scores();
	draw_borders();


		blank_screen();
		draw_borders();
		draw_scores();
		draw_borders();
	if (cur_player == 1 && multi_player == 0) {
		disp_char(1, i, 'S');
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

		disp_char(6, i, 'p');
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

		disp_char(5, 3, '|');
		disp_char(5, 5, '|');
		disp_char(6, 3, '|');
		disp_char(6, 5, '|');
		disp_char(7, 4, 'v');
}
    else if (cur_player == 1 && multi_player == 1) {
        disp_char(2, i, 'P');
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
		disp_char(6, i, 'p');
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

		disp_char(5, 3, '|');
		disp_char(5, 5, '|');
		disp_char(6, 3, '|');
		disp_char(6, 5, '|');
		disp_char(7, 4, 'v');


    } else {
        disp_char(3, i, 'P');
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

		disp_char(6, i, 'p');
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

		disp_char(5, 3, '|');
		disp_char(5, 5, '|');
		disp_char(6, 3, '|');
		disp_char(6, 5, '|');
		disp_char(7, 4, 'v');
    }
}

void disp_end_game()
{
	int i = 30;
//	int j, k;
	run = 0;

	if(cur_player == 1 && multi_player == 0)
	{	
		if(player_1_ball != 0)
		{
			player_1_ball--;

			//run = 1;
		}
		else
		{
			blank_screen();
			draw_borders();
			disp_char(2, i, 'G');
			disp_char(2, i+7, 'A');
			disp_char(2, i+14, 'M');
			disp_char(2, i+21, 'E');

			disp_char(3, i, 'O');
			disp_char(3, i+7, 'V');
			disp_char(3, i+14, 'E');
			disp_char(3, i+21, 'R');

			end_game = 1;
   		}
	}

	else if(cur_player == 1 && multi_player == 1)
	{	
		if(player_1_ball != 0)
		{
			player_1_ball--;
			cur_player =2;
			//run = 1;
		}
		else
		{
			blank_screen();
			draw_borders();
			disp_char(2, i, 'G');
			disp_char(2, i+7, 'A');
			disp_char(2, i+14, 'M');
			disp_char(2, i+21, 'E');

			disp_char(3, i, 'O');
			disp_char(3, i+7, 'V');
			disp_char(3, i+14, 'E');
			disp_char(3, i+21, 'R');

			end_game = 1;

		}
		//cur_player = 2;
	}
	else
	{
		if(player_2_ball != 0 && multi_player == 1)
		{	
			
			player_2_ball--;
			cur_player = 1;
			//run = 1;
		}
		else 
		{
			blank_screen();
			draw_borders();
			disp_char(2, i, 'G');
			disp_char(2, i+7, 'A');
			disp_char(2, i+14, 'M');
			disp_char(2, i+21, 'E');

			disp_char(3, i, 'O');
			disp_char(3, i+7, 'V');
			disp_char(3, i+14, 'E');
			disp_char(3, i+21, 'R');


			end_game = 1;			
		}
	}

		
	//TR2 = 0;
	//blank_screen();
	//draw_borders();
	//draw_scores();
	//display_player_ready();
	refresh_screen();
	x_pos = 40;
	y_pos = 40;
	x_vel = pow((-1), pot_value) * 2;
	y_vel = -1;

//	while(BUTTON == 1)
//	{}
//	run = 1;
//	return;

}

void draw_paddle()
{	
	
	int i;
	pot_avg = (int)pot_avg;
	if(cur_player == 1)
	{
		paddle_size_1 = (int)paddle_size_1;
		for(i = 898; i < (898 + paddle_size_1 + 1); i++)
		{
			screen[i + pot_avg] |= 0xC0;
		}
	}
	else
	{
		paddle_size_2 = (int)paddle_size_2;
		for(i = 898; i < (898 + paddle_size_2 + 1); i++)
		{
			screen[i + pot_avg] |= 0xC0;
		}
	}	
}
	//data_out = (31*data_out) >> 12; // convert POT value to a temp value between 0-30
	//pot_value += 55 + data_out; // Desired range is 55-85, hence the 55 value

	
