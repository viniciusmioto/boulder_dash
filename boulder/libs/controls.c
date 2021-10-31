#include "controls.h"
/* ** All the following functions with the (AL) label are designed and recommended by the Allegro tutorial ** */

/* (AL) Verify if an specific initialization went right */
void must_init(bool test, const char *description)
{
    if (test)
        return;

    fprintf(stderr, "couldn't initialize %s\n", description);
    exit(1);
}

/* (AL) Select a specific region of the spritesheet */
ALLEGRO_BITMAP *sprite_grab(SPRITES *sprites, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sprites->_spritesheet, x, y, w, h);
    return sprite;
}

/* (AL) Initialize our sprites */
void sprites_init(SPRITES *sprites)
{
    /* Load the spritesheet with all the elements */
    sprites->_spritesheet = al_load_bitmap("./resources/spritesheet.png");
    must_init(sprites->_spritesheet, "map sprites");

    /* Rockford's movements and animations */
    sprites->hero = sprite_grab(sprites, 0, TILE_SIZE, TILE_SIZE * 8, TILE_SIZE * 5);
    must_init(sprites->hero, "map sprites - Rockford");

    /* diamonds' animations */
    sprites->diamond = sprite_grab(sprites, 0, TILE_SIZE * 10, TILE_SIZE * 8, TILE_SIZE);
    must_init(sprites->diamond, "map sprites - diamonds");

    /* boulder tile */
    sprites->boulder = sprite_grab(sprites, 0, TILE_SIZE * 7, TILE_SIZE, TILE_SIZE);
    must_init(sprites->boulder, "map sprites - boulder");

    /* dirt tile */
    sprites->dirt = sprite_grab(sprites, TILE_SIZE, TILE_SIZE * 7, TILE_SIZE, TILE_SIZE);
    must_init(sprites->dirt, "sprites - dirt");

    /* brick tile */
    sprites->brick = sprite_grab(sprites, TILE_SIZE, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
    must_init(sprites->brick, "sprites - brick");

    /* wall tile */
    sprites->wall = sprite_grab(sprites, TILE_SIZE * 3, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
    must_init(sprites->wall, "sprites - wall");

    /* exit tile */
    sprites->exit = sprite_grab(sprites, 0, TILE_SIZE * 6, TILE_SIZE * 2, TILE_SIZE);
    must_init(sprites->exit, "sprites - exit");

    /* exit tile */
    sprites->explosion = sprite_grab(sprites, TILE_SIZE, 0 , TILE_SIZE, TILE_SIZE);
    must_init(sprites->explosion, "sprites - explosion");
}

/* (AL) Free memory of sprites */
void sprites_deinit(SPRITES *sprites)
{
    al_destroy_bitmap(sprites->_spritesheet);
    al_destroy_bitmap(sprites->hero);
    al_destroy_bitmap(sprites->dirt);
    al_destroy_bitmap(sprites->boulder);
    al_destroy_bitmap(sprites->wall);
    al_destroy_bitmap(sprites->brick);
    al_destroy_bitmap(sprites->diamond);
    al_destroy_bitmap(sprites->exit);
    al_destroy_bitmap(sprites->explosion);
}

/* (AL) Initialize our display */
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

/* (AL) Free memory of display */
void disp_deinit(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_destroy_bitmap(*buffer);
    al_destroy_display(*display);
}

/* (AL) Pre-draw the display (buffer) */
void disp_pre_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_target_bitmap(*buffer);
}

/* (AL) Update the display (buffer) */
void disp_post_draw(ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_target_backbuffer(*display);
    al_draw_scaled_bitmap(*buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
    al_flip_display();
}

/* (AL) Initialize the keyboard */
void keyboard_init(unsigned char *key)
{
    memset(key, 0, sizeof(key));
}

/* (AL) Check for events updates with the keyboard (Allegro Template Function) */
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

/* Load the map by using a matrix in text file */
void load_map(const char *fileName, int map[23][40])
{
    int i = 0, j = 0;
    char c;

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            i++;
            j = 0;
        }
        else if (c != ' ')
        {
            map[i][j] = c - '0';
            j++;
        }
    }

    fclose(file);
}

/* Show the map on the screen with some animations */
void draw_map(int map[MAP_H][MAP_W], SPRITES *sprites, int counter)
{
    int i, j;
    for (i = i; i < 23; i++)
    {
        for (j = 0; j < 40; j++)
        {
            switch (map[i][j])
            {
            case DIRT:
                al_draw_bitmap(sprites->dirt, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case BOULDER:
                al_draw_bitmap(sprites->boulder, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case WALL:
                al_draw_bitmap(sprites->brick, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case BRICK:
                al_draw_bitmap(sprites->wall, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case DIAMOND:
                if (counter % 100 <= 15)
                    al_draw_bitmap_region(sprites->diamond, TILE_SIZE * 0, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else if (counter % 100 <= 30)
                    al_draw_bitmap_region(sprites->diamond, TILE_SIZE * 1, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else if (counter % 100 <= 45)
                    al_draw_bitmap_region(sprites->diamond, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else if (counter % 100 <= 60)
                    al_draw_bitmap_region(sprites->diamond, TILE_SIZE * 3, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else if (counter % 100 <= 75)
                    al_draw_bitmap_region(sprites->diamond, TILE_SIZE * 4, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else if (counter % 100 <= 90)
                    al_draw_bitmap_region(sprites->diamond, TILE_SIZE * 5, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else
                    al_draw_bitmap_region(sprites->diamond, TILE_SIZE * 6, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case EXIT:
                if (counter % 50 <= 25)
                    al_draw_bitmap_region(sprites->exit, TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else
                    al_draw_bitmap_region(sprites->exit, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case EXPLOSION:
                if (counter % 50 <= 25)
                    al_draw_bitmap_region(sprites->explosion, 0, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                else
                    al_draw_bitmap_region(sprites->explosion, TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            default:
                break;
            }
        }
    }
}
