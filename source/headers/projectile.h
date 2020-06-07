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

        virtual void draw(Graphics &graphics);
        virtual void update(float elapsedTime);

        //Direction might be different for certain projectiles (homing missiles)
        //Make a namesapce with LEFT, RIGHT, UPRIGHT etc?
        virtual void moveDirection(float elapsedTime);
        virtual void setDirection(Direction direction);

        int getDamage();

        //virtual void handleEnemyCollisions(std::vector<Enemy*> enemies)
        //virtual void handleTileCollisions(std::vector<Tile> tiles)

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

        void draw(Graphics &graphics);
        void update(float elapsedTime);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        void moveDirection(float elapsedTime);
        void setDirection(Direction direction);

    protected:
};

#endif // PROJECTILE_H
