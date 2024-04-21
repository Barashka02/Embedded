
//---------------------------------------------------
// THIS IS THE MAIN FILE THAT CONTROLS ALL GAME FLOW
//---------------------------------------------------


#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/draw.h"
#include "../Inc/move.h"
#include "../Inc/global.h"

//------------------------------------------------------
//VARIABLES
//------------------------------------------------------

sbit BUTTON = P2^6;
code unsigned char sine[16] = { 176, 217, 244, 254, 244, 217, 176, 128, 80, 39, 12, 2, 12, 39, 80, 128 };
unsigned char phase = sizeof(sine)-1;  // Current point in sine to output
            
//------------------------------------------------------
//INTERUPTS 
//------------------------------------------------------

/**
 * Interrupt Service Routine for generating sound effects.
 * It plays a sound by toggling the interrupt flag and outputting a sine wave through a DAC.
 * No parameters, returns nothing.
 */
void sound() interrupt 16
{
    T4CON = T4CON ^ 0x80; 	// Toggle the interrupt flag to clear it
    DAC0H = sine[phase]; 	// Output the current phase of the sine wave to the DAC

    if (phase < sizeof(sine) - 1) {
        phase++; 			// Increment the phase of the sine wave
    } else if (duration > 0) {
        phase = 0;  		// Reset the phase to the start of the sine wave
        duration--; 		// Decrement the duration for how long the sound should play
    }
    if (duration == 0) {
        T4CON = 0x00;  		// Disable the timer if the duration is 0, stopping the sound
    }
}


/**
 * ADC Interrupt Service Routine for potentiometer readings.
 * It processes ADC results to calculate the paddle's position.
 * No parameters, returns nothing.
 */
void adc_int() interrupt 15
{
	AD0INT = 0;		
	data_out = (ADC0H << 8) | ADC0L;	//data out of the ADC. After calculations this will be the leftmost position of the paddle

	//data_out is the scaled to fit the paddle in the screen.  Its a value between 0 and the right edge of the screen (88) - paddles size.
	if(cur_player == 1)
	{
		data_out = (data_out * ((88L - paddle_size_1)+1)) >> 12;
	}
	else
	{
		data_out = (data_out * ((88L - paddle_size_2)+1)) >> 12;
	}
	pot_value += data_out;				// Desired range is 0 to max width - paddle size
	count++;							//increase cound to move through the main loop

	if(count % 7 == 0)					//take the average.
	{
		pot_avg = pot_value >> 3;		//divide by 8 (8 samples/8=average)
		pot_value = 0;					//reset the accumulating value
	}
}

//Interupt for timer2 (looping through main loop
void timer2(void) interrupt 5{
	TF2 = 0;				//reset interupt flag

	if(count == speed)		//enough time has passed. Draw everything
	{
    	count = 0;
    	move_on = 1;		// Will allow the game loop to advance
	}
	else 					//time has not yet passed.  Keep waiting
	{
    	move_on = 0; 		// Prevents the game loop from advancing too quickly
	}
}


//------------------------------------------------------
//MAIN
//------------------------------------------------------
void main()
{
//Inititizling the speeds and paddles size arrays for later
	char sizes[4] = {8, 12, 16, 24};
	char speeds[4] = {50, 70, 90, 110};

//Initialization routine
//-----------------------------------------------------------
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

	//Enabling interupts
	IE = 0xA0;
	EIE2 = 0x06;

	//setting up timer 2 (16 bit 10 milisecond auto reload);
	T2CON = 0X00;
	
	//setting up the timer 2 for the speed of the ball
	RCAP2H = -2211 >> 8;
	RCAP2L = -2211;

	//setting up adc
	ADC0CN = 0x8C;
	REF0CN = 0x07;
	ADC0CF = 0x40; //setting values to read from the pot
	AMX0SL = 0x0;

	//setting up the DAC
	DAC0CN = 0x9c;
	RCAP4H = -1;
	RCAP4L = -144;

	//starting the timers
	TR2 = 1;
	T4CON = T4CON & 0x04;
	CKCON = 0x40;

//Initializing the switches
//-----------------------------------------------------------
	
	//switch for paddle sizes for player 1 (switches 0 and 1)
	paddle_size_1 = P1 & 0x03;
	paddle_size_1  = sizes[paddle_size_1];

	//switch for paddle size for player 2 (switches 2 and 3)
	paddle_size_2 = (P1 & 0x0c) >> 2;
	paddle_size_2  = sizes[paddle_size_2];
	
	//switch for speed of the ball (switchs 4 and 5)
	speed = (P1 & 0x30) >> 4;
	speed = speeds[speed];

	//swtich for multi player mode (switch 7)
	multi_player = P1 >> 7;
	
	init_lcd();

//Main loop
//-----------------------------------------------------------
	
	while(1)
	{
		//Starting screen of the game
		if(end_game != 1)
		{
			display_player_ready();
			refresh_screen();
			while(BUTTON == 1)
			{}
			run = 1;
		}
		//Main loop for running through the game
		while(run == 1)
		{
			blank_screen();
			draw_borders();
			draw_paddle();
			draw_bricks();
			move_ball();
			
			//updating the high scores
			if(high_score < score1 || high_score < score2)
			{
				if(cur_player == 1)//player 1 high score
				{
					high_score = score1;
				}
				else //player 2 high score
				{
					high_score = score2;
				}

			}
			// updating the scores
			draw_scores();

			//waiting to draw scores (so that it draws at a consistent speed and doesn't overwrite previous calls)
			if(run == 1)
			{
	 			while(move_on == 0)
				{}
				refresh_screen();
			}
		}
	}	
}


