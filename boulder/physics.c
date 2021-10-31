#include "physics.h"

/* Initialize hero (Rockford) attributes */
void hero_init(HERO *hero)
{
    hero->sourceX = 0;
    hero->sourceY = 0;
    hero->mapX = 3;
    hero->mapY = 3;
    hero->easteregg = 0;
    hero->direction = STOPPED;
    hero->diamonds = 0;
    hero->won = false;
    hero->lose = false;
}

/* Boulder and diamond physics */
void update_map(HERO *hero, int map[MAP_H][MAP_W], int element, int counter)
{
    int y, x, falling_distance;
    bool game_over = false;

    falling_distance = 0;
    if (counter % 5 == 1)
    {
        for (y = 0; y < MAP_H; y++)
        {
            for (x = 0; x < MAP_W; x++)
            {
                if (map[y][x] == element)
                {
                    /* object falling */
                    if (map[y + 1][x] == EMPTY && !(y + 1 == hero->mapY && x == hero->mapX))
                    {
                        falling_distance++;
                        if (map[y][x] == BOULDER && (hero->mapY >= y + 1 && hero->mapX == x) && falling_distance >= 1 && hero->direction != DOWN)
                        {
                            game_over = true;
                        }
                        map[y + 1][x] = element;
                        map[y][x] = EMPTY;
                    }
                    /* object rolling left */
                    else if (map[y + 1][x] == element && map[y + 1][x - 1] == EMPTY && !(y + 1 == hero->mapY && x - 1 == hero->mapX))
                    {
                        falling_distance++;
                        if (map[y][x] == BOULDER && (hero->mapY >= y + 1 && hero->mapX == x - 1) && falling_distance >= 1 && hero->direction != LEFT)
                        {
                            game_over = true;
                        }
                        map[y + 1][x - 1] = element;
                        map[y][x] = EMPTY;
                    }
                    /* object rolling right */
                    else if (map[y + 1][x] == element && map[y + 1][x + 1] == EMPTY && !(y + 1 == hero->mapY && x + 1 == hero->mapX))
                    {
                        falling_distance++;
                        if (map[y][x] == BOULDER && (hero->mapY >= y + 1 && hero->mapX == x + 1) && falling_distance >= 1 && hero->direction != RIGHT)

                        {
                            game_over = true;
                        }
                        map[y + 1][x + 1] = element;
                        map[y][x] = EMPTY;
                    }
                }
            }
        }
    }

    /* if the hero loses, then a explosion is created */
    if (game_over)
    {
        /* here come the explosions */
        for (y = hero->mapY - 1; y <= hero->mapY + 1; y++)
        {
            for (x = hero->mapX - 1; x <= hero->mapX + 1; x++)
            {
                map[y][x] = EXPLOSION;
            }
        }
        hero->lose = true;
    }
}

void push_boulder(HERO *hero, int map[MAP_H][MAP_W], int x, int y)
{
    if (hero->direction == LEFT && map[y][x - 1] == EMPTY)
    {
        map[y][x - 1] = BOULDER;
        map[y][x] = EMPTY;
    }
    else if (hero->direction == RIGHT && map[y][x + 1] == EMPTY)
    {
        map[y][x + 1] = BOULDER;
        map[y][x] = EMPTY;
    }
}

bool object_collision(HERO *hero, int map[MAP_H][MAP_W], int x, int y)
{
    switch (map[y][x])
    {
    case EMPTY:
        return false;
    case DIRT:
        map[y][x] = 0;
        return false;
    case WALL:
        return true;
    case BRICK:
        return true;
    case DIAMOND:
        hero->diamonds++;
        map[y][x] = 0;
        return false;
    case EXIT:
        hero->won = true;
        map[y][x] = 0;
        return false;
    default:
        return true;
    }
}

/* Movements with arrow keys and animations with sprites */
void move_hero(HERO *hero, SPRITES *sprites, unsigned char key[ALLEGRO_KEY_MAX], int counter, int map[MAP_H][MAP_W])
{
    /* the counter delay allows us to make the animations and movements more fluidly */
    if (counter % 6 == 0)
    {
        /* check which key is being pressed */
        if (key[ALLEGRO_KEY_LEFT])
        {
            hero->direction = LEFT;
            if (!object_collision(hero, map, hero->mapX - 1, hero->mapY))
                hero->mapX--;
            else
            {
                push_boulder(hero, map, hero->mapX - 1, hero->mapY);
                hero->direction = STOPPED;
            }
        }
        else if (key[ALLEGRO_KEY_RIGHT])
        {
            hero->direction = RIGHT;
            if (!object_collision(hero, map, hero->mapX + 1, hero->mapY))
                hero->mapX++;
            else
            {
                push_boulder(hero, map, hero->mapX + 1, hero->mapY);
                hero->direction = STOPPED;
            }
        }
        else if (key[ALLEGRO_KEY_UP])
        {
            hero->direction = UP;
            if (!object_collision(hero, map, hero->mapX, hero->mapY - 1))
                hero->mapY--;
            else
                hero->direction = STOPPED;
        }
        else if (key[ALLEGRO_KEY_DOWN])
        {
            hero->direction = DOWN;
            if (!object_collision(hero, map, hero->mapX, hero->mapY + 1))
                hero->mapY++;
            else
                hero->direction = STOPPED;
        }
        /* no key is pressed */
        else
            hero->direction = STOPPED;

        /* set sourceX and sourceY for the animations */

        hero->sourceX += al_get_bitmap_width(sprites->hero) / 8;

        if (hero->sourceX >= al_get_bitmap_width(sprites->hero))
            hero->sourceX = 0;

        hero->sourceY = hero->direction;
        update_map(hero, map, BOULDER, counter);
        update_map(hero, map, DIAMOND, counter);
    }

    /* check if the hero is inside the map */
    if (hero->mapX < 0)
        hero->mapX = 0;
    if (hero->mapX > MAP_W - 1)
        hero->mapX = MAP_W - 1;
    if (hero->mapY < 0)
        hero->mapY = 0;
    if (hero->mapY > MAP_H - 1)
        hero->mapY = MAP_H - 1;

    /* check if the hero has collected all the diamonds */
    if (hero->diamonds >= 12)
        /* open the door */
        map[MAP_H - 6][MAP_W - 2] = 6;
}

int verify_easter_egg(HERO *hero, unsigned char key[ALLEGRO_KEY_MAX])
{
    if (key[ALLEGRO_KEY_R])
        hero->easteregg = 1;
    if (hero->easteregg == 1 && key[ALLEGRO_KEY_O])
        hero->easteregg = 2;
    if (hero->easteregg == 2 && key[ALLEGRO_KEY_C])
        hero->easteregg = 3;
    if (hero->easteregg == 3 && key[ALLEGRO_KEY_K])
        hero->easteregg = 4;
}

/* Draw and animate the hero on display */
void hero_draw(HERO *hero, SPRITES *sprites)
{
    /* it gets the a specific region of the spritesheet by using sourceX and sourceY */

    if (hero->easteregg == 4)
        al_draw_tinted_bitmap_region(sprites->hero, al_map_rgb(255, 0, 255), hero->sourceX, hero->sourceY * al_get_bitmap_height(sprites->hero) / 5, HERO_W, HERO_H, hero->mapX * HERO_W, hero->mapY * HERO_H, 0);
    else
        al_draw_bitmap_region(sprites->hero, hero->sourceX, hero->sourceY * al_get_bitmap_height(sprites->hero) / 5, HERO_W, HERO_H, hero->mapX * HERO_W, hero->mapY * HERO_H, 0);
}