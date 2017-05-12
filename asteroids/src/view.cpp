/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

static const colour_t background = rgb(0,51,102); /* Midnight Blue */


/* DOUBLE BUFFERING */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

/* DRAW ARENA FOR GAME */
void drawArena(void) {
		graphics->setTextSize(1);
		graphics->fillScreen(background);
		//graphics->drawRect(5, 5, 470, 20, WHITE);
		graphics->setCursor(140,260);
		graphics->printf("2017 SCOTT PARKINSON - W15013727"); 
}

/* DRAW ASTEROIDS LOGO */
void drawLogo(void) {
		graphics->setTextSize(7);
		graphics->setCursor(50,50);
		graphics->printf("ASTEROIDS"); 
		graphics->setTextSize(2);
		graphics->setCursor(30,150);
		graphics->printf("PRESS CENTRE JOYSTICK TO START GAME"); 
		graphics->setTextSize(1);
		//CONTROLS
		graphics->drawRect(50, 180, 380, 70, WHITE);
		graphics->setCursor(80,200);
		graphics->printf("LEFT  - Anti-clockwise"); 
		graphics->setCursor(260,200);
		graphics->printf("RIGHT - Clockwise"); 
		graphics->setCursor(80,220);
		graphics->printf("UP    - Thrust"); 
		graphics->setCursor(260,220);
		graphics->printf("DOWN  - Activate Shield"); 
}

/* DRAW GAME OVER */
void drawGameOver(void) {
		int printedScore = score/30;
		graphics->setTextSize(7);
		graphics->setCursor(50,50);
		graphics->printf("GAME OVER"); 
		graphics->setTextSize(2);
		graphics->setCursor(210,130);
		graphics->printf("SCORE"); 
		graphics->setTextSize(3);
		graphics->setCursor(210,160);
		graphics->printf("%i", printedScore);
		graphics->setTextSize(2);
		graphics->setCursor(85,210);
		graphics->printf("PRESS UP & CENTRE JOYSTICK");
		graphics->setCursor(145,230);
		graphics->printf("TO RESTART GAME"); 
		graphics->setTextSize(1);
}

/* DRAW LIVES ON-SCREEN */
void drawLives(void) {
		int x1 = 20;
		int x2 = 30;
		int x3 = 40;
		int x4 = 50;
		int x5 = 60;
		int y = 40;
		if(lives == 5) {
				graphics->drawTriangle(x1+5,y+5,x1,y-5,x1-5,y+5,WHITE);
				graphics->drawTriangle(x2+5,y+5,x2,y-5,x2-5,y+5,WHITE);
				graphics->drawTriangle(x3+5,y+5,x3,y-5,x3-5,y+5,WHITE);
				graphics->drawTriangle(x4+5,y+5,x4,y-5,x4-5,y+5,WHITE);
				graphics->drawTriangle(x5+5,y+5,x5,y-5,x5-5,y+5,WHITE);
		}
		else if(lives == 4) {
				graphics->drawTriangle(x1+5,y+5,x1,y-5,x1-5,y+5,WHITE);
				graphics->drawTriangle(x2+5,y+5,x2,y-5,x2-5,y+5,WHITE);
				graphics->drawTriangle(x3+5,y+5,x3,y-5,x3-5,y+5,WHITE);
				graphics->drawTriangle(x4+5,y+5,x4,y-5,x4-5,y+5,WHITE);
		}
		else if(lives == 3) {
				graphics->drawTriangle(x1+5,y+5,x1,y-5,x1-5,y+5,WHITE);
				graphics->drawTriangle(x2+5,y+5,x2,y-5,x2-5,y+5,WHITE);
				graphics->drawTriangle(x3+5,y+5,x3,y-5,x3-5,y+5,WHITE);
		}
		else if(lives == 2) {
				graphics->drawTriangle(x1+5,y+5,x1,y-5,x1-5,y+5,WHITE);
				graphics->drawTriangle(x2+5,y+5,x2,y-5,x2-5,y+5,WHITE);
		}
		else if(lives == 1) {
				graphics->drawTriangle(x1+5,y+5,x1,y-5,x1-5,y+5,RED);
		}
}

/* DRAW SHIP ON-SCREEN */
void drawShip(void) {
		if(lives > 1) {
				graphics->drawLine(shipOriginX+shipLeftX, shipOriginY+shipLeftY, shipOriginX+shipPointX, shipOriginY+shipPointY, WHITE);
				graphics->drawLine(shipOriginX+shipLeftX, shipOriginY+shipLeftY, shipOriginX+shipRightX, shipOriginY+shipRightY, WHITE);
				graphics->drawLine(shipOriginX+shipRightX, shipOriginY+shipRightY, shipOriginX+shipPointX, shipOriginY+shipPointY, WHITE);
		}
		else {
				graphics->drawLine(shipOriginX+shipLeftX, shipOriginY+shipLeftY, shipOriginX+shipPointX, shipOriginY+shipPointY, RED);
				graphics->drawLine(shipOriginX+shipLeftX, shipOriginY+shipLeftY, shipOriginX+shipRightX, shipOriginY+shipRightY, RED);
				graphics->drawLine(shipOriginX+shipRightX, shipOriginY+shipRightY, shipOriginX+shipPointX, shipOriginY+shipPointY, RED);
		}
}

/* DRAW SHIELDS FOR SHIP ON-SCREEN */
void drawShield(void) {	
		graphics->setTextSize(2);
		graphics->fillRect(120,16,120,20, background);
		
		//Shield recharges every 30 seconds
		if((score/30) % 30 == 00) {
				shieldReady = true;
		}
		if(shieldReady == false && shieldState == 0) {
				graphics->setCursor(150,16);
				graphics->printf("Recharging Shields");
		}
		else if(shieldReady == true && shieldState == 0) {
				graphics->setCursor(150,16);
				graphics->printf("Shields Recharged");
		}
		else if(shieldState == 1) {
				graphics->setCursor(150,16);
				graphics->printf("Shields Failing!");
				graphics->drawCircle(shipOriginX, shipOriginY, 13, rgb(51,204,255));
		}
		else if(shieldState == 2) {
				graphics->setCursor(150,16);
				graphics->printf("Shields Holding");
				graphics->drawCircle(shipOriginX, shipOriginY, 13, rgb(51,204,255));
				graphics->drawCircle(shipOriginX, shipOriginY, 15, rgb(0,153,255));
		}
		else if(shieldState == 3) {
				graphics->setCursor(150,16);
				graphics->printf("Shields Online");
				graphics->drawCircle(shipOriginX, shipOriginY, 13, rgb(51,204,255));
				graphics->drawCircle(shipOriginX, shipOriginY, 15, rgb(0,153,255));
				graphics->drawCircle(shipOriginX, shipOriginY, 17, rgb(0,0,255));
		}
}

/* DRAW MISSILES ON-SCREEN */
void drawMissiles(struct missile *missileList) {
		//Cycle through the linked list and draw all exiting missiles
		while(missileList) {
				graphics->fillCircle(missileList->missileX, missileList->missileY, 1, GREEN);
				missileList = missileList->next;
		}
}

/* DRAW ASTEROIDS ON-SCREEN */
void drawAsteroids(struct asteroid *asteroidList) {
		//Cycle through the linked list and draw all existing asteroids
		while(asteroidList) {
				graphics->drawCircle(asteroidList->asteroidX, asteroidList->asteroidY, asteroidList->asteroidSize, WHITE);
				asteroidList = asteroidList->next;
		}
}

/* DRAW SHIP EXPLOSION - UNUSED FUNCTION*/
void drawShipExplosion(void) {
		int explosion;
		explosion++;
		graphics->fillCircle(shipOriginX, shipOriginY, 12, YELLOW);
		if(explosion > 10000) {
				graphics->fillCircle(shipOriginX, shipOriginY, 16, RED);
		}
		if(explosion > 20000) {
			graphics->fillCircle(shipOriginX, shipOriginY, 20, RED);
		}
		if(explosion > 30000) {
				explosion = 0;
		}
}

/* DRAW SCORE */
void drawScore(void) {
		score++;
		
		int printedScore = score/30;
		//Score Counter
		graphics->setTextSize(2);
		graphics->setCursor(16,16);
		graphics->printf("%i", printedScore);
}

void draw(void)
{
		//Draw Arena
		drawArena();
		
		//If game hasn't started show start screen
		if(gameStart == false && lives > 0) {
				lives = 5;
				drawLogo();
		}
		//If game has started and lives is greater than 0
		else if (gameStart == true && lives > 0) {
				drawShip();
				drawLives();
				drawShield();
				drawScore();
				drawAsteroids(asteroidActive);
				drawMissiles(missileActive);
		}
		//If game has started but lives is now 0
		else if (lives == 0) {
				drawGameOver();
		}
    
    swap_DBuffer();
}
