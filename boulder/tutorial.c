#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#define BUFFER_W 426
#define BUFFER_H 245

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

typedef enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

int main(void)
{
    ALLEGRO_DISPLAY *display;


    const int FPS = 60;
    Direction direction = DOWN;

    if (!al_init())
    {
        fprintf(stderr, "couldn't initialize");
        return -1;
    }

    display = al_create_display(DISP_W, DISP_H);

    if (!display)
    {
        fprintf(stderr, "couldn't create display");
        return -1;
    }

    al_set_window_title(display, "Boulder Dash");

    al_install_keyboard();
    al_init_image_addon();

    bool done = false, redraw = true, activate = false;
    float x = 10, y = 10;
    int moveSpeed = 5, sourceX = 0, sourceY = 0;

    ALLEGRO_BITMAP *player = al_load_bitmap("sprites.png");

    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    while (!done)
    {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        al_get_keyboard_state(&keyState);

        if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if (events.type == ALLEGRO_EVENT_TIMER)
        {
            activate = true;
            if (al_key_down(&keyState, ALLEGRO_KEY_UP))
            {
                y -= moveSpeed;
                direction = UP;
            }
            else if (al_key_down(&keyState, ALLEGRO_KEY_DOWN))
            {
                y += moveSpeed;
                direction = DOWN;
            }
            else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT))
            {
                x -= moveSpeed;
                direction = LEFT;
            }
            else if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
            {
                x += moveSpeed;
                direction = RIGHT;
            }
            else
            {
                activate = false;
            }

            if (activate)
            {
                sourceX += al_get_bitmap_width(player) / 8;
            }
            else
            {
                sourceX = 32;
            }

            if (sourceX >= al_get_bitmap_width(player))
            {
                sourceX = 0;
            }

            sourceY = direction;

            redraw = true;
        }

        if (redraw)
        {
            al_draw_bitmap_region(player, sourceX, sourceY * al_get_bitmap_height(player)/4, 32, 32, x, y, 0);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
