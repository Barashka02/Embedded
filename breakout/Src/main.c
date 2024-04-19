#include <C8051F020.h>
#include "../Inc/lcd.h"
#include "../Inc/draw.h"
#include "../Inc/move.h"
#include "../Inc/global.h"
//#define BUTTON (P2 & (1 << 6))
//#define BUTTON (P2^6)
//------------------------------------------------------
//GLOBAL VARIABLES
//------------------------------------------------------

sbit BUTTON = P2^6;

void adc_int() interrupt 15
{
	AD0INT = 0;
	data_out = (ADC0H << 8) | ADC0L;

	data_out = (data_out * ((88L - paddle_size)+1)) >> 12;
	//data_out = (((72 - paddle_size)+1)*data_out) >> 12;	// convert POT value to a temp value between 0-30
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


//	count = 0;
//	blank_screen();
//	draw_paddle(pot_avg, paddle_size);
	
//	move_ball();
	
	//draw_paddle(0, 20);
	//draw_borders();
	//draw_scores(1, 2, 1, 3);
	//refresh_screen();
}



void main()
{
	char sizes[4] = {8, 12, 16, 24};
	char speeds[4] = {50, 70, 90, 110};
   WDTCN = 0xde;  // disable watchdog
   WDTCN = 0xad;
   XBR2 = 0x40;   // enable port output
   //XBR0 = 4;      // enable uart 0
   OSCXCN = 0x67; // turn on external crystal
   TMOD = 0x20;   // wait 1ms using T1 mode 2
   TH1 = -167;    // 2MHz clock, 167 counts - 1ms
   TR1 = 1;
   while ( TF1 == 0 ) { }          // wait 1ms
   while ( !(OSCXCN & 0x80) ) { }  // wait till oscillator stable
   OSCICN = 8;    // switch over to 22.1184MHz
   //SCON0 = 0x50;  // 8-bit, variable baud, receive enable
   //TH1 = -6;      // 9600 baud
   	IE = 0xA0;
	EIE2 = 0x02;
	//setting up timer 2 (16 bit 10 milisecond auto reload);
	T2CON = 0X00;

	RCAP2H = -2211 >> 8; // Assign the high byte
	RCAP2L = -2211; // Assign the low byte

	//setting up adc
	ADC0CN = 0x8C;
	REF0CN = 0x07;
	ADC0CF = 0x40; //setting values to read from the pot
	AMX0SL = 0x0;
	
	// Enable flash writes, set necessary flags
    PSCTL = 0x01; // PSWE = 1, PSEE = 0

	TR2 = 1;
	
	paddle_size = P1 & 0x03;
	paddle_size = sizes[paddle_size];

	speed = (P1 & 0x0c) >> 2;
	speed = speeds[speed];

	
	init_lcd();
	//draw_borders();
	//draw_scores(1, 2, 1, 3);
	//refresh_screen();
	//display_player_ready();
	//draw_borders();
	//refresh_screen();
	while(BUTTON == 1)
	{}
	run = 1;
	//---------------
	while(1)
	{	
		//while(P2^6 == 0)
		//{}
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
			
 			while(move_on == 0)
			{}
			refresh_screen();
			}
		}
	
	//------------------
	//while(1)
	//{
	
	//}
}



/*
//display character function
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

long data_out = 0;
long far_val = 0;
void adc_int() interrupt 15
{
	AD0INT = 0;
	data_out = (ADC0H << 8) | ADC0L;
	averaged = 0;
	

	if(index < 256) // checking if the value needs to be from the temp sensor.
	{
		far_val = ((data_out - 2475) * 12084L) >> 16;
		temp_value += far_val;
		
		if(index == 255)
		{
			ADC0CF = 0x40; //setting values to read from the pot
			AMX0SL = 0x0;
		}
		index++;
	}
	else if(index < 512)  // checking if the value needs to be from the pot.
	{
		data_out = (31*data_out) >> 12;	// convert POT value to a temp value between 0-30
		pot_value += 55 + data_out;		// Desired range is 55-85, hence the 55 value
		if(index == 511)
		{
			ADC0CF = 0x41; //setting values to read from the temp
			AMX0SL = 0x08;
		}
		index++;
	}
		if(index == 512)
		{
			//take the average.
			temp_avg = temp_value >> 8;
			pot_avg = pot_value >> 8;
			averaged = 1;
			
			//clear all values
			temp_value = 0;
			pot_value = 0;

			index = 0;
		}
		
		}
void t2_int() interrupt 5
{
   TF2 = 0;


   //checking if we need to read the temp sensor
   if(index < 256) 
   {
   		
		toggle = 0;
   }
   //checking if we need to read the pot sensor
   else if(index < 512)
   {
	
   }
   else
   {
		index = 0;
		ADC0CF = 0x41; //setting values to read from the temp
		AMX0SL = 0x08;
   }
}

void main()
{
   WDTCN = 0xde;  // disable watchdog
   WDTCN = 0xad;
   XBR2 = 0x40;   // enable port output
   XBR0 = 4;      // enable uart 0
   OSCXCN = 0x67; // turn on external crystal
   TMOD = 0x20;   // wait 1ms using T1 mode 2
   TH1 = -167;    // 2MHz clock, 167 counts - 1ms
   TR1 = 1;
   while ( TF1 == 0 ) { }          // wait 1ms
   while ( !(OSCXCN & 0x80) ) { }  // wait till oscillator stable
   OSCICN = 8;    // switch over to 22.1184MHz
   SCON0 = 0x50;  // 8-bit, variable baud, receive enable
   TH1 = -6;      // 9600 baud
   //interupt setup

   IE = 0x80;
   EIE2 = 0x02;
   
   // setting up the ADC
   ADC0CN = 0x8C;
   REF0CN = 0x07;
   ADC0CF = 0x41; //setting values to read from the temp (possibly 0)
   AMX0SL = 0x08;
   AMX0CF = 0x00;

   // setting up timer 2
   T2CON = 0x00;

   //RCAP2H = 0xF9;  // This overflows 512 times in 400 milisecond. 
   //RCAP2L = 0x40;

   RCAP2H = -843 >> 8;  // This overflows 512 times in 400 milisecond. 
   RCAP2L = -843;

   //RCAP2H = 0xF9;
   //RCAP2L = 0x40;

   TR2 = 1;
init_lcd();
while(1)
{
	if(averaged == 1)
	{
		blank_screen();

		tens = (pot_avg / 10) % 10;
		ones = (pot_avg  % 10);

		temp_tens = (temp_avg / 10) % 10;
		temp_ones = (temp_avg % 10);
		
	
		disp_char(0, 0, 'T');
		disp_char(0, 7, 'e');
		disp_char(0, 14, 'm');
		disp_char(0, 21, 'p');
			
		disp_char(0, 35, '0' + temp_tens);
		disp_char(0, 42, '0' + temp_ones);

		disp_char(2, 0, 'P');
		disp_char(2, 7, 'o');
		disp_char(2, 14, 't');


		disp_char(2, 28, '0' + tens);
		disp_char(2, 35, '0' + ones);

		if(temp_avg < pot_avg)
		{
			P3 = 0x00;
			P2 = 0x00;

		}
		else
		{
			P3 = 0xff;
			P2 = 0xff;
		}
		
	}
	
	refresh_screen();

}

	
   //init_lcd();
   //for ( ; ; )
   //{
     
   //}
}
*/
