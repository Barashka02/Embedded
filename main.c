#include <C8051F020.h>

code unsigned char sine[] = { 176, 217, 244, 254, 244, 217, 176, 128, 80, 39, 12, 2, 12, 39, 80, 128 };
unsigned char phase = sizeof(sine)-1;  // Current point in sine to output
unsigned int duration = 0;              // Number of cycles left to output

// New variables for amplitude scaling
unsigned int maxDuration = 800; // Initial max duration for scaling
unsigned char maxAmplitude = 255; // Maximum amplitude. Adjust according to your needs
unsigned char scaledAmplitude = 0;


void timer2(void) interrupt 5
{
    TF2 = 0;
    
    // Scale the sine value based on remaining duration
    scaledAmplitude = (unsigned char)((long)sine[phase] * duration / maxDuration);
    
    // Ensure the scaled amplitude does not exceed the DAC range
    if (scaledAmplitude > maxAmplitude) {
        scaledAmplitude = maxAmplitude;
    }
    
    DAC0H = scaledAmplitude;
    
    if (phase < sizeof(sine)-1) {
        phase++;
    } else if (duration > 0) {
        phase = 0;
        duration--;
    }
}

sbit F_BUTTON = P2^7; // Right Button = Front Button
sbit B_BUTTON = P2^6; // Left Button = Back Button
bit f_pressed = 0; // Flag to indicate if F_BUTTON was previously pressed
bit b_pressed = 0; // Flag to indicate if B_BUTTON was previously pressed

void main(void)
{
    WDTCN = 0x0DE;    // Disable watchdog
    WDTCN = 0x0AD;
    XBR2 = 0x40;      // Enable port output
    OSCXCN = 0x67;    // Turn on external crystal
    TMOD = 0x20;      // Wait 1ms using T1 mode 2
    TH1 = 256 - 167;  // 2MHz clock, 167 counts = 1ms
    TR1 = 1;
    while (TF1 == 0) { }
    while ((OSCXCN & 0x80) == 0) { }
    OSCICN = 0x08;    // Engage! Now using 22.1184MHz
    T2CON = 4;        // Timer 2, auto reload
    RCAP2H = -1;
    REF0CN = 3;       // Turn on voltage reference
    DAC0CN = 0x9C;    // Update on timer 2, left justified
    IE = 0xA0;        // Enable timer 2 only

    for (;;)
    {
        if (F_BUTTON && !f_pressed) { // If F_BUTTON is pressed and was not pressed before
            f_pressed = 1; // Set the flag indicating the button is pressed
            if (duration == 0) {
                RCAP2L = -144;              // Start high frequency
                duration = maxDuration = 800; // Reset maxDuration as well for scaling
            }
        }
        if (!F_BUTTON && f_pressed) { // If F_BUTTON is released
            f_pressed = 0; // Reset the flag
        }

        if (B_BUTTON && !b_pressed) { // If B_BUTTON is pressed and was not pressed before
            b_pressed = 1; // Set the flag indicating the button is pressed
            if (duration == 0) {
                RCAP2L = -181;              // Start low frequency
                duration = maxDuration = 635; // Reset maxDuration as well for scaling
            }
        }
        if (!B_BUTTON && b_pressed) { // If B_BUTTON is released
            b_pressed = 0; // Reset the flag
        }
    }
}
