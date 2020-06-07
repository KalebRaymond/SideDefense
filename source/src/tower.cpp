#include "tower.h"
#include "projectile.h"
#include "graphics.h"

Tower::Tower()
{

}

Tower::Tower(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate, int maxHealth)
    :   AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
        _maxHealth(maxHealth),
        _currentHealth(maxHealth),
        _fireCoolDown(0),
        _lastFireTime(0)
{

}

void Tower::update(int elapsedTime, Graphics &graphics)
{
    AnimatedSprite::update(elapsedTime);
}

void Tower::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

Projectile* Tower::fireProjectile(Graphics &graphics, int elapsedTime)
{
    return nullptr;
}

bool Tower::getDragged()
{
    return this->_dragged;
}

void Tower::setDragged(bool dragged)
{
    this->_dragged = dragged;
}

bool Tower::pointCollides(int x, int y)
{
    Rectangle rect = this->getBoundingBox();

    return  x >= rect.getLeft() && x <= rect.getRight()
            && y >= rect.getTop() && y <= rect.getBottom();
}

void Tower::handleMouseEvent(int mouseX, int mouseY)
{
    //This function can use a lot of work, but for now it works good enough for proof of concept purposes.

    //The tower sprite snaps so that the mouse position becomes the center of the sprite. Ideally the relative
    //mouse position should stay the same. Calculate an offset based on bounding box left and top?

    Sprite::update();
}

//BulletTower class

BulletTower::BulletTower()
{

}

BulletTower::BulletTower(Graphics &graphics, Vector2 spawnPoint)
    :   Tower(graphics, "content/sprites/bulletTower.png", 0, 0, 16, 16, spawnPoint, 100, 5),
        _startingX(spawnPoint.x),
        _startingY(spawnPoint.y)
{
    this->_fireCoolDown = 1000;
    this->_lastFireTime = -1;
    this->setupAnimation();
    this->playAnimation("FaceLeft");
}

void BulletTower::update(int elapsedTime, Graphics &graphics)
{
    //Destroy if health is 0
    if(this->_dragged)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        this->_x = mouseX - graphics.getCameraOffsetX() - 16;
        this->_y = mouseY - graphics.getCameraOffsetY() - 16;
    }
    Sprite::update();
}

void BulletTower::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void BulletTower::animationDone(std::string currentAnimation)
{

}

void BulletTower::setupAnimation()
{
    this->addAnimation(1, 0, 0, "FaceLeft", 16, 16, Vector2(0, 0));
}

Projectile* BulletTower::fireProjectile(Graphics &graphics, int elapsedTime)
{
    this->_lastFireTime += elapsedTime;
    if(( _lastFireTime > _fireCoolDown || _lastFireTime < 0 ) && !this->_dragged)
    {
        _lastFireTime = 0;
        return new Bullet(graphics, Vector2( this->_x, this->_y + 11 ));
    }
    else
    {
        return nullptr;
    }
}

//Can probably delete this
void BulletTower::handleMouseEvent(int mouseX, int mouseY)
{
    this->setDragged(true);
}

//RocketTower class

RocketTower::RocketTower()
{

}

RocketTower::RocketTower(Graphics &graphics, Vector2 spawnPoint)
    :   Tower(graphics, "content/sprites/rocketTower.png", 0, 0, 16, 16, spawnPoint, 100, 5),
        _startingX(spawnPoint.x),
        _startingY(spawnPoint.y)
{
    this->_fireCoolDown = 500;
    this->_lastFireTime = -1;
    this->setupAnimation();
    this->playAnimation("FaceLeft");
}

void RocketTower::update(int elapsedTime, Graphics &graphics)
{

}

void RocketTower::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void RocketTower::animationDone(std::string currentAnimation)
{

}

void RocketTower::setupAnimation()
{
    this->addAnimation(1, 0, 0, "FaceLeft", 16, 16, Vector2(0, 0));
}

Projectile* RocketTower::fireProjectile(Graphics &graphics, int elapsedTime)
{
    this->_lastFireTime += elapsedTime;
    if(( _lastFireTime > _fireCoolDown || _lastFireTime < 0 ) && !this->_dragged)
    {
        _lastFireTime = 0;
        return new Bullet(graphics, Vector2( this->_x, this->_y + 11 ));
    }
    else
    {
        return nullptr;
    }
}

void RocketTower::handleMouseEvent(int mouseX, int mouseY)
{

}
