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

void hero_init(HERO *hero)
{
    hero->x = (BUFFER_W / 2) - (HERO_W / 2);
    hero->y = (BUFFER_H / 2) - (HERO_H / 2);
    hero->direction = STOPPED;
    hero->lives = 3;
    hero->sourceX = 0;
    hero->sourceY = 0;
    hero->active = false;
}

void hero_draw(HERO *hero, SPRITES *sprites)
{

    al_draw_bitmap_region(sprites->hero, hero->sourceX, hero->sourceY * al_get_bitmap_height(sprites->hero) / 5, 32, 32, hero->x, hero->y, 0);
}

void move_hero(HERO *hero, SPRITES *sprites, unsigned char key[ALLEGRO_KEY_MAX])
{
    hero->active = true;
    /* movement */
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
    else
    {
        hero->direction = STOPPED;
        // *active = false;
    }

    if (hero->active)
    {
        hero->sourceX += al_get_bitmap_width(sprites->hero) / 8;
    }
    else
    {
        hero->sourceX = 32;
    }

    if (hero->sourceX >= al_get_bitmap_width(sprites->hero))
    {
        hero->sourceX = 0;
    }

    hero->sourceY = hero->direction;

    /* bounds */
    if (hero->x < 0)
        hero->x = 0;
    if (hero->y < 0)
        hero->y = 0;
    if (hero->x > HERO_MAX_X)
        hero->x = HERO_MAX_X;
    if (hero->y > HERO_MAX_Y)
        hero->y = HERO_MAX_Y;
}

void keyboard_init(unsigned char *key)
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT *event, unsigned char *key)
{
    if (event->type == ALLEGRO_EVENT_KEY_DOWN)
        key[event->keyboard.keycode] = 1;
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

// we are not using these functions yet
// int between(int min, int max)
// {
//     return min + (rand() % (max - min));
// }

// float between_f(float min, float max)
// {
//     return min + ((float)rand() / (float)RAND_MAX) * (max - min);
// }