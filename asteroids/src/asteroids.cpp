/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
struct ship player;

float Dt = 0.01f;

bool gameStart = false;
int shieldState;
bool shieldReady = false;
int shipTrajectory;

Ticker model, view, controller, rocks;

void timerHandler();

bool paused = true;
/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);

/* Restore Lives and Shields - Reset Score - Reset Ship Position */
void masterReset(void) {
		lives = 5;
		shieldState = 3;
		score = 0;
		shipOriginX = 230;
		shipOriginY = 120;
}

int main()
{
    init_DBuffer();
    intialiseAsteroidHeap();
	intialiseMissileHeap();
	
	/* Attach Files */
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
	rocks.attach( spawnAsteroid, 0.1);
    
    lives = 5;
	shieldState = 3;
    
    /* Pause to start */
    while( userbutton.read() ){ /* remember 1 is not pressed */
        paused=true;
        wait_ms(100);
    }
    paused = false;
    
    while(true) {
				if(lives == 0 && gameStart == true) {
						view.detach();
						model.detach();
						controller.detach();
						rocks.detach();
						drawGameOver();
						wait_ms(200);
				}
		}
}
