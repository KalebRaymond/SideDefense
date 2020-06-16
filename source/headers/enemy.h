#ifndef ENEMY_H
#define ENEMY_H

#include "animated_sprite.h"
#include "globals.h"
#include <string>

class Graphics;
class Tower;
class Projectile;

class Enemy : public AnimatedSprite
{
    public:
        Enemy();
        Enemy(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, int spawnX, int spawnY, int timeToUpdate);

        virtual void update(int elapsedTime);
        virtual void draw(Graphics &graphics);

        virtual void moveDirection(int elapsedTime);

        const inline int getMaxHealth() const { return this->_maxHealth; }
        const inline int getCurrentHealth() const { return this->_currentHealth; }
        void setSpeed(float dx, float _dy);

        void reduceHealth(int dmg);
        /*  handleBulletCollision() decreases _currentHealth by p->_damage. As of right now
        *   enemies don't do anything special when hit depending on their subclass type,
        *   but if they do in the future, this needs to be a virtual void function.
        */
        void handleProjectileCollision(Projectile* p);

        /* attack will at the very least reduce tower's _currentHealth by this->_damage
        */
        virtual void attack(Tower* tower) = 0;

    protected:
        Direction _direction;
        float _dx, _dy;
        int _maxHealth;
        int _currentHealth;
        int _damage;
};

class BasicEnemy : public Enemy
{
    public:
        BasicEnemy();
        BasicEnemy(Graphics &graphics, int spawnX, int spawnY);

        void update(int elapsedTime);
        void draw(Graphics &graphics);

        void moveDirection(int elapsedTime);

        void attack(Tower* tower);
};

class ToughEnemy : public Enemy
{
     public:
        ToughEnemy();
        ToughEnemy(Graphics &graphics, int spawnX, int spawnY);

        void update(int elapsedTime);
        void draw(Graphics &graphics);

        void moveDirection(int elapsedTime);

        void attack(Tower* tower);
};

#endif // ENEMY_H
