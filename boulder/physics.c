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
    hero->x = 32;
    hero->y = 32;
    hero->lives = 3;
    hero->sourceX = 0;
    hero->sourceY = 0;
    hero->active = false;
    hero->direction = STOPPED;
}

/* Movements with arrow keys and animations with sprites */
void move_hero(HERO *hero, SPRITES *sprites, unsigned char key[ALLEGRO_KEY_MAX], int counter)
{
    /* activate means that the hero must be drawn again at the display */
    hero->active = true;

    /* the counter delay allows us to make the animations and movements more fluidly */
    if (counter % 5 == 0)
    {
        /* check which key is being pressed */
        if (key[ALLEGRO_KEY_LEFT])
        {
            hero->x -= HERO_SPEED;
            hero->direction = LEFT;
        }
        else if (key[ALLEGRO_KEY_RIGHT])
        {
            hero->x += HERO_SPEED;
            hero->direction = RIGHT;
        }
        else if (key[ALLEGRO_KEY_UP])
        {
            hero->y -= HERO_SPEED;
            hero->direction = UP;
        }
        else if (key[ALLEGRO_KEY_DOWN])
        {
            hero->y += HERO_SPEED;
            hero->direction = DOWN;
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

    /* check if the hero is out of the screen by our bounds */
    if (hero->x < 0)
        hero->x = 0;
    if (hero->y < 0)
        hero->y = 0;
    if (hero->x > HERO_MAX_X)
        hero->x = HERO_MAX_X;
    if (hero->y > HERO_MAX_Y)
        hero->y = HERO_MAX_Y;

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
        al_draw_tinted_bitmap_region(sprites->hero, al_map_rgb(255,0,255),hero->sourceX, hero->sourceY * al_get_bitmap_height(sprites->hero) / 5, HERO_W, HERO_H, hero->x, hero->y, 0);
    else
        al_draw_bitmap_region(sprites->hero, hero->sourceX, hero->sourceY * al_get_bitmap_height(sprites->hero) / 5, HERO_W, HERO_H, hero->x, hero->y, 0);

}

// we are not using these functions yet
// int between(int min, int max)
// {
//     return min + (rand() % (max - min));
// }

// float between_f(float min, float max)
// {
//     return min + ((float)rand() / (float)RAND_MAX) * (max - min);
// }