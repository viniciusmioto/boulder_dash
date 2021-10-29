#include "physics.h"

int main()
{
    /* INIT */
    HERO hero;
    SPRITES sprites;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *buffer = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    long frames = 0;
    bool done = false, redraw = true;
    unsigned char key[ALLEGRO_KEY_MAX];
    int activate_easter_egg = 0, loadCounterX = 0, loadCounterY = 0, mapSizeX = 0, mapSizeY = 0;
    int map[10][10];

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init(&display, &buffer);

    must_init(al_init_image_addon(), "image");

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;

    al_start_timer(timer);
    hero_init(&hero);
    keyboard_init(key);
    sprites_init(&sprites);

    loadMap("map.txt", map);

    /* MAIN LOOP */
    while (!done)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            move_hero(&hero, &sprites, key, event.timer.count);
            verify_easter_egg(&hero, key);
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

        keyboard_update(&event, key);

        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw(&display, &buffer);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            hero_draw(&hero, &sprites);
            draw_map(map, &sprites);
            disp_post_draw(&display, &buffer);
            redraw = false;
        }
    }

    sprites_deinit(&sprites);
    disp_deinit(&display, &buffer);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
