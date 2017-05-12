/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Some insitial struct types if you want to usd them */
struct ship {
    coordinate_t p;
    vector_t     v;
}; 

/* initial struts for building linked lists */
typedef struct asteroid {
    float asteroidX;
		float asteroidY;
		float asteroidVelocityX;
		float asteroidVelocityY;
		int asteroidSize;
		float asteroidTTL;
    struct asteroid *next;
} asteroid_t;

typedef struct missile {
    float missileX;
		float missileY;
		float missileVelocityX;
		float missileVelocityY;
		float missileList;
		float missileTTL;
    struct missile *next;
} missile_t;

/* FUNCTIONS */
void physics(void);
void asteroidFreeNode(asteroid_t *i);
void missileFreeNode(missile_t *i);
void intialiseAsteroidHeap(void);
void intialiseMissileHeap(void);
void fireMissile(void);
void spawnAsteroid(void);

/* NODES */
asteroid_t *allocateNodeAsteroid(void);
missile_t *allocateNodeMissile(void);

/* EXTERNAL VARIABLES */
extern double shipSpeed;
extern float shipOriginX;
extern float shipOriginY;
extern float shipPointX;
extern float shipPointY;
extern float shipLeftX;
extern float shipLeftY;
extern float shipRightX;
extern float shipRightY;
extern struct missile *missileActive;
extern struct asteroid *asteroidActive;
extern const int asteroidHeapsize;
extern const int missileHeapsize;
extern int asteroidSize;

