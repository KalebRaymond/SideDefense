#ifndef ENEMY_H
#define ENEMY_H

#include "animated_sprite.h"
#include "globals.h"
#include <string>

class Graphics;

class Enemy : public AnimatedSprite
{
    public:
        Enemy();
        Enemy(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, int spawnX, int spawnY, int timeToUpdate);

        virtual void update(int elapsedTime);
        virtual void draw(Graphics &graphics);

        virtual void moveDirection(float elapsedTime);

        const inline int getMaxHealth() const { return this->_maxHealth; }
        const inline int getCurrentHealth() const { return this->_currentHealth; }
        void setSpeed(float dx, float _dy);

        //handleBulletCollision decreases _currentHealth by damage. As of right now
        //enemies don't do anything special when hit depending on their subclass type,
        //but if they do in the future, this needs to be a virtual void function.
        void handleProjectileCollision(int damage);

    protected:
        float _dx, _dy;
        Direction _direction;
        int _maxHealth;
        int _currentHealth;
        //Enemy* _targetTower;
};

class BasicEnemy : public Enemy
{
    public:
        BasicEnemy();
        BasicEnemy(Graphics &graphics, int spawnX, int spawnY);

        void update(int elapsedTime);
        void draw(Graphics &graphics);

        void moveDirection(float elapsedTime);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        //Move

    private:
        float _startingX, _startingY;

};

#endif // ENEMY_H
