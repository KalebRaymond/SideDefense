#ifndef TOWER_H
#define TOWER_H

#include <string>
#include "animated_sprite.h"
#include "globals.h"

class Graphics;
class Projectile;

class Tower : public AnimatedSprite
{
    public:
        Tower();
        Tower(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate, int maxHealth);

        virtual void update(int elapsedTime, Graphics &graphics);
        virtual void draw(Graphics &graphics);

        virtual Projectile* fireProjectile(Graphics &graphics, int elapsedTime);

        bool getDragged();
        void setDragged(bool dragged);
        bool getPlaced();
        void setPlaced(bool placed);

        //All towers react to mouse-overs the same way. The user can click and drag
        //a tower to place it in the level (if enough cash)...
        //Returns true if the point (x, y) is inside the bounding box for this tower
        bool pointCollides(int x, int y);
        virtual void handleMouseEvent(int mouseX, int mouseY);

        void setCurrentHealth(int hp);
        void reduceCurrentHealth(int dmg);
        int getCurrentHealth();
        int getPrice();

    protected:
        //Direction _direction
        float _startingX, _startingY; //I'm not totally sure these even do anything
        int _maxHealth;
        int _currentHealth;
        bool _dragged;
        bool _placed;

        //_fireCoolDown and _lastFireTime are measured in milliseconds
        int _fireCoolDown;
        int _lastFireTime;

        int _price;

};

class Bullet;

class BulletTower : public Tower
{
    public:
        BulletTower();
        BulletTower(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, Graphics &graphics);
        void draw(Graphics &graphics);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);

        void handleMouseEvent(int mouseX, int mouseY);

    private:
};


class RocketTower : public Tower
{
    public:
        RocketTower();
        RocketTower(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, Graphics &graphics);
        void draw(Graphics &graphics);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);

        void handleMouseEvent(int mouseX, int mouseY);

    private:
};

#endif // TOWER_H
