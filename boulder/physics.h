#ifndef __PHYSICS__
#define __PHYSICS__

#include "animations.h"


#define HERO_W 32
#define HERO_H 32

#define HERO_SPEED 3
#define HERO_MAX_X (BUFFER_W - HERO_W)
#define HERO_MAX_Y (BUFFER_H - HERO_H)

/* KEYBOARD STUFF */
#define KEY_SEEN 1
#define KEY_RELEASED 2

typedef enum Direction
{
    DOWN,
    UP,
    STOPPED,
    LEFT,
    RIGHT
} Direction;

typedef struct HERO
{
    int x, y, shot_timer, lives, respawn_timer, invincible_timer, sourceX, sourceY;
    bool active;
    Direction direction;
} HERO;

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

void sprites_init(SPRITES *sprites);

void sprites_deinit(SPRITES *sprites);

void hero_init(HERO *hero);

void hero_draw(HERO *hero, SPRITES *sprites);

void move_hero(HERO *hero, SPRITES *sprites, unsigned char key[ALLEGRO_KEY_MAX]);

void keyboard_init(unsigned char *key);

void keyboard_update(ALLEGRO_EVENT *event, unsigned char *key);

#endif