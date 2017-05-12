/* Game state */

extern float elapsed_time; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int   lives;        /* lives remaining */

extern struct ship player;

extern struct rock *asteroids; /* array of rocks / pointer to linked-list */
extern struct missile *shots;  /* array of missiles / pointer to linked-list */

extern const float Dt; /* Time step for physics, needed for consistent motion */

extern bool 	gameStart; 	/* determines whether to display Game Menu and Game Over screens */
extern int shieldState;		/* the strength of the shield */
extern int shipTrajectory;	/* the heading trajectory of the ship */
extern bool shieldReady;	/* determines whether the shield has recharged and activated */

void masterReset();			/* the master reset function for use in controller.cpp */
