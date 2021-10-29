#include "controls.h"
/* ** All the following functions are designed and recommended by the Allegro tutorial ** */

/* Verify if an specific initialization went right */
void must_init(bool test, const char *description)
{
    if (test)
        return;

    fprintf(stderr, "couldn't initialize %s\n", description);
    exit(1);
}

ALLEGRO_BITMAP *sprite_grab(SPRITES *sprites, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sprites->_spritesheet, x, y, w, h);
    return sprite;
}

/* Initialize our sprites */
void sprites_init(SPRITES *sprites)
{
    sprites->_spritesheet = al_load_bitmap("spritesheet.png");
    must_init(sprites->_spritesheet, "map sprites");

    sprites->hero = sprite_grab(sprites, 0, TILE_SIZE, TILE_SIZE * 8, TILE_SIZE * 5);
    must_init(sprites->hero, "map sprites - Rockford");

    sprites->diamond = sprite_grab(sprites, 0, TILE_SIZE * 10, TILE_SIZE * 8, TILE_SIZE);
    must_init(sprites->diamond, "map sprites - diamonds");

    sprites->boulder = sprite_grab(sprites, 0, TILE_SIZE * 7, TILE_SIZE, TILE_SIZE);
    must_init(sprites->boulder, "map sprites - boulder");

    sprites->dirt = sprite_grab(sprites, TILE_SIZE, TILE_SIZE * 7, TILE_SIZE, TILE_SIZE);
    must_init(sprites->dirt, "sprites - dirt");

    sprites->brick = sprite_grab(sprites, TILE_SIZE, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
    must_init(sprites->brick, "sprites - brick");

    sprites->wall = sprite_grab(sprites, TILE_SIZE * 3, TILE_SIZE * 6, TILE_SIZE, TILE_SIZE);
    must_init(sprites->wall, "sprites - wall");

}

/* Free memory of sprites */
void sprites_deinit(SPRITES *sprites)
{
    al_destroy_bitmap(sprites->hero);
    // al_destroy_bitmap(sprites->map);
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

void loadMap(const char *fileName, int map[23][40])
{
    int i = 0, j = 0;
    char c;

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    i = 0, j = 0;
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

void draw_map(int map[23][40], SPRITES *sprites)
{
    int i, j;
    
    for (i = 1; i < 23; i++)
    {
        for (j = 0; j < 40; j++)
        {
            switch (map[i-1][j])
            {
            case 1:
                al_draw_bitmap(sprites->dirt, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case 2:
                al_draw_bitmap(sprites->boulder, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case 3:
                al_draw_bitmap(sprites->brick, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case 4:
                al_draw_bitmap(sprites->wall, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            case 5:
                al_draw_bitmap_region(sprites->diamond, 0, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE * j, TILE_SIZE * i, 0);
                break;
            default:
                break;
            }
        }
    }
}
