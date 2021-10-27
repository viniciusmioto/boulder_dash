#ifndef __ANIMATIONS__
#define __ANIMATIONS__

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

typedef struct SPRITES
{
    ALLEGRO_BITMAP *hero;
} SPRITES;

void must_init(bool test, const char *description);

void sprites_init(SPRITES *sprites);

void sprites_deinit(SPRITES *sprites);

#endif