#include "cursor.h"
#include "graphics.h"

Cursor::Cursor()
{

}

Cursor::Cursor(Graphics& graphics)
    :   AnimatedSprite(graphics, "content/sprites/misc.png", 0, 0, 4, 5, 215, START, 100)
{
    this->addAnimation(1, 0, 0, "cursorBounce", 4, 5, Vector2(0, 0));
    this->playAnimation("cursorBounce");
}

void Cursor::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Cursor::setPosition(Position p)
{
    this->_y = p;
}

Position Cursor::getPosition()
{
    return (Position)(this->_y);
}
