#include "projectile.h"
#include "enemy.h"
#include "tower.h"
#include "level.h"

Projectile::Projectile()
{

}

Projectile::Projectile(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate)
    :   AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
        _damage(0),
        _dx(0),
        _dy(0),
        _direction(NONE),
        _dealDamage(true)
{

}

Projectile::~Projectile()
{

}

void Projectile::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Projectile::update(int elapsedTime, std::vector<Enemy*> *enemies)
{
    AnimatedSprite::update(elapsedTime);
}

void Projectile::moveDirection(int elapsedTime)
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

void Projectile::handleEnemyCollision(Enemy* enemy)
{

}

void Projectile::handleTowerCollision(Tower* tower)
{

}

void Projectile::handleWallCollision(Level &level)
{

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
    this->_dealDamage = true;
    this->_damage = 1;
    this->_dx = -0.1;
    this->_dy = 0;
    this->_direction = LEFT;

    this->addAnimation(1, 2, 18, "MoveLeft", 4, 3, Vector2(0, 0));
    this->playAnimation("MoveLeft");
}

void Bullet::update(int elapsedTime, std::vector<Enemy*> *enemies)
{
    if(!this->_dealDamage)
    {
        this->_damage = 0;
    }

    if(this->_x < 32)
    {
        this->_destroy = true;
    }

    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void Bullet::handleEnemyCollision(Enemy* enemy)
{
    this->_dealDamage = false;

    if(enemy->getCurrentHealth() <= 0 || this->_x <= enemy->getBoundingBox().getCenterX())
    {
        this->_destroy = true;
    }
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

Rocket::Rocket(Graphics &graphics, Vector2 spawnPoint, int floor)
    :   Projectile(graphics, "content/sprites/rocketTower.png", 9, 19, 4, 6, spawnPoint, 75),
    _targetX(-1),
    _targetY(-1),
    _lastDirectionChangeTime(0),
    _directionChangeCooldown(150),
    _floor(floor)
{
    this->_damage = 0;
    this->_dealDamage = true;
    this->_dx = 0;
    this->_dy = -0.1;
    this->_direction = UP;

    this->addAnimation(1, 2, 20, "MoveUpLeft", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 9, 19, "MoveUp", 3, 6, Vector2(0, 0));
    this->addAnimation(1, 14, 20, "MoveUpRight", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 14, 26, "MoveRight", 6, 3, Vector2(0, 0));
    this->addAnimation(1, 14, 30, "MoveDownRight", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 9, 30, "MoveDown", 3, 6, Vector2(0, 0));
    this->addAnimation(1, 2, 30, "MoveDownLeft", 5, 5, Vector2(0, 0));
    this->addAnimation(1, 1, 26, "MoveLeft", 6, 3, Vector2(0, 0));
    this->addAnimation(5, 17, 1, "Explode", 17, 17, Vector2(-9, -9));

    this->playAnimation("MoveUp");
}

void Rocket::update(int elapsedTime, std::vector<Enemy*> *enemies)
{
   if(this->_currentAnimation == "Explode")
    {
        //Rocket should only do damage on sprite 4 of "Explode" (which has the circle with the biggest radius)
        //and only for 1 frame
        if(this->getFrameIndex() == 4)
        {
            if(this->_damage == 0)
            {
                this->_damage = 2;
            }
            else if(this->_damage == 2)
            {
                this->_damage = 0;
                this->_dealDamage = false;
            }
        }
        //Explosion animation is finished, rocket can now be deleted
        else if(this->_animationDone)
        {
            this->_destroy = true;
        }
    }
    else
    {
        this->_damage = 0;
    }

    if(enemies != nullptr && enemies->size() != 0)
    {
        //Target the first enemy on the same floor as the tower that created this
        Enemy* targetEnemy = nullptr;
        for(int i = 0; i < enemies->size(); ++i)
        {
            targetEnemy = enemies->at(i);
            if(targetEnemy->getBoundingBox().getBottom() == this->_floor)
            {
                break;
            }
        }

        if(targetEnemy != nullptr && targetEnemy->getBoundingBox().getBottom() == this->_floor)
        {
            this->_targetX = targetEnemy->getBoundingBox().getCenterX();
            this->_targetY = targetEnemy->getBoundingBox().getCenterY();
        }
        else
        {
            this->setDirection(NONE);
        }
    }
    else
    {
        //If there aren't any enemies on screen, self destruct
        this->setDirection(NONE);
    }

    if(this->_x < 32)
    {
        //If rocket somehow hits wall, self destruct
        this->setDirection(NONE);
    }

    //Increase time since last direction change
    _lastDirectionChangeTime += elapsedTime;

    //Only change direction every 100ms
    if(_lastDirectionChangeTime >= _directionChangeCooldown && this->_currentAnimation != "Explode")
    {
        _lastDirectionChangeTime = 0;

        //If rocket is within 3 pixels of target, explode.
        if((pow(this->_targetX - this->_x, 2) + pow(this->_targetY - this->_y, 2)) <= 9 )
        {
            this->setDirection(NONE);
        }
        //Otherwise, get angle to target in degrees
        else
        {
            float angleToTarget = functions::getAngle(this->_x, this->_y, this->_targetX, this->_targetY);

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

void Rocket::handleEnemyCollision(Enemy* enemy)
{
    this->setDirection(NONE);
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
            if(this->_currentAnimation != "Explode")
            {
                this->playAnimation("Explode");
            }
            break;
    }
}

//Fireball subclass

Fireball::Fireball()
{

}

Fireball::Fireball(Graphics &graphics, Vector2 spawnPoint)
    :   Projectile(graphics, "content/sprites/fireEnemy.png", 74, 2, 6, 5, spawnPoint, 100)
{
    this->_dealDamage = true;
    this->_damage = 1;
    this->_dx = 0.1;
    this->_dy = 0;
    this->_direction = RIGHT;

    this->addAnimation(4, 74, 2, "MoveRight", 6, 5, Vector2(0, 0));
    this->playAnimation("MoveRight");
}

void Fireball::update(int elapsedTime, std::vector<Enemy*> *enemies)
{
    if(!this->_dealDamage)
    {
        this->_damage = 0;
    }

    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void Fireball::handleTowerCollision(Tower* tower)
{
    if(this->_dealDamage)
    {
        tower->reduceCurrentHealth( this->_damage );
    }

    this->_dealDamage = false;

    if(tower->getCurrentHealth() <= 0 || this->getBoundingBox().getCenterX() >= tower->getX())
    {
        this->_destroy = true;
    }
}

void Fireball::handleWallCollision(Level &level)
{
    if(this->_dealDamage)
    {
       level.reduceHealth(this->_damage);
    }

    this->_dealDamage = false;
}

void Fireball::setDirection(Direction direction)
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
