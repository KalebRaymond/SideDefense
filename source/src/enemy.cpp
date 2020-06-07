#include "enemy.h"
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
        _currentHealth(0)
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

void Enemy::moveDirection(float elapsedTime)
{

}

void Enemy::setSpeed(float dx, float dy)
{
    this->_dx = dx;
    this->_dy = dy;
}

void Enemy::handleProjectileCollision(int damage)
{
    this->_currentHealth -= damage;
}

//BasicEnemy class

BasicEnemy::BasicEnemy()
{

}

BasicEnemy::BasicEnemy(Graphics &graphics, int spawnX, int spawnY)
    :   Enemy(graphics, "content/sprites/enemy.png", 0, 0, 16, 32, spawnX, spawnY, 150),
        _startingX(spawnX),
        _startingY(spawnY)
{
    this->_maxHealth = 50;
    this->_currentHealth = 50;
    this->_dx = 0.05;
    this->_dy = 0;
    this->_direction = RIGHT;

    this->setupAnimation();
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

void BasicEnemy::moveDirection(float elapsedTime)
{
    //Move by dx
    this->_x += this->_dx * elapsedTime;
    //Move by dy
    this->_y += this->_dy * elapsedTime;
}

void BasicEnemy::animationDone(std::string currentAnimation)
{

}

void BasicEnemy::setupAnimation()
{
    this->addAnimation( 4, 0, 0, "WalkRight", 16, 32, Vector2(0, 0) );
    this->addAnimation( 5, 0, 36, "AttackRight", 16, 32, Vector2(0, 0) ); //Looks like he's moonwalking
}
