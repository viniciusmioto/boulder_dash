#include "physics.h"

/* Initialize hero (Rockford) attributes */
void hero_init(HERO *hero)
{
    hero->score = 0;
    hero->sourceX = 0;
    hero->sourceY = 0;
    hero->mapX = 3;
    hero->mapY = 3;
    hero->easteregg = 0;
    hero->direction = STOPPED;
    hero->diamonds = 0;
    hero->win = false;
    hero->lose = false;
}

/* Update map by boulder and diamond physics. Verify if the hero will survive */
void update_map(HERO *hero, SAMPLES *samples, int map[MAP_H][MAP_W], int object, int counter)
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
                if (map[y][x] == object)
                {
                    /* object falling */
                    if (map[y + 1][x] == EMPTY && !(y + 1 == hero->mapY && x == hero->mapX))
                    {
                        falling_distance++;
                        /* if the boulder falls more than one tile, then Rockford will die */
                        if (hero->mapY >= y + 1 && hero->mapX == x && falling_distance >= 1 && hero->direction != DOWN)
                        {
                            game_over = true;
                            al_play_sample(samples->sound_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        map[y + 1][x] = object;
                        map[y][x] = EMPTY;
                        al_play_sample(samples->sound_boulder, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }
                    /* object rolling */
                    else if (map[y + 1][x] == BOULDER || map[y + 1][x] == DIAMOND)
                    {
                        /* object rolling left */
                        if (map[y + 1][x - 1] == EMPTY && map[y][x - 1] == EMPTY && !(y + 1 == hero->mapY && x - 1 == hero->mapX))
                        {
                            falling_distance++;
                            /* if the object rolls and falls in Rockford's head, we lose */
                            if (hero->mapY >= y + 1 && hero->mapX == x - 1 && falling_distance >= 1 && hero->direction != LEFT)
                            {
                                game_over = true;
                                al_play_sample(samples->sound_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                            map[y + 1][x - 1] = object;
                            map[y][x] = EMPTY;
                            al_play_sample(samples->sound_boulder, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        /* object rolling right */
                        else if (map[y + 1][x + 1] == EMPTY && map[y][x + 1] == EMPTY && !(y + 1 == hero->mapY && x + 1 == hero->mapX))
                        {
                            falling_distance++;
                            /* if the object rolls and falls in Rockford's head, we lose */
                            if (hero->mapY >= y + 1 && hero->mapX == x + 1 && falling_distance >= 1 && hero->direction != RIGHT)
                            {
                                game_over = true;
                                al_play_sample(samples->sound_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                            map[y + 1][x + 1] = object;
                            map[y][x] = EMPTY;
                            al_play_sample(samples->sound_boulder, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
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

/* The hero can push boulders to left or right */
void push_boulder(HERO *hero, int map[MAP_H][MAP_W], int x, int y)
{
    /* if the hero is facing a boulder in the left direction and there is a empty position, then the boulder is pushed */
    if (hero->direction == LEFT && map[y][x - 1] == EMPTY)
    {
        map[y][x - 1] = BOULDER;
        map[y][x] = EMPTY;
    }
    /* same thing, but for the right direction */
    else if (hero->direction == RIGHT && map[y][x + 1] == EMPTY)
    {
        map[y][x + 1] = BOULDER;
        map[y][x] = EMPTY;
    }
}

/* It detects collision between hero and map objects */
bool object_collision(HERO *hero, SAMPLES *samples, int map[MAP_H][MAP_W], int x, int y)
{
    switch (map[y][x])
    {
    case EMPTY:
        return false;
    case DIRT:
        /* dirty goes away */
        al_play_sample(samples->sound_dirt, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        map[y][x] = 0;
        return false;
    case WALL:
        return true;
    case BRICK:
        /* easteregg allows to move through bricks */
        if (hero->easteregg == 4)
        {
            hero->easteregg = 0;
            return false;
        }
        else
            return true;
    case DIAMOND:
        /* hero has collected a diamond */
        al_play_sample(samples->sound_diamond, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        hero->diamonds++;
        hero->score += 10;
        map[y][x] = 0;
        return false;
    case EXIT:
        /* hero has reached the exit */
        al_play_sample(samples->sound_exit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        hero->win = true;
        map[y][x] = 0;
        return false;
    default:
        return true;
    }
}

/* Movements with arrow keys and animations with sprites */
void move_hero(HERO *hero, SPRITES *sprites, SAMPLES *samples, unsigned char key[ALLEGRO_KEY_MAX], int counter, int map[MAP_H][MAP_W])
{
    /* the counter delay allows us to make the animations and movements more fluidly */
    if (counter % 6 == 0)
    {
        /* check which key is being pressed, then verify collisions */
        if (key[ALLEGRO_KEY_LEFT])
        {
            hero->direction = LEFT;
            if (!object_collision(hero, samples, map, hero->mapX - 1, hero->mapY))
                hero->mapX--;
            else
            {
                if (map[hero->mapY][hero->mapX - 1] == BOULDER)
                    push_boulder(hero, map, hero->mapX - 1, hero->mapY);
                hero->direction = STOPPED;
            }
        }
        else if (key[ALLEGRO_KEY_RIGHT])
        {
            hero->direction = RIGHT;
            if (!object_collision(hero, samples, map, hero->mapX + 1, hero->mapY))
                hero->mapX++;
            else
            {
                if (map[hero->mapY][hero->mapX + 1] == BOULDER)
                    push_boulder(hero, map, hero->mapX + 1, hero->mapY);
                hero->direction = STOPPED;
            }
        }
        else if (key[ALLEGRO_KEY_UP])
        {
            hero->direction = UP;
            if (!object_collision(hero, samples, map, hero->mapX, hero->mapY - 1))
                hero->mapY--;
            else
                hero->direction = STOPPED;
        }
        else if (key[ALLEGRO_KEY_DOWN])
        {
            hero->direction = DOWN;
            if (!object_collision(hero, samples, map, hero->mapX, hero->mapY + 1))
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

        /* update map after hero movement */
        update_map(hero, samples, map, DIAMOND, counter);
        update_map(hero, samples, map, BOULDER, counter);
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

    /* check if the hero has collected enough diamonds */
    if (hero->diamonds >= 12)
        /* open the door */
        map[MAP_H - 6][MAP_W - 2] = 6;
}

/* Check for a sequence of characters to activate a easter egg */
void verify_easter_egg(HERO *hero, unsigned char key[ALLEGRO_KEY_MAX])
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
        /* easteregg changes the hero colour */
        al_draw_tinted_bitmap_region(sprites->hero, al_map_rgb(255, 0, 255), hero->sourceX, hero->sourceY * al_get_bitmap_height(sprites->hero) / 5, HERO_W, HERO_H, hero->mapX * HERO_W, hero->mapY * HERO_H, 0);
    else
        al_draw_bitmap_region(sprites->hero, hero->sourceX, hero->sourceY * al_get_bitmap_height(sprites->hero) / 5, HERO_W, HERO_H, hero->mapX * HERO_W, hero->mapY * HERO_H, 0);
}