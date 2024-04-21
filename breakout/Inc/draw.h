
//------------------------------------------------------------------------------------
// THIS IS THE FILE THAT HAS ALL THE FUNCTIONS TO DRAW EVERYTHING NEEDED ON THE SCREEN
//------------------------------------------------------------------------------------


//---------------------------------------------------------
// SUPPORT FUNCTIONS
//---------------------------------------------------------

/**
 * Draw the ball on the screen and handle wall collisions.
 *
 * @param x The x-coordinate of the ball's center.
 * @param y The y-coordinate of the ball's center.
 * @return int Non-zero if the ball hits any screen edges or zero otherwise.
 */
char draw_ball(int x, int y);

/**
 * Display a single character on the LCD at a specific row and column.
 *
 * @param row The row number on the LCD where the character will be displayed.
 * @param column The column number on the LCD where the character will be displayed.
 * @param character The ASCII character to be displayed.
 */
void disp_char(unsigned char row, unsigned char column, char character);

/**
 * Draw bricks on the screen based on the game's current state.
 */
void draw_bricks();

//---------------------------------------------------------
// GLOBAL FUNCTIONS
//---------------------------------------------------------

/**
 * Draw borders around the game area on the LCD.
 */
void draw_borders();

/**
 * Display current and high scores on the LCD.
 */
void draw_scores();

/**
 * Manage the display of the game screen when a player is ready or the game starts.
 */
void display_player_ready();

/**
 * Handle end game conditions and display appropriate messages or effects.
 */
void disp_end_game();

/**
 * Draw the paddle on the screen based on the potentiometer's value.
 */
void draw_paddle();