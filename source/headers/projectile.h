#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>
#include "globals.h"
#include "animated_sprite.h"


class Graphics;
class Enemy;
class Tower;
class Level;

class Projectile : public AnimatedSprite
{
    public:
        Projectile();
        Projectile(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
        ~Projectile();

        void draw(Graphics &graphics);
        virtual void update(int elapsedTime, std::vector<Enemy*> *enemies);

        void moveDirection(int elapsedTime);
        virtual void setDirection(Direction direction);

        virtual void handleEnemyCollision(Enemy* enemy);
        virtual void handleTowerCollision(Tower* tower);
        virtual void handleWallCollision(Level &level);

        const inline bool dealsDamage() const
        {
            return this->_dealDamage;
        }

        int getDamage();

    protected:
        Direction _direction;
        float _dx, _dy;
        int _damage;
        bool _dealDamage;
};

/*  Bullet class
*   Used with bullet tower class
*/
class Bullet : public Projectile
{
    public:
        Bullet();
        Bullet(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, std::vector<Enemy*> *enemies);

        /*  Sets _damage to zero. Sets _destroy to true if enemy HP is
        *   zero, or if bullet crosses enemy's sprite's midpoint.
        */
        void handleEnemyCollision(Enemy* enemy);

        void animationDone(std::string currentAnimation);

        void setDirection(Direction direction);
};

/*  Rocket class
*   Used with RocketTower
*/
class Rocket : public Projectile
{
    public:
        Rocket();
        Rocket(Graphics &graphics, Vector2 spawnPoint, int floor);

        void update(int elapsedTime, std::vector<Enemy*> *enemies);

        void handleEnemyCollision(Enemy* enemy);

        void setDirection(Direction direction);

    private:
        int _targetX, _targetY;
        int _lastDirectionChangeTime;
        int _directionChangeCooldown;

        /*  This rocket can only follow enemies that are on the same
        *   floor as the tower that created the rocket.
        */
        int _floor;
};


/*  Fireball class
*   Used with fire enemy class
*/
class Fireball : public Projectile
{
    public:
        Fireball();
        Fireball(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, std::vector<Enemy*> *enemies);

        /*  Reduces tower's HP by this->_damage. Sets _damage to zero. Sets
        *   _destroy to true if Fireball crosses tower's sprite's midpoint.
        */
        void handleTowerCollision(Tower* tower);

        /*  handleWallCollision reduces player's health by this->_damage
        */
        void handleWallCollision(Level &level);

        void animationDone(std::string currentAnimation);

        void setDirection(Direction direction);
};

#endif // PROJECTILE_H
