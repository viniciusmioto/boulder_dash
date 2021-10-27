#include "animations.h"

void must_init(bool test, const char *description)
{
    if (test)
        return;

    fprintf(stderr, "couldn't initialize %s\n", description);
    exit(1);
}

void sprites_init(SPRITES *sprites)
{
    sprites->hero = al_load_bitmap("sprites.png");
    must_init(sprites->hero, "sprites");
}

void sprites_deinit(SPRITES *sprites)
{
    al_destroy_bitmap(sprites->hero);
}