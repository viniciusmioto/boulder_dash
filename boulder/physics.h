#ifndef __PHYSICS__
#define __PHYSICS__

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

/* DISPLAY STUFF */

#define BUFFER_W 426
#define BUFFER_H 245

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)


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

typedef struct SPRITES
{
    ALLEGRO_BITMAP *hero;
} SPRITES;

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

void sprites_init(SPRITES *sprites);

void sprites_deinit(SPRITES *sprites);

void hero_init(HERO *hero);

void hero_draw(HERO *hero, SPRITES *sprites);

void move_hero(HERO *hero, SPRITES *sprites, unsigned char key[ALLEGRO_KEY_MAX]);

#endif