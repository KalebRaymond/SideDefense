#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>
#include "globals.h"
#include "animated_sprite.h"


class Graphics;
class Enemy;

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

        virtual void handleEnemyCollision(Enemy* enemy) = 0;

        const inline bool shallBeDestroyed() const
        {
            return _destroy;
        }

        int getDamage();

    protected:
        Direction _direction;
        float _dx, _dy;
        int _damage;
        bool _destroy;
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
        ~Bullet();

        void update(int elapsedTime, std::vector<Enemy*> *enemies);

        /*  Sets _damage to zero. Sets _destroy to true if enemy HP is
        *   zero, or if bullet crosses enemy's sprite's midpoint.
        */
        void handleEnemyCollision(Enemy* enemy);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        void setDirection(Direction direction);
};

/*  Rocket class
*   Used with RocketTower
*/
class Rocket : public Projectile
{
    public:
        Rocket();
        Rocket(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, std::vector<Enemy*> *enemies);

        void handleEnemyCollision(Enemy* enemy);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        void setDirection(Direction direction);

    private:
        int _targetX, _targetY;
        int _lastDirectionChangeTime;
        int _directionChangeCooldown;
};

#endif // PROJECTILE_H
