/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"
#include "view.h"

/* Joystick 5-way switch for PRESS */
enum position { left,down,right,up,centre };
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};// LFT, RGHT, UP, DWN, CTR

/* Return the position of the joystick */
bool joystickPress(enum position p) {
		return !joystick[p];
}

/* CONTROLS */
void controls(void)
{
		//Wait for centre joystick press before starting game
		if(gameStart == false && joystickPress(centre)) {
				gameStart = true;
				masterReset();
				wait_ms(200);
		}
		//If all lives have been used - reset game
		else if(gameStart == true && lives == 0 && shieldState == 0 && joystickPress(up)) {
				gameStart = false;
		}
		//If centre joystick press - fire missile
		else if(lives > 0 && joystickPress(centre)) {
				fireMissile();
		}
		//If joystick is UP - thrust ship
		else if(joystickPress(up) && shipSpeed <= 2.4) {
				shipSpeed = shipSpeed + 0.2;
		}
		//If joystick is LEFT - rotate ship clockwise (INVERTED)
		else if(joystickPress(left)) {
				shipTrajectory = shipTrajectory + 26;
		}
		//If joystick is RIGHT - rotate ship anti-clockwise (INVERTED)
		else if(joystickPress(right)) {
				shipTrajectory = shipTrajectory - 26;
		}
		//If joystick is DOWN - activate shield
		else if(shieldReady == true && joystickPress(down)) {
				shieldState = 3;
				shieldReady = false;
		}
}
