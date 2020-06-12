#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>
#include "globals.h"
#include "animated_sprite.h"


class Graphics;

class Projectile : public AnimatedSprite
{
    public:
        Projectile();
        Projectile(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
        ~Projectile();

        void draw(Graphics &graphics);
        virtual void update(float elapsedTime);

        void moveDirection(float elapsedTime);
        virtual void setDirection(Direction direction);

        int getDamage();

    protected:
        float _dx, _dy;
        Direction _direction;
        int _damage;

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

        void update(float elapsedTime);

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

        float getAngleToTarget();

        void update(float elapsedTime);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        void setDirection(Direction direction);

    private:
        int _targetX, _targetY;
};

#endif // PROJECTILE_H
