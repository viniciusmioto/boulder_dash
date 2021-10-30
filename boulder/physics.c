#include "physics.h"

/* It returns whether two squares will colide */
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if (ax1 > bx2)
        return false;
    if (ax2 < bx1)
        return false;
    if (ay1 > by2)
        return false;
    if (ay2 < by1)
        return false;

    return true;
}

/* Initialize hero (Rockford) attributes */
void hero_init(HERO *hero)
{
    hero->lives = 3;
    hero->sourceX = 0;
    hero->sourceY = 0;
    hero->mapX = 3;
    hero->mapY = 3;
    hero->easteregg = 0;
    hero->active = false;
    hero->direction = STOPPED;
    hero->diamonds = 0;
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
    default:
        return true;
    }
}

/* Movements with arrow keys and animations with sprites */
void move_hero(HERO *hero, SPRITES *sprites, unsigned char key[ALLEGRO_KEY_MAX], int counter, int map[MAP_H][MAP_W])
{
    /* activate means that the hero must be drawn again at the display */
    hero->active = true;

    /* the counter delay allows us to make the animations and movements more fluidly */
    if (counter % 6 == 0)
    {
        /* check which key is being pressed */
        if (key[ALLEGRO_KEY_LEFT])
        {
            hero->direction = LEFT;
            if (!object_collision(hero, map, hero->mapX - 1, hero->mapY))
                hero->mapX--;
            
        }
        else if (key[ALLEGRO_KEY_RIGHT])
        {

            hero->direction = RIGHT;
            if (!object_collision(hero, map, hero->mapX + 1, hero->mapY))
                hero->mapX++;
        }
        else if (key[ALLEGRO_KEY_UP])
        {
            hero->direction = UP;
            if (!object_collision(hero, map, hero->mapX, hero->mapY - 1))
                hero->mapY--;
            
        }
        else if (key[ALLEGRO_KEY_DOWN])
        {
            hero->direction = DOWN;
            if (!object_collision(hero, map, hero->mapX, hero->mapY + 1))
                hero->mapY++;
        }
        /* no key is pressed */
        else
            hero->direction = STOPPED;

        /* set sourceX and sourceY for the animations */
        if (hero->active)
            hero->sourceX += al_get_bitmap_width(sprites->hero) / 8;
        else
            hero->sourceX = 32;

        if (hero->sourceX >= al_get_bitmap_width(sprites->hero))
            hero->sourceX = 0;

        hero->sourceY = hero->direction;
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

    object_collision(hero, map, hero->mapX, hero->mapY);

    hero->active = false;
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