
//----------------------------------------------------------------------------
// THIS IS THE FILE THAT TAKES CARE OF ALL THE MOVEMENT HAPPENING IN THE GAME
//----------------------------------------------------------------------------

/**
 * Draw the ball on the screen and handle wall collisions.
 *
 * @param x The x-coordinate of the ball's center.
 * @param y The y-coordinate of the ball's center.
 * @return int Non-zero if the ball hits any screen edges or zero otherwise.
 */
char draw_ball(int x, int y);

/**
 * Update ball position based on velocity and handle brick and paddle collisions.
 */
void move_ball();