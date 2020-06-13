#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

namespace globals
{
    const int GAME_VIEWPORT_W = 640;
    const int GAME_VIEWPORT_H = 480;
    const int MENU_VIEWPORT_W = 640;
    const int MENU_VIEWPORT_H = 112;

    const int SCREEN_WIDTH = GAME_VIEWPORT_W;
    const int SCREEN_HEIGHT = GAME_VIEWPORT_H + MENU_VIEWPORT_H;

    const float SPRITE_SCALE = 2.0f;

    const float CAMERA_SPEED = 0.2f;
}

namespace sides
{
    enum Side
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        NONE
    };

    inline Side getOppositeSide(Side side)
    {
        //It's beautiful...
        return
            side == TOP ? BOTTOM :
            side == BOTTOM ? TOP :
            side == LEFT ? RIGHT :
            side == RIGHT ? LEFT :
            NONE;
    }
}
enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    UPLEFT,
    UPRIGHT,
    DOWNLEFT,
    DOWNRIGHT,
    NONE
};

struct Vector2
{
    int x;
    int y;

    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2 zero()
    {
        return Vector2(0, 0);
    }
};

#endif // GLOBALS_H
