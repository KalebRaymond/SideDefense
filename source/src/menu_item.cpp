#include "menu_item.h"
#include "graphics.h"

MenuItem::MenuItem()
{

}

MenuItem::MenuItem(std::string name, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint)
    :   Sprite(graphics, "content/sprites/menu.png", sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y),
        _name(name)
{

}

void MenuItem::update(int elapsedTime)
{
    Sprite::update();
}

void MenuItem::draw(Graphics &graphics, int x, int y)
{
    SDL_Rect destRectangle = {  x,
                                y,
                                this->_sourceRect.w * globals::SPRITE_SCALE,
                                this->_sourceRect.h * globals::SPRITE_SCALE};

    graphics.blitMenu(this->_spriteSheet, &this->_sourceRect, &destRectangle);
}

void MenuItem::setSprite(int sourceX, int sourceY, int width, int height)
{

}
