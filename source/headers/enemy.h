#ifndef ENEMY_H
#define ENEMY_H

#include "animated_sprite.h"
#include "globals.h"
#include <string>

class Graphics;
class Tower;
class Projectile;
class Level;

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
        const inline int getValue() const { return this->_value; }
        void setSpeed(float dx, float _dy);

        void reduceHealth(int dmg);

        /*  handleProjectileCollision() decreases _currentHealth by p->_damage. As of right now
        *   enemies don't do anything special when hit depending on their subclass type,
        *   but if they do in the future, this needs to be a virtual void function.
        */
        void handleProjectileCollision(Projectile* p);

        /*  handleWallCollision() reduces player's health by this->_damage.
        */
        void handleWallCollision(Level &level);


        /* attack reduces tower's _currentHealth by this->_damage
        */
        void attack(Tower* tower);

        /*  Returns nullptr here. In derived classes, fireProjectile() increments
        *   _lastFireTime by elapsedTime. If _lastFireTime > _fireCoolDown, a pointer
        *   to a projectile subclass will be returned.
        */
        virtual Projectile* fireProjectile(Graphics &graphics, int elapsedTime) = 0;

    protected:
        Direction _direction;
        float _dx, _dy;
        int _maxHealth;
        int _currentHealth;
        int _damage;
        int _value;

        //_fireCoolDown and _lastFireTime are measured in milliseconds
        int _fireCoolDown;
        int _lastFireTime;

        bool _dealDamage;
};

class BasicEnemy : public Enemy
{
    public:
        BasicEnemy();
        BasicEnemy(Graphics &graphics, int spawnX, int spawnY);

        void attack(Tower* tower);

        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);
};

class ToughEnemy : public Enemy
{
     public:
        ToughEnemy();
        ToughEnemy(Graphics &graphics, int spawnX, int spawnY);

        void attack(Tower* tower);

        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);
};

class FireEnemy : public Enemy
{
    public:
        FireEnemy();
        FireEnemy(Graphics &graphics, int spawnX, int spawnY);

        void update(int elapsedTime);

        void attack(Tower* tower);

        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);

    private:
        bool _canShoot;
};

#endif // ENEMY_H
