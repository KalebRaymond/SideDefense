#include "enemy.h"
#include "tower.h"
#include "projectile.h"
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
        _damage(0)
{
}

void Enemy::update(int elapsedTime)
{
    AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Enemy::moveDirection(int elapsedTime)
{

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

//BasicEnemy class

BasicEnemy::BasicEnemy()
{

}

BasicEnemy::BasicEnemy(Graphics &graphics, int spawnX, int spawnY)
    :   Enemy(graphics, "content/sprites/basicEnemy.png", 0, 0, 16, 32, spawnX, spawnY, 150)
{
    this->_maxHealth = 10;
    this->_currentHealth = 1000;
    this->_damage = 1;
    this->_dx = 0.05;
    this->_dy = 0;
    this->_direction = RIGHT;

    this->addAnimation( 4, 0, 0, "WalkRight", 16, 32, Vector2(0, 0) );
    this->addAnimation( 5, 0, 36, "AttackRight", 16, 32, Vector2(0, 0) );

    this->playAnimation("WalkRight");
}

void BasicEnemy::update(int elapsedTime)
{
    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void BasicEnemy::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void BasicEnemy::moveDirection(int elapsedTime)
{
    this->_x += this->_dx * elapsedTime;
    this->_y += this->_dy * elapsedTime;
}

void BasicEnemy::attack(Tower* tower)
{
    if(this->getFrameIndex() == 3)
    {
        tower->reduceCurrentHealth(this->_damage);
    }
}

//ToughEnemy class

ToughEnemy::ToughEnemy()
{

}

ToughEnemy::ToughEnemy(Graphics &graphics, int spawnX, int spawnY)
    :   Enemy(graphics, "content/sprites/toughEnemy.png", 0, 0, 16, 32, spawnX, spawnY, 150)
{
    this->_maxHealth = 10;
    this->_currentHealth = 10000;
    this->_damage = 0;
    this->_dx = 0.05;
    this->_dy = 0;
    this->_direction = RIGHT;

    this->addAnimation( 4, 0, 0, "WalkRight", 16, 32, Vector2(0, 0) );
    this->addAnimation( 5, 2, 36, "AttackRight", 16, 32, Vector2(0, 0) );

    this->playAnimation("WalkRight");
}

void ToughEnemy::update(int elapsedTime)
{
    std::cout << this->_currentHealth << "\n";

    this->moveDirection(elapsedTime);
    AnimatedSprite::update(elapsedTime);
}

void ToughEnemy::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void ToughEnemy::moveDirection(int elapsedTime)
{
    this->_x += this->_dx * elapsedTime;
    this->_y += this->_dy * elapsedTime;
}

void ToughEnemy::attack(Tower* tower)
{
    if(this->getFrameIndex() == 3)
    {
        tower->reduceCurrentHealth(this->_damage);
    }
}
