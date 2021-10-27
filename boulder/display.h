#ifndef __ANIMATIONS__
#define __ANIMATIONS__

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#define BUFFER_W 1280
#define BUFFER_H 736


#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)


typedef struct SPRITES
{
    ALLEGRO_BITMAP *hero;
} SPRITES;

void must_init(bool test, const char *description);

void sprites_init(SPRITES *sprites);

void sprites_deinit(SPRITES *sprites);

void disp_init(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

void disp_deinit(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

void disp_pre_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

void disp_post_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);

#endif