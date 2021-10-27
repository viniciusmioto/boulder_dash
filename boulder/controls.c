#include "controls.h"
/* ** All the following functions are recommended by the Allegro tutorial ** */

/* Verify if an specific initialization went right */
void must_init(bool test, const char *description)
{
    if (test)
        return;

    fprintf(stderr, "couldn't initialize %s\n", description);
    exit(1);
}

/* Initialize our sprites */
void sprites_init(SPRITES *sprites, char fileName[100])
{
    sprites->hero = al_load_bitmap(fileName);
    must_init(sprites->hero, "sprites");
}

/* Free memory of sprites */
void sprites_deinit(SPRITES *sprites)
{
    al_destroy_bitmap(sprites->hero);
}

/* Initialize our display */
void disp_init(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    /* set our display */
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    /* create display and buffer */
    *display = al_create_display(DISP_W, DISP_H);
    must_init(*display, "display");

    *buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(*buffer, "bitmap buffer");
}

/* Free memory of display */
void disp_deinit(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_destroy_bitmap(*buffer);
    al_destroy_display(*display);
}

/* Pre-draw the display (buffer) */
void disp_pre_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_target_bitmap(*buffer);
}

/* Update the display (buffer) */
void disp_post_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_target_backbuffer(*display);
    al_draw_scaled_bitmap(*buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
    al_flip_display();
}

/* Initialize the keyboard */
void keyboard_init(unsigned char *key)
{
    memset(key, 0, sizeof(key));
}

/* Check for events updates with the keyboard (Allegro Template Function) */
void keyboard_update(ALLEGRO_EVENT *event, unsigned char *key)
{
    /* check if the key is pressed */
    if (event->type == ALLEGRO_EVENT_KEY_DOWN)
        key[event->keyboard.keycode] = 1;
    /* check if the key is being held or released */
    else if (event->type == ALLEGRO_EVENT_KEY_UP)
    {
        switch (event->type)
        {
        case ALLEGRO_EVENT_TIMER:
            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
        }
    }
}