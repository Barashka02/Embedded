#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/draw.h"
#include "../Inc/move.h"
#include "../Inc/global.h"

//------------------------------------------------------
//GLOBAL VARIABLES
//------------------------------------------------------

sbit BUTTON = P2^6;

code unsigned char sine[16] = { 176, 217, 244, 254, 244, 217, 176, 128, 80, 39, 12, 2, 12, 39, 80, 128 };
unsigned char phase = sizeof(sine)-1;  // Current point in sine to output
            

void sound() interrupt 16
{
	T4CON = T4CON^0x80;  // clearing the interupt flag
	DAC0H = sine[phase];

    if (phase < sizeof(sine)-1) {
        phase++;
    } else if (duration > 0) {
        phase = 0;
        duration--;
    }
	if(duration == 0)
	{
		T4CON = 0x00;
	}
}

void adc_int() interrupt 15
{
	AD0INT = 0;
	data_out = (ADC0H << 8) | ADC0L;
	if(cur_player == 1)
	{
		data_out = (data_out * ((88L - paddle_size_1)+1)) >> 12;
	}
	else
	{
		data_out = (data_out * ((88L - paddle_size_2)+1)) >> 12;
	}
	pot_value += data_out;		// Desired range is 0 to max width - paddle size
	count++;

	if(count % 7 == 0)
	{
		//take the average.
		pot_avg = pot_value >> 3;
		pot_value = 0;
	}
}
void timer2(void) interrupt 5{
	TF2 = 0;
	if(count == speed) {
    	count = 0;
    	move_on = 1; // Will allow the game loop to advance
	}
	else {
    	move_on = 0; // Prevents the game loop from advancing too quickly
	}
}



void main()
{
	char sizes[4] = {8, 12, 16, 24};
	char speeds[4] = {50, 70, 90, 110};
	WDTCN = 0xde;  // disable watchdog
	WDTCN = 0xad;
	XBR2 = 0x40;   // enable port output

	OSCXCN = 0x67; // turn on external crystal
	TMOD = 0x20;   // wait 1ms using T1 mode 2
	TH1 = -167;    // 2MHz clock, 167 counts - 1ms
	TR1 = 1;
	while ( TF1 == 0 ) { }          // wait 1ms
	while ( !(OSCXCN & 0x80) ) { }  // wait till oscillator stable
	OSCICN = 8;    // switch over to 22.1184MHz
	IE = 0xA0;
	EIE2 = 0x06;
	//setting up timer 2 (16 bit 10 milisecond auto reload);
	T2CON = 0X00;

	RCAP2H = -2211 >> 8; // Assign the high byte
	RCAP2L = -2211; // Assign the low byte

	//setting up adc
	ADC0CN = 0x8C;
	REF0CN = 0x07;
	ADC0CF = 0x40; //setting values to read from the pot
	AMX0SL = 0x0;

	//setting up the DAC
	DAC0CN = 0x9c;
	RCAP4H = -1;
	RCAP4L = -144;
	// Enable flash writes, set necessary flags
	PSCTL = 0x01; // PSWE = 1, PSEE = 0

	TR2 = 1;
	T4CON = T4CON & 0x04;
	CKCON = 0x40;

	paddle_size_1 = P1 & 0x03;
	paddle_size_1  = sizes[paddle_size_1];

	paddle_size_2 = (P1 & 0x0c) >> 2;
	paddle_size_2  = sizes[paddle_size_2];

	speed = (P1 & 0x30) >> 4;
	speed = speeds[speed];

	multi_player = P1 >> 7;


	init_lcd();
	//draw_borders();
	//draw_scores(1, 2, 1, 3);
	//refresh_screen();
	

	//---------------
	while(1)
	{
		if(end_game != 1)
		{
			display_player_ready();
			//disp_start_game();
			refresh_screen();
			while(BUTTON == 1)
			{}
			run = 1;
			//while(P2^6 == 0)
			//{}
		}
		while(run == 1)
		{
			blank_screen();
			draw_borders();
			draw_paddle();
			draw_bricks();
			move_ball();
			if(high_score < score1 || high_score < score2)
			{
				if(cur_player == 1)
				{
					high_score = score1;
					//*((unsigned char xdata *) 0x00ff) = high_score;
				}
				else 
				{
					high_score = score2;
					//*((unsigned char xdata *) 0x00ff) = high_score;
				}

			}
			draw_scores();
			if(run == 1)
			{
	 			while(move_on == 0)
				{}
				refresh_screen();
			}
		}
	}	
}


