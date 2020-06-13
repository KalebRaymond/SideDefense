#include "projectile.h"
#include "enemy.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

Projectile::Projectile()
{

}

Projectile::Projectile(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate)
    :   AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
        _damage(0),
        _dx(0),
        _dy(0),
        _direction(NONE),
        _destroy(false)
{

}

Projectile::~Projectile()
{

}

void Projectile::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Projectile::update(float elapsedTime, std::vector<Enemy*> *enemies)
{
    AnimatedSprite::update(elapsedTime);
}

void Projectile::moveDirection(float elapsedTime)
{
    //Move by dx
    this->_x += this->_dx * elapsedTime;
    //Move by dy
    this->_y += this->_dy * elapsedTime;
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

void Bullet::update(float elapsedTime, std::vector<Enemy*> *enemies)
{
    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void Bullet::handleEnemyCollision()
{
    this->_destroy = true;
}

void Bullet::setDirection(Direction direction)
{
    this->_direction = direction;

    switch(direction)
    {
        case LEFT:
            this->_dx = -0.1;
            break;
        case RIGHT:
            this->_dx = 0.1;
            break;
        default:
            this->_dx = 0;
    }
}

//Rocket class

Rocket::Rocket()
{

}

Rocket::Rocket(Graphics &graphics, Vector2 spawnPoint)
    :   Projectile(graphics, "content/sprites/rocketTower.png", 9, 19, 4, 6, spawnPoint, 75),
    _targetX(-1),
    _targetY(-1),
    _dealDamage(true),
    _lastDirectionChangeTime(0),
    _directionChangeCooldown(150)
{
    this->_damage = 0;
    this->_dx = 0;
    this->_dy = -0.1;
    this->_direction = UP;

    this->setupAnimation();
    this->playAnimation("MoveUp");
}

float Rocket::getAngleToTarget()
{
    //atan2 returns the angle in radians of the line from (0, 0) to (x, y) from the x-axis.
    //The return value is in the interval [-pi, pi], which means if the target is below the
    //rocket, atan2 will return a negative angle.
    //To get the angle relative to the projectile, the projectile must act as the origin from
    //which the arctan is measured. This means the target coordinates must be translated by
    //(-this->_x, -this->_y).
    //Also, the arguments for atan2 are atan2(y, x)

    float y = this->_targetY - this->_y;
    float x = this->_targetX - this->_x;

    //Because the SDL coordinate system is upside down (higher y value -> lower on the screen),
    //the angle will have the wrong sign.
    return -1 * atan2(y, x);
}

void Rocket::update(float elapsedTime, std::vector<Enemy*> *enemies)
{
    //Rocket should only do damage on sprite 4 of "Explode" (which has the circle with the biggest radius)
    //and only for 1 frame.
    if(this->_currentAnimation == "Explode" && this->getFrameIndex() == 4 && this->_dealDamage)
    {
        this->_damage = 5;
        this->_dealDamage = false;
    }
    else
    {
        this->_damage = 0;
    }

    if(enemies != nullptr && enemies->size() != 0)
    {
        this->_targetX = enemies->at(0)->getBoundingBox().getCenterX();
        this->_targetY = enemies->at(0)->getBoundingBox().getCenterY();
    }
    else
    {
        this->setDirection(NONE);
    }

    //Increase time since last direction change
    _lastDirectionChangeTime += elapsedTime;

    //Only change direction every 100ms
    if(_lastDirectionChangeTime >= _directionChangeCooldown && this->_currentAnimation != "Explode")
    {
        _lastDirectionChangeTime = 0;

        //If rocket is within 3 pixels of target, explode. Eventually replace with timer? Explode after a certain time?
        if((pow(this->_targetX - this->_x, 2) + pow(this->_targetY - this->_y, 2)) <= 9 && false)
        {
            this->setDirection(NONE);
        }
        //Otherwise, get angle to target in degrees
        else
        {
            float angleToTarget = this->getAngleToTarget() * (180 / PI);

            if(angleToTarget >= -30 && angleToTarget < 30)
            {
                this->setDirection(RIGHT);
            }
            else if(angleToTarget >= 30 && angleToTarget < 60)
            {
                this->setDirection(UPRIGHT);
            }
            else if(angleToTarget >= 60 && angleToTarget < 120)
            {
                this->setDirection(UP);
            }
            else if(angleToTarget >= 120 && angleToTarget < 150)
            {
                this->setDirection(UPLEFT);
            }
            else if(angleToTarget >= 150 || angleToTarget < -150)
            {
                this->setDirection(LEFT);
            }
            else if(angleToTarget >= -150 && angleToTarget < -120)
            {
                this->setDirection(DOWNLEFT);
            }
            else if(angleToTarget >= -120 && angleToTarget < -60)
            {
                this->setDirection(DOWN);
            }
            else if(angleToTarget >= -60 && angleToTarget < -30)
            {
                this->setDirection(DOWNRIGHT);
            }
        }
    }

    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void Rocket::handleEnemyCollision()
{
    this->setDirection(NONE);
}

void Rocket::animationDone(std::string currentAnimation)
{
    if(currentAnimation == "Explode")
    {
        this->_destroy = true;
    }
}

void Rocket::setupAnimation()
{
    this->addAnimation(1, 2, 20, "MoveUpLeft", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 9, 19, "MoveUp", 3, 6, Vector2(0, 0));
    this->addAnimation(1, 14, 20, "MoveUpRight", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 14, 26, "MoveRight", 6, 3, Vector2(0, 0));
    this->addAnimation(1, 14, 30, "MoveDownRight", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 9, 30, "MoveDown", 3, 6, Vector2(0, 0));
    this->addAnimation(1, 2, 30, "MoveDownLeft", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 1, 26, "MoveLeft", 6, 3, Vector2(0, 0));
    this->addAnimation(5, 17, 1, "Explode", 17, 17, Vector2(-9, -9));
}

void Rocket::setDirection(Direction direction)
{
    this->_direction = direction;

    switch(direction)
    {
        case UPLEFT:
            this->_dx = -0.09;
            this->_dy = -0.09;
            this->_sourceRect.w = 5;
            this->_sourceRect.h = 5;
            this->playAnimation("MoveUpLeft");
            break;
        case UP:
            this->_dx = 0.0;
            this->_dy = -0.12;
            this->_sourceRect.w = 3;
            this->_sourceRect.h = 6;
            this->playAnimation("MoveUp");
            break;
        case UPRIGHT:
            this->_dx = 0.09;
            this->_dy = -0.09;
            this->_sourceRect.w = 5;
            this->_sourceRect.h = 5;
            this->playAnimation("MoveUpRight");
            break;
        case RIGHT:
            this->_dx = 0.12;
            this->_dy = 0.0;
            this->_sourceRect.w = 6;
            this->_sourceRect.h = 3;
            this->playAnimation("MoveRight");
            break;
        case DOWNRIGHT:
            this->_dx = 0.09;
            this->_dy = 0.09;
            this->_sourceRect.w = 5;
            this->_sourceRect.h = 5;
            this->playAnimation("MoveDownRight");
            break;
        case DOWN:
            this->_dx = 0.0;
            this->_dy = 0.12;
            this->_sourceRect.w = 3;
            this->_sourceRect.h = 6;
            this->playAnimation("MoveDown");
            break;
        case DOWNLEFT:
            this->_dx = -0.09;
            this->_dy = 0.09;
            this->_sourceRect.w = 5;
            this->_sourceRect.h = 5;
            this->playAnimation("MoveDownLeft");
            break;
        case LEFT:
            this->_dx = -0.12;
            this->_dy = 0.0;
            this->_sourceRect.w = 6;
            this->_sourceRect.h = 3;
            this->playAnimation("MoveLeft");
            break;
        case NONE:
            this->_dx = 0;
            this->_dy = 0;
            this->_sourceRect.w = 17;
            this->_sourceRect.h = 17;
            this->playAnimation("Explode", true); //Play animation once
            break;
    }
}
