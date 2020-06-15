#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <math.h>

#define PI 3.14159265

namespace globals
{
    const int GAME_VIEWPORT_W = 640;
    const int GAME_VIEWPORT_H = 480;
    const int MENU_VIEWPORT_W = GAME_VIEWPORT_W;
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

//Saving this in case I need it later. If used, Direction has to be put in a namespace
/*inline Direction clockwiseTurn(Direction curDirection)
{
    if(curDirection == NONE)
    {
        return NONE;
    }

    Direction newDirection = (Direction)((curDirection + 1) % 9);
    if(newDirection == NONE)
    {
        newDirection = LEFT;
    }

    return newDirection;
}*/

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


namespace functions
{
    /*  Return angle to (targetX, targetY) from (originX, originY) in degrees
    */
    inline float getAngle(int originX, int originY, int targetX, int targetY)
    {
        //atan2 returns the angle in radians of the line from (0, 0) to (x, y) from the x-axis.
        //The return value is in the interval [-pi, pi], which means if the target is below the
        //origin, atan2 will return a negative angle.
        //To get the angle relative to the target, the target coordinates must be translated by
        //(-originX, -originY).
        //Also, the arguments for atan2 are atan2(y, x)

        float y = targetY - originY;
        float x = targetX - originX;

        //Because the SDL coordinate system is upside down (higher y value -> lower on the screen),
        //the angle will have the wrong sign.
        return -1 * atan2(y, x) * (180 / PI);
    }
}

#endif // GLOBALS_H
