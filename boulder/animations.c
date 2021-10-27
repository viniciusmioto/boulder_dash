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

void disp_init(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    *display = al_create_display(DISP_W, DISP_H);
    must_init(*display, "display");

    *buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(*buffer, "bitmap buffer");
}

void disp_deinit(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_destroy_bitmap(*buffer);
    al_destroy_display(*display);
}

void disp_pre_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_target_bitmap(*buffer);
}

void disp_post_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_target_backbuffer(*display);
    al_draw_scaled_bitmap(*buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
    al_flip_display();
}