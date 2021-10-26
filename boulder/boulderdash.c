#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "physics.h"

/* GENERAL STUFF */

void must_init(bool test, const char *description)
{
    if (test)
        return;

    fprintf(stderr, "couldn't initialize %s\n", description);
    exit(1);
}

/* DISPLAY STUFF */

#define BUFFER_W 426
#define BUFFER_H 245

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *buffer;

void disp_init()
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    display = al_create_display(DISP_W, DISP_H);
    must_init(display, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}

void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}

/* KEYBOARD STUFF */

#define KEY_SEEN 1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT *event)
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

/* SPRITES */

#define HERO_W 32
#define HERO_H 32

typedef struct SPRITES
{
    ALLEGRO_BITMAP *hero;
} SPRITES;
SPRITES sprites;

void sprites_init()
{
    sprites.hero = al_load_bitmap("sprites.png");
    must_init(sprites.hero, "sprites");
}

void sprites_deinit()
{
    al_destroy_bitmap(sprites.hero);
}

typedef enum Direction
{
    DOWN,
    STOPPED,
    UP,
    LEFT,
    RIGHT
} Direction;

#define HERO_SPEED 3
#define HERO_MAX_X (BUFFER_W - HERO_W)
#define HERO_MAX_Y (BUFFER_H - HERO_H)

typedef struct HERO
{
    int x, y;
    int shot_timer;
    int lives;
    int respawn_timer;
    int invincible_timer;
} HERO;
HERO hero;

void hero_init()
{
    hero.x = (BUFFER_W / 2) - (HERO_W / 2);
    hero.y = (BUFFER_H / 2) - (HERO_H / 2);
}

void hero_update(int *sourceX, int *sourceY, bool *active, Direction direction)
{
    /* movement */
    if (key[ALLEGRO_KEY_LEFT])
    {
        hero.x -= HERO_SPEED;
        direction = LEFT;
    }
    else if (key[ALLEGRO_KEY_RIGHT])
    {
        hero.x += HERO_SPEED;
        direction = RIGHT;
    }
    else if (key[ALLEGRO_KEY_UP])
    {
        hero.y -= HERO_SPEED;
        direction = UP;
    }
    else if (key[ALLEGRO_KEY_DOWN])
    {
        hero.y += HERO_SPEED;
        direction = DOWN;
    }
    else
    {
        direction = STOPPED;
        // *active = false;
    }

    if (*active)
    {
        *sourceX += al_get_bitmap_width(sprites.hero) / 8;
    }
    else
    {
        *sourceX = 32;
    }

    if (*sourceX >= al_get_bitmap_width(sprites.hero))
    {
        *sourceX = 0;
    }

    *sourceY = direction;

    /* bounds */
    if (hero.x < 0)
        hero.x = 0;
    if (hero.y < 0)
        hero.y = 0;
    if (hero.x > HERO_MAX_X)
        hero.x = HERO_MAX_X;
    if (hero.y > HERO_MAX_Y)
        hero.y = HERO_MAX_Y;
}

void hero_draw(int sourceX, int sourceY)
{

    al_draw_bitmap_region(sprites.hero, sourceX, sourceY * al_get_bitmap_height(sprites.hero) / 5, 32, 32, hero.x, hero.y, 0);

}

int main()
{
    /* INIT */
    long frames = 0;
    long score = 0;
    int sourceX = 0, sourceY = 0;
    bool done = false, redraw = true, active = false;

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();
    Direction direction = STOPPED;

    must_init(al_init_image_addon(), "image");
    sprites_init();

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    hero_init();

    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            active = true;
            hero_update(&sourceX, &sourceY, &active, direction);
            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            redraw = true;
            frames++;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        keyboard_update(&event);

        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            hero_draw(sourceX, sourceY);
            disp_post_draw();
            redraw = false;
        }
    }

    sprites_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
