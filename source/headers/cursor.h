#ifndef CURSOR_H
#define CURSOR_H

#include "globals.h"
#include "animated_sprite.h"
#include <string>

class Graphics;

enum Position
{
    START = 300,
    SETTINGS = 332
};

class Cursor : public AnimatedSprite
{
    public:
        Cursor();
        Cursor(Graphics &graphics);

        void draw(Graphics &graphics);

        void setPosition(Position p);
        Position getPosition();
};

#endif // CURSOR_H
