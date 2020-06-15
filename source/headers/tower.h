#ifndef TOWER_H
#define TOWER_H

#include <string>
#include "animated_sprite.h"
#include "globals.h"

class Graphics;
class Enemy;
class Projectile;

class Tower : public AnimatedSprite
{
    public:
        Tower();
        Tower(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate, int maxHealth);

        virtual void update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies);
        virtual void draw(Graphics &graphics);

        /*  Returns nullptr here. In derived classes, fireProjectile() increments
        *   _lastFireTime by elapsedTime. If _lastFireTime > _fireCoolDown and this
        *   tower is not being dragged, returns a pointer to a projectile subclass
        *   corresponding to the type of the tower. Otherwise returns nullptr
        */
        virtual Projectile* fireProjectile(Graphics &graphics, int elapsedTime);

        bool getDragged();
        void setDragged(bool dragged);
        bool getPlaced();
        void setPlaced(bool placed);
        bool isPositionValid();
        void setPositionValid(bool valid);

        bool canPlaceOnWall();

        /*pointCollides() returns true if the point (x, y) is inside the bounding box for this tower
        */
        bool pointCollides(int x, int y);

        void setCurrentHealth(int hp);
        void reduceCurrentHealth(int dmg);
        int getCurrentHealth();
        int getPrice();

    protected:
        int _maxHealth;
        int _currentHealth;

        //_fireCoolDown and _lastFireTime are measured in milliseconds
        int _fireCoolDown;
        int _lastFireTime;

        int _price;

        bool _dragged;
        bool _placed;
        bool _valid;
        bool _canPlaceOnWall;
};

class BulletTower : public Tower
{
    public:
        BulletTower();
        BulletTower(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies);
        void draw(Graphics &graphics);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);

};

class RocketTower : public Tower
{
    public:
        RocketTower();
        RocketTower(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies);
        void draw(Graphics &graphics);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);
};

class SniperTower : public Tower
{
    public:
        SniperTower();
        SniperTower(Graphics &graphics, Vector2 spawnPoint);

        void update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies);
        void draw(Graphics &graphics);

        void animationDone(std::string currentAnimation);
        void setupAnimation();

        /*  SniperTower::fireProjectile will always return nullptr, because rather
        *   than fire a projectile, the tower directly reduces an enemy's HP and
        *   draws a line.
        */
        Projectile* fireProjectile(Graphics &graphics, int elapsedTime);

    private:
        int _barrelX, _barrelY;
        int _damage;

        /*  _laserLifespan dictates the amount of time a laser rendered from this
        *   class will remain on the screen. Regardless of this value, a laser will
        *   only deal damage on the first frame it is created.
        */
        int _laserLifespan;
};

#endif // TOWER_H
