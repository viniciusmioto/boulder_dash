#ifndef __PHYSICS__
#define __PHYSICS__

#include "controls.h"

#define HERO_W 32
#define HERO_H 32

#define HERO_SPEED 32
#define HERO_MAX_X (BUFFER_W - HERO_W)
#define HERO_MAX_Y (BUFFER_H - HERO_H)

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
    int sourceX, sourceY, easteregg, mapX, mapY, diamonds;
    long score;
    char name[10];
    bool win, lose;
    ALLEGRO_BITMAP *sprites;
    Direction direction;
} HERO;

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

void hero_init(HERO *hero);

void hero_draw(HERO *hero, SPRITES *sprites);

void move_hero(HERO *hero, SPRITES *sprites, SAMPLES *samples, unsigned char key[ALLEGRO_KEY_MAX], int counter, int map[23][40]);

void verify_easter_egg(HERO *hero, unsigned char key[ALLEGRO_KEY_MAX]);

#endif