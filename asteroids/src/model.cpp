/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

/* STATIC VARIABLES */
static const int missileHeapsize = 4; 	//Original Asteroids only allowed 4 missiles on-screen at one time
static const int asteroidHeapsize = 10;	//10 Asteroids
static missile_t missileHeap[missileHeapsize];
static asteroid_t asteroidHeap[asteroidHeapsize];
static missile_t *missileFreeNodes;
static asteroid_t *asteroidFreeNodes;

/* STRUCTURES FOR LINKED LISTS */
struct missile *missileActive = NULL;
struct asteroid *asteroidActive = NULL;

/* VARIABLES FOR SHIP MOVEMENT, SPEED & ASTEROID SIZE */
float shipPointX;
float shipPointY;
float shipLeftX;
float shipLeftY;
float shipRightX;
float shipRightY;
float shipOriginX = 230;
float shipOriginY = 120;
double shipSpeed = 0;
float headingTrajectory;
int asteroidSize;

/* INTIALISE MISSILE AND ASTEROID LINKED LISTS */
//Intialise the heap for Missile structure
void intialiseMissileHeap(void) {
		int i;
		for(i = 0; i < (missileHeapsize-1); i++) {
				missileHeap[i].next = &missileHeap[i+1];
		}
		missileHeap[i].next = NULL;
		missileFreeNodes = &missileHeap[0];
}

//Intialise the heap for Asteroid structure
void intialiseAsteroidHeap(void) {
		int i;
		for(i = 0; i < (asteroidHeapsize-1); i++) {
				asteroidHeap[i].next = &asteroidHeap[i+1];
		}
		asteroidHeap[i].next = NULL;
		asteroidFreeNodes = &asteroidHeap[0];
}

/* ALLOCATE NODES FOR MISSILE AND ASTEROID LINKED LISTS */
//Allocate nodes for Missile structure
missile_t *allocateNodeMissile(void) {
		missile_t *missileNode = NULL;
		if(missileFreeNodes) {
				missileNode = missileFreeNodes;
				missileFreeNodes = missileFreeNodes->next;
		}
		return missileNode;
}

//Allocate nodes for Asteroid structure
asteroid_t *allocateNodeAsteroid(void) {
		asteroid_t *asteroidNode = NULL;
		if(asteroidFreeNodes) {
				asteroidNode = asteroidFreeNodes;
				asteroidFreeNodes = asteroidFreeNodes->next;
		}
		return asteroidNode;
}

/* SET DEFAULT ORIGINS OF MISSILE AND ASTEROID SPAWNS */
//Missile Origin spawns from the spear-point of the ship
void missileOrigin(struct missile *missileO) {
		int missileVelocity = 200;
		missileO->missileX = shipOriginX;
		missileO->missileY = shipOriginY;
		missileO->missileVelocityX = missileVelocity * (cos(headingTrajectory));
		missileO->missileVelocityY = missileVelocity * (sin(headingTrajectory));
		missileO->missileTTL = 200;
}

//Asteroid Origin spawns at a random position
void asteroidOrigin(struct asteroid *asteroidO) {
		int asteroidSpeed = randrange(30,90);
		int shipRadiusProtect = 60;		//Set at double the ships radius to prevent asteroid spawn right next to ship
		asteroidO->asteroidX = randrange(20,460);
		asteroidO->asteroidY = randrange(20,240);
		//Prevent asteroid spawning next to Ship
		//If asteroid origin matches ships current origin pick another random origin for the asteroid
		if(((asteroidO->asteroidX) > (shipOriginX - shipRadiusProtect) && (asteroidO->asteroidX) < (shipOriginX + shipRadiusProtect)) && ((asteroidO->asteroidY) > (shipOriginY - shipRadiusProtect) && (asteroidO->asteroidY) < (shipOriginY + shipRadiusProtect))) {
				asteroidO->asteroidX = randrange(20,460);
				asteroidO->asteroidY = randrange(20,240);
		}
		asteroidO->asteroidSize = randrange(10,30);	//Each asteroid has a speed between 10 and 30
		asteroidO->asteroidVelocityX = randrange(-asteroidSpeed,asteroidSpeed);
		asteroidO->asteroidVelocityY = randrange(-asteroidSpeed,asteroidSpeed);
		asteroidO->asteroidTTL = randrange(900,1100);
}

/* SCREEN WRAP */
void screenWrap(struct asteroid *a) {
		//Goes off right of screen
		if(shipOriginX > 480) {
				shipOriginX = 5;
		}
		//Goes off left of screen
		else if(shipOriginX < 0) {
				shipOriginX = 475;
		}
		//Goes off bottom of screen
		else if(shipOriginY > 260) {
				shipOriginY = 5;
		}
		//Goes off top of screen
		else if(shipOriginY < 0) {
				shipOriginY = 255;
		}
		//ASTEROID WRAP
		for(; a ; a = a->next) {
				if(a->asteroidX > 480) {
						a->asteroidX = 0;
				}
				else if(a->asteroidX < 0) {
						a->asteroidX = 480;
				}
				else if(a->asteroidY > 260) {
						a->asteroidY = 0;
				}
				else if(a->asteroidY < 0) {
						a->asteroidY = 260;
				}
		}
}

/* COLLISION CHECKS */
//Check for Collision between Asteroid and Missile
void missileAsteroidCollisionCheck(struct asteroid *a, struct missile *m) {
		for(; m ; m = m->next) {	//For every missile
				for(; a ; a = a->next) {	//Check each asteroid
						if((m->missileX > (a->asteroidX - 27) && m->missileX < (a->asteroidX + 27))
								&& (m->missileY > (a->asteroidY - 27) && m->missileY < (a->asteroidY + 27))) {
								//Delete relevant missile and asteroid
								m->missileTTL = 0;
								a->asteroidTTL = 0;
						}
				}
		}
}

//Check for Collision between Asteroid and another Asteroid
/* UN-USED
void asteroidAsteroidCollisionCheck(struct asteroid *a, struct asteroid *b) {
		for(; a ; a = a->next) {
				for(; b ; b = b->next) {
						float asteroid1X = a->asteroidX;
						float asteroid1Y = a->asteroidY;
						float asteroid2X = b->asteroidX;
						float asteroid2Y = b->asteroidY;
						
						//THIS COULD BE INVERTED WITH THE USE OF A !
						if(asteroid1X != asteroid2X && asteroid1Y != asteroid2Y) {
								//DO SOMETHING
						}
					
						if((a->asteroidX > (a->asteroidX - 27) && m->missileX < (a->asteroidX + 27))
								&& (m->missileY > (a->asteroidY - 27) && m->missileY < (a->asteroidY + 27))) {
									
								}
				}
		}
}
*/

//Check for Collision between Asteroid and Ship - sets shield radius
void shipAsteroidCollisionCheck(struct asteroid *a) {
		for(; a ; a = a->next) {
				int shieldRadius;
				if(shieldState == 3) {
						//Larger radius when shield is online
						shieldRadius = 25;
				}
				else if (shieldState == 2) {
						//Reduced radius when shield is holding
						shieldRadius = 24;
				}
				else if (shieldState == 1) {
						//Reduced radius when shield is failing
						shieldRadius = 23;
				}
				else {
						//Smaller radius when shield is offline
						shieldRadius = 15;
				}
				int asteroidRadius = a->asteroidSize;
				int totalRadius = (asteroidRadius/2) + (shieldRadius);
				if(((a->asteroidX) > (shipOriginX - totalRadius) && (a->asteroidX) < (shipOriginX + totalRadius)) && ((a->asteroidY) > (shipOriginY - totalRadius) && (a->asteroidY) < (shipOriginY + totalRadius))) {
						//Destroy Asteroid
						a->asteroidTTL = 0;
						if(shieldState > 0) {
								//Reduce shield level and reset ship co-ordinates
								shieldState = shieldState -1;
								shipOriginX = 230;
								shipOriginY = 120;
								shipSpeed = 0;
						}
						else if (lives > 0){
								//Reduce lifes and reset ship co-ordinates
								lives = lives -1;
								shipOriginX = 230;
								shipOriginY = 120;
								shipSpeed = 0;
						}
				}
		}
}

//Fire Missile Function
void fireMissile(void) {
		struct missile *fire = allocateNodeMissile();
		if(fire) {
				fire->next = missileActive;
				missileActive = fire;
				missileOrigin(fire);		
		}
}

//Spawn Asteroid Function
void spawnAsteroid(void) {
		struct asteroid *spawn = allocateNodeAsteroid();
		if(spawn) {
				spawn->next = asteroidActive;
				asteroidActive = spawn;
				asteroidOrigin(spawn);
		}
}

//Missile Status
void missileStatus(struct missile *m) {
		for( ; m; m = m->next) {
				m->missileX += m->missileVelocityX * Dt;
				m->missileY += m->missileVelocityY * Dt;
				//If missile reaches the end of the screen
				if(m->missileX < 0 || 480 < m->missileX) {
						m->missileTTL = 0;
				}
				if(m->missileY < 10 || 260 < m->missileY) {
						m->missileTTL = 0;
				}
				m->missileTTL -=Dt;
				
				if(m->next->missileTTL<=0) {
						struct missile *mDestroyed = m->next;
						m->next = m->next->next;
						missileFreeNode(mDestroyed);
				}
		}
}

//Asteroid Status
void asteroidStatus (struct asteroid *a) {
		for( ; a; a = a->next) {
				a->asteroidX += a-> asteroidVelocityX * Dt;
				a->asteroidY += a-> asteroidVelocityY * Dt;
			
				if(a->next->asteroidTTL <= 0) {
						struct asteroid *aDestroyed = a->next;
						a->next = a->next->next;
						asteroidFreeNode(aDestroyed);
				}
		}
}

/* MANAGE SHIP MOVEMENT */
void manageShipMovement(void) {
		//Convert shipTrajectory into radians
		headingTrajectory = radians(shipTrajectory);
		shipOriginX = shipOriginX + shipSpeed * (cos(headingTrajectory));
		shipOriginY = shipOriginY + shipSpeed * (sin(headingTrajectory));
	
		shipPointX = 10 * (cos(headingTrajectory));
		shipPointY = 10 * (sin(headingTrajectory));
		
		shipLeftX = (-5 *(cos(headingTrajectory))) - (5 * (sin(headingTrajectory)));
		shipLeftY = (-5 *(sin(headingTrajectory))) + (5 * (cos(headingTrajectory)));
	
		shipRightX = (-5 *(cos(headingTrajectory))) - (-5 * (sin(headingTrajectory)));
		shipRightY = (-5 *(sin(headingTrajectory))) + (-5 * (cos(headingTrajectory)));
}

/* MANAGE SHIP SPEED */
void manageShipSpeed(void) {
		//Reduce ship velocity when thrust isn't enabled
		if(shipSpeed > 0.01) {
				shipSpeed = shipSpeed - 0.01;
		}
}

/* FREE NODES */
//Free Missile Nodes
void missileFreeNode(missile_t *freeMissile) {
		freeMissile->next = missileFreeNodes;
		missileFreeNodes = freeMissile;
}

//Free Asteroid Nodes
void asteroidFreeNode(asteroid_t *freeAsteroid) {
		freeAsteroid->next = asteroidFreeNodes;
		asteroidFreeNodes = freeAsteroid;
}

/* PHYSICS ENGINE */
void physics(void)
{
		screenWrap(asteroidActive);
		manageShipMovement();
		manageShipSpeed();
		
		missileAsteroidCollisionCheck(asteroidActive, missileActive);
		shipAsteroidCollisionCheck(asteroidActive);	
		asteroidStatus(asteroidActive);
		missileStatus(missileActive);
}
