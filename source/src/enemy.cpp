#include "enemy.h"
#include "tower.h"
#include "projectile.h"
#include "level.h"
#include "graphics.h"

Enemy::Enemy()
{

}

Enemy::Enemy(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, int spawnX, int spawnY, int timeToUpdate)
    :   AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnX, spawnY, timeToUpdate),
        _dx(0),
        _dy(0),
        _direction(NONE),
        _maxHealth(0),
        _currentHealth(0),
        _damage(0),
        _value(0),
        _fireCoolDown(0),
        _lastFireTime(0),
        _dealDamage(false)
{
}

void Enemy::update(int elapsedTime)
{
    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Enemy::moveDirection(int elapsedTime)
{
    this->_x += this->_dx * elapsedTime;
    this->_y += this->_dy * elapsedTime;
}

void Enemy::setSpeed(float dx, float dy)
{
    this->_dx = dx;
    this->_dy = dy;
}

void Enemy::reduceHealth(int dmg)
{
    this->_currentHealth -= dmg;
}

void Enemy::handleProjectileCollision(Projectile* p)
{
    if(p->dealsDamage())
    {
        this->reduceHealth( p->getDamage() );
    }
}

void Enemy::handleWallCollision(Level &level)
{
    if(this->getFrameIndex() == 3)
    {
        if(this->_dealDamage)
        {
            level.reduceHealth(this->_damage);
        }

        this->_dealDamage = false;
    }
    else
    {
        this->_dealDamage = true;
    }
}

void Enemy::attack(Tower* tower)
{
    if(this->getFrameIndex() == 3)
    {
        if(this->_dealDamage)
        {
            tower->reduceCurrentHealth(this->_damage);
        }

        this->_dealDamage = false;
    }
    else
    {
        this->_dealDamage = true;
    }
}

//BasicEnemy class

BasicEnemy::BasicEnemy()
{

}

BasicEnemy::BasicEnemy(Graphics &graphics, int spawnX, int spawnY)
    :   Enemy(graphics, "content/sprites/basicEnemy.png", 0, 0, 16, 32, spawnX, spawnY, 150)
{
    this->_maxHealth = 3;
    this->_currentHealth = 3;
    this->_damage = 1;
    this->_dx = 0.04;
    this->_dy = 0;
    this->_direction = RIGHT;
    this->_value = 25;

    this->addAnimation( 4, 0, 0, "WalkRight", 16, 32, Vector2(0, 0) );
    this->addAnimation( 5, 0, 36, "AttackRight", 16, 32, Vector2(0, 0) );

    this->playAnimation("WalkRight");
}



Projectile* BasicEnemy::fireProjectile(Graphics &graphics, int elapsedTime)
{
    return nullptr;
}

//ToughEnemy class

ToughEnemy::ToughEnemy()
{

}

ToughEnemy::ToughEnemy(Graphics &graphics, int spawnX, int spawnY)
    :   Enemy(graphics, "content/sprites/toughEnemy.png", 0, 0, 16, 32, spawnX, spawnY, 150)
{
    this->_maxHealth = 6;
    this->_currentHealth = 6;
    this->_damage = 1;
    this->_dx = 0.04;
    this->_dy = 0;
    this->_direction = RIGHT;
    this->_value = 50;

    this->addAnimation( 4, 0, 0, "WalkRight", 16, 32, Vector2(0, 0) );
    this->addAnimation( 5, 2, 36, "AttackRight", 16, 32, Vector2(0, 0) );

    this->playAnimation("WalkRight");
}

Projectile* ToughEnemy::fireProjectile(Graphics &graphics, int elapsedTime)
{
    return nullptr;
}

//FireEnemy class

FireEnemy::FireEnemy()
{

}

FireEnemy::FireEnemy(Graphics &graphics, int spawnX, int spawnY)
    :   Enemy(graphics, "content/sprites/fireEnemy.png", 0, 0, 18, 32, spawnX, spawnY, 150),
        _canShoot(false)
{
    this->_maxHealth = 4;
    this->_currentHealth = 4;
    this->_damage = 1;
    this->_dx = 0.04;
    this->_dy = 0;
    this->_direction = RIGHT;
    this->_value = 65;

    this->_fireCoolDown = 3000;

    this->addAnimation( 4, 0, 0, "WalkRight", 18, 32, Vector2(0, 0) );
    this->addAnimation( 5, 2, 36, "AttackRight", 18, 32, Vector2(0, 0) );
    this->addAnimation( 6, 0, 71, "Fire", 28, 32, Vector2(0, 0) );

    this->playAnimation("WalkRight");
}

void FireEnemy::update(int elapsedTime)
{
    if(this->_currentAnimation != "AttackRight" && this->_lastFireTime > this->_fireCoolDown)
    {
        this->_lastFireTime = 0;
        this->_canShoot = true;
        this->_sourceRect.w = 28;
        this->setSpeed(0.0, 0.0);
        this->playAnimation("Fire", true);
    }
    else
    {
        if(this->_currentAnimation != "Fire")
        {
            this->_sourceRect.w = 18;
        }

        this->_lastFireTime += elapsedTime;
    }

    if(this->_currentAnimation == "Fire" && this->_animationDone)
    {
        this->_sourceRect.w = 18;
        this->setSpeed(0.04, 0.0);
        this->playAnimation("WalkRight");
    }

    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

Projectile* FireEnemy::fireProjectile(Graphics &graphics, int elapsedTime)
{
    if(this->_currentAnimation == "Fire" && this->getFrameIndex() == 3 && this->_canShoot)
    {
        this->_canShoot = false;
        return new Fireball(graphics, Vector2(this->_x + 24, this->_y + 32));
    }
}
