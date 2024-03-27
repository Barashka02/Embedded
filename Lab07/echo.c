#include <C8051F020.h>
#include "lcd.h"

bit toggle = 1;


long temp_value = 0x00;
long pot_value = 0x00;
long temp_avg = 0x00;
long pot_avg = 0x00;
int index = 0;
bit averaged = 0;
char tens = 0;
char ones = 0;
char temp_tens = 0;
char temp_ones = 0;


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
		
		disp_char(0, 0, '0' + temp_tens);
		disp_char(0, 7, '0' + temp_ones);

		disp_char(2, 0, '0' + tens);
		disp_char(2, 7, '0' + ones);

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

