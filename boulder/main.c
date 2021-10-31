#include "./libs/physics.h"
#include "./libs/scores.h"

int main(int argc, char **argv)
{
    /* -- INIT -- */

    /* Structures */
    HERO hero;
    SPRITES sprites;
    LinkedList scores_list;

    /* Allegro Components */
    ALLEGRO_FONT *font;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *buffer = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_EVENT event;

    char name[25];

    /* Read txt with Scores List */
    init_list(&scores_list);
    read_scores(&scores_list);

    /* Game Variables */
    long frames = 0;
    bool done = false, redraw = true;
    unsigned char key[ALLEGRO_KEY_MAX];
    int activate_easter_egg = 0,
        loadCounterX = 0, loadCounterY = 0,
        mapSizeX = 0, mapSizeY = 0;
    int map[MAP_H][MAP_W];

    /* Init Allegro Components */
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init(&display, &buffer);

    font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image");
    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    hero_init(&hero);

    /* if -n is passed, use the argument as the hero name */
    if (argc == 3 && strcmp(argv[1], "-n") == 0)
        strcpy(hero.name, argv[2]);
    else
        strcpy(hero.name, "Rockford");

    keyboard_init(key);
    sprites_init(&sprites);

    load_map("./resources/map.txt", map);

    /* MAIN LOOP */
    while (!done)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            move_hero(&hero, &sprites, key, event.timer.count, map);
            verify_easter_egg(&hero, key);

            if (key[ALLEGRO_KEY_ESCAPE])
            /* restart game */
            {
                if (event.timer.count % 5 == 0)
                {
                    save_score(&scores_list, hero.score, hero.name);
                    write_scores(scores_list);
                    hero_init(&hero);
                    load_map("./resources/map.txt", map);
                    al_set_timer_count(timer, 0);
                    al_resume_timer(timer);
                }
            }
            if (key[ALLEGRO_KEY_F1])
            /* info */
            {
                /* nothing to do */
            }
            if (key[ALLEGRO_KEY_Q])
            {
                done = true;
            }

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

            draw_map(map, &sprites, event.timer.count);
            hud_draw(font, 150 - event.timer.count / 60, hero.score);

            if (hero.lose)
            {
                al_draw_text(
                    font,
                    al_map_rgb_f(1, 1, 1),
                    (DISP_W / 2), TILE_SIZE / 2,
                    ALLEGRO_ALIGN_CENTER,
                    "G A M E  O V E R !");

                if (event.timer.count % 71 == 0)
                {
                    save_score(&scores_list, hero.score, hero.name);
                    write_scores(scores_list);
                    hero_init(&hero);
                    load_map("./resources/map.txt", map);
                    al_set_timer_count(timer, 0);
                    al_resume_timer(timer);
                }
            }
            else if (hero.win)
            {
                al_draw_text(
                    font,
                    al_map_rgb_f(1, 1, 1),
                    (DISP_W / 2), TILE_SIZE / 2,
                    ALLEGRO_ALIGN_CENTER,
                    "Y O U  W I N !");

                al_stop_timer(timer);
                hero.score += (150 - event.timer.count / 60);
            }
            else
                hero_draw(&hero, &sprites);

            disp_post_draw(&display, &buffer);
            redraw = false;
        }
    }

    save_score(&scores_list, hero.score, hero.name);
    write_scores(scores_list);

    sprites_deinit(&sprites);
    disp_deinit(&display, &buffer);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    deallocate_list(&scores_list);

    return 0;
}
