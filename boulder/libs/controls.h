#ifndef __DISPLAY__
#define __DISPLAY__

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

/* map elements */
#define EMPTY 0
#define DIRT 1
#define BOULDER 2
#define WALL 3
#define BRICK 4
#define DIAMOND 5
#define EXIT 6
#define EXPLOSION 7

/* width and heigh dimensions */
#define BUFFER_W 1280
#define BUFFER_H 736

/* we can modify this to change the size of the display */
#define DISP_SCALE 1

/* real size that is going to appear on the screen */
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

/* tile objects */
#define TILE_SIZE 32

/* map dimensions */
#define MAP_W 40
#define MAP_H 23

/* KEYBOARD STUFF */
#define KEY_SEEN 1
#define KEY_RELEASED 2

typedef struct SPRITES
{
    ALLEGRO_BITMAP *_spritesheet;
    ALLEGRO_BITMAP *hero;
    ALLEGRO_BITMAP *diamond;
    ALLEGRO_BITMAP *boulder;
    ALLEGRO_BITMAP *dirt;
    ALLEGRO_BITMAP *wall;
    ALLEGRO_BITMAP *brick;
    ALLEGRO_BITMAP *exit;
    ALLEGRO_BITMAP *explosion;
} SPRITES;

void must_init(bool test, const char *description);

void sprites_init(SPRITES *sprites);

void sprites_deinit(SPRITES *sprites);

void disp_init(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

void disp_deinit(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

void disp_pre_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

void disp_post_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

void keyboard_init(unsigned char *key);

void keyboard_update(ALLEGRO_EVENT *event, unsigned char *key);

void load_map(const char *fileName, int map[23][40]);

void draw_map(int map[23][40], SPRITES *sprites, int counter);

#endif