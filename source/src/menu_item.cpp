#include "menu_item.h"
#include "rectangle.h"
#include "graphics.h"
#include "input.h"
#include "tower.h"

MenuItem::MenuItem()
{

}

MenuItem::MenuItem(std::string name, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint)
    :   Sprite(graphics, "content/sprites/menu.png", sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y),
        _name(name),
        _clicked(false)
{

}

void MenuItem::update(int elapsedTime, Input &input)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    Rectangle mousePosition = {mouseX, mouseY, 0, 0};

    //The music and sfx icons should toggle on or off, but the save icon should
    //only have the pressed-down sprite while the left mouse button is held down
    if( mousePosition.collidesWith(this->getBoundingBox()) )
    {
        if(!this->_clicked)
        {
             this->onHover();
        }

        if(this->_name == "save" && input.getLeftClick())
        {
            this->onClick();
        }

        if( (this->_name == "music" || this->_name == "sfx") && input.getLeftReleased() )
        {
            this->_clicked = !this->_clicked;
            this->onClick();
        }
    }
    else
    {
        if(this->_name == "save")
        {
            setSprite(this->_name, 3, this->_sourceRect.y, 16, 16);
        }

        if(( this->_name == "music" || this->_name == "sfx") && !this->_clicked)
        {
            setSprite(this->_name, 3, this->_sourceRect.y, 16, 16);
        }
    }

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

void MenuItem::setSprite(std::string name, int sourceX, int sourceY, int width, int height)
{
    this->_name = name;
    this->_sourceRect.x = sourceX;
    this->_sourceRect.y = sourceY;
    this->_sourceRect.w = width;
    this->_sourceRect.h = height;
}

void MenuItem::onHover()
{
    if(this->_name != "hp" && this->_name != "cash")
    {
        setSprite(this->_name, 21, this->_sourceRect.y, 16, 16);
    }
}

void MenuItem::onClick()
{
    //The music and sfx icons should toggle on and off, while the save icon should only
    //stay on the the third sprite as long as the left mouse button is held down on it.
    if( (this->_name == "music" || this->_name == "sfx") )
    {
        if(this->_clicked)
        {
            setSprite(this->_name, 39, this->_sourceRect.y, 16, 16);
        }
        else
        {
            setSprite(this->_name, 21, this->_sourceRect.y, 16, 16);
        }
    }
    else if(this->_name == "save")
    {
        setSprite(this->_name, 39, this->_sourceRect.y, 16, 16);
    }
}

//TowerMenuItem class

TowerMenuItem::TowerMenuItem()
{

}

TowerMenuItem::TowerMenuItem(std::string name, int price, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint)
    :   MenuItem(name, graphics, sourceX, sourceY, width, height, spawnPoint),
        _price(price)
{

}

void TowerMenuItem::update(int elapsedTime, Input &input)
{
    this->_clicked = false;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    Rectangle mousePosition = {mouseX, mouseY, 0, 0};

    if( mousePosition.collidesWith(this->getBoundingBox()) )
    {
        this->onHover();

        if(input.getLeftReleased())
        {
            this->_clicked = true;
        }
    }
    else
    {
        setSprite(this->_name, 58, this->_sourceRect.y, 40, 40);
    }

    Sprite::update();
}

void TowerMenuItem::onHover()
{
    setSprite(this->_name, 100, this->_sourceRect.y, 40, 40);
}

Tower* TowerMenuItem::createTower(Graphics &graphics, int mouseX, int mouseY, int money)
{
    Tower* tower = nullptr;

    if(this->_name == "bulletTower" && money >= 100)
    {
       tower = new BulletTower(graphics, Vector2(mouseX, mouseY));
    }
    else if(this->_name == "rocketTower" && money >= 250)
    {
       tower = new RocketTower(graphics, Vector2(mouseX, mouseY));
    }
    else if(this->_name == "sniperTower" && money >= 75)
    {
       tower = new SniperTower(graphics, Vector2(mouseX, mouseY));
    }

    if(tower != nullptr)
    {
        tower->setDragged(true);
    }

    return tower;
}
