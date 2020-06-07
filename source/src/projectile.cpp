#include "projectile.h"
#include "iostream"

Projectile::Projectile()
{

}

Projectile::Projectile(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate)
    :   AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
        _damage(0),
        _dx(0),
        _dy(0),
        _direction(NONE)
{

}

Projectile::~Projectile()
{

}

void Projectile::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Projectile::update(float elapsedTime)
{
    AnimatedSprite::update(elapsedTime);
}

void Projectile::moveDirection(float elapsedTime)
{

}

void Projectile::setDirection(Direction direction)
{
    this->_direction = direction;
}

int Projectile::getDamage()
{
    return this->_damage;
}

//Bullet subclass

Bullet::Bullet()
{

}

Bullet::Bullet(Graphics &graphics, Vector2 spawnPoint)
    :   Projectile(graphics, "content/sprites/bulletTower.png", 2, 18, 4, 3, spawnPoint, 100)
{
    this->_damage = 1;
    this->_dx = -0.1;
    this->_dy = 0;
    this->_direction = LEFT;

    this->setupAnimation();
    this->playAnimation("MoveLeft");
}

Bullet::~Bullet()
{

}

void Bullet::animationDone(std::string currentAnimation)
{

}

void Bullet::setupAnimation()
{
    this->addAnimation(1, 2, 18, "MoveLeft", 4, 3, Vector2(0, 0));
}

void Bullet::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Bullet::update(float elapsedTime)
{
    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void Bullet::moveDirection(float elapsedTime)
{
    //Move by dx
    this->_x += this->_dx * elapsedTime;
    //Move by dy
    this->_y += this->_dy * elapsedTime;
}

void Bullet::setDirection(Direction direction)
{
    this->_direction = direction;
    if(direction == LEFT)
    {
        this->_dx = -0.02;
        this->_dy = 0;
    }
    else if(direction == RIGHT)
    {
        this->_dx = 0.02;
        this->_dy = 0;
    }
    else
    {
        this->_dx = 0;
        this->_dy = 0;
    }
}
