#include "tower.h"
#include "enemy.h"
#include "projectile.h"
#include "menu_item.h"
#include "graphics.h"

Tower::Tower()
{

}

Tower::Tower(Graphics &graphics, std::string filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate, int maxHealth)
    :   AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
        _maxHealth(maxHealth),
        _currentHealth(maxHealth),
        _fireCoolDown(0),
        _lastFireTime(0),
        _dragged(false),
        _placed(false),
        _selected(false),
        _valid(false),
        _price(0),
        _canPlaceOnWall(false)
{

}

void Tower::update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies)
{
    AnimatedSprite::update(elapsedTime);
}

void Tower::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

Projectile* Tower::fireProjectile(Graphics &graphics, int elapsedTime)
{
    return nullptr;
}

bool Tower::getDragged()
{
    return this->_dragged;
}

void Tower::setDragged(bool dragged)
{
    this->_dragged = dragged;
}

bool Tower::getPlaced()
{
    return this->_placed;
}

void Tower::setPlaced(bool placed)
{
    this->_placed = placed;
}

void Tower::setSelected(bool selected)
{
    this->_selected = selected;
}

bool Tower::isPositionValid()
{
    return this->_valid;
}

void Tower::setPositionValid(bool valid)
{
    this->_valid = valid;
}

bool Tower::canPlaceOnWall()
{
    return this->_canPlaceOnWall;
}

bool Tower::pointCollides(int x, int y)
{
    Rectangle rect = this->getBoundingBox();

    return  x >= rect.getLeft() && x <= rect.getRight()
            && y >= rect.getTop() && y <= rect.getBottom();
}

void Tower::setCurrentHealth(int hp)
{
    this->_currentHealth = hp;
}

void Tower::reduceCurrentHealth(int dmg)
{
    this->_currentHealth -= dmg;
}

int Tower::getCurrentHealth()
{
    return this->_currentHealth;
}

int Tower::getPrice()
{
    return this->_price;
}

//BulletTower class

BulletTower::BulletTower()
{

}

BulletTower::BulletTower(Graphics &graphics, Vector2 spawnPoint)
    :   Tower(graphics, "content/sprites/bulletTower.png", 0, 0, 16, 16, spawnPoint, 100, 5)
{
    this->_fireCoolDown = 1000;
    this->_lastFireTime = -1;
    this->_price = 100;

    this->addAnimation(1, 0, 0, "FaceLeft", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 16, 0, "Selected", 17, 16, Vector2(-2, 0));
    this->playAnimation("FaceLeft");
}

void BulletTower::update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies)
{
    if(this->_selected)
    {
        this->_sourceRect.w = 17;
        this->playAnimation("Selected");
    }
    else
    {
        this->_sourceRect.w = 16;
        this->playAnimation("FaceLeft");
    }

    Sprite::update();
}

void BulletTower::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

Projectile* BulletTower::fireProjectile(Graphics &graphics, int elapsedTime)
{
    if(( _lastFireTime > _fireCoolDown || _lastFireTime < 0 ) && !this->_dragged)
    {
        _lastFireTime = 0;
        return new Bullet(graphics, Vector2( this->_x, this->_y + 11 ));
    }
    else
    {
        this->_lastFireTime += elapsedTime;
        return nullptr;
    }
}

std::pair< TowerMenuItem*, TowerMenuItem* > BulletTower::getMenuItems(Graphics &graphics)
{
    //Memory leak
    TowerMenuItem* from = new TowerMenuItem("bulletStatic", 0, graphics, 58, 3, 40, 40, Vector2(200, globals::GAME_VIEWPORT_H), false);
    TowerMenuItem* to = new TowerMenuItem("bulletUpgrade", 150, graphics, 142, 3, 40, 40, Vector2(475, globals::GAME_VIEWPORT_H), true);

    return std::make_pair( from, to );
}

//RocketTower class

RocketTower::RocketTower()
{

}

RocketTower::RocketTower(Graphics &graphics, Vector2 spawnPoint)
    :   Tower(graphics, "content/sprites/rocketTower.png", 0, 0, 16, 16, spawnPoint, 100, 5)
{
    this->_fireCoolDown = 2000;
    this->_lastFireTime = -1;
    this->_price = 250;

    this->addAnimation(1, 0, 0, "FaceLeft", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 22, 16, "Selected", 17, 16, Vector2(-2, 0));
    this->playAnimation("FaceLeft");
}

void RocketTower::update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies)
{
    if(this->_selected)
    {
        this->_sourceRect.w = 17;
        this->playAnimation("Selected");
    }
    else
    {
        this->_sourceRect.w = 16;
        this->playAnimation("FaceLeft");
    }

    Sprite::update();
}

void RocketTower::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

Projectile* RocketTower::fireProjectile(Graphics &graphics, int elapsedTime)
{
    if(( _lastFireTime > _fireCoolDown || _lastFireTime < 0 ) && !this->_dragged)
    {
        _lastFireTime = 0;
        return new Rocket(graphics, Vector2( this->_x + 12, this->_y + 6 ));
    }
    else
    {
        this->_lastFireTime += elapsedTime;
        return nullptr;
    }
}

std::pair< TowerMenuItem*, TowerMenuItem* > RocketTower::getMenuItems(Graphics &graphics)
{
    TowerMenuItem* from = new TowerMenuItem("rocketStatic", 0, graphics, 58, 45, 40, 40, Vector2(200, globals::GAME_VIEWPORT_H), false);
    TowerMenuItem* to = new TowerMenuItem("rocketUpgrade", 300, graphics, 142, 45, 40, 40, Vector2(475, globals::GAME_VIEWPORT_H), true);

    return std::make_pair( from, to );
}

//SniperTower class

SniperTower::SniperTower()
{

}

SniperTower::SniperTower(Graphics &graphics, Vector2 spawnPoint)
    :   Tower(graphics, "content/sprites/sniperTower.png", 0, 0, 16, 16, spawnPoint, 100, 5),
        _barrelX( spawnPoint.x + 4),
        _barrelY( spawnPoint.y + 7),
        _damage(1),
        _laserLifespan(0)
{
    this->_fireCoolDown = 1000;
    this->_lastFireTime = 0;
    this->_price = 75;
    this->_canPlaceOnWall = true;

    this->addAnimation(1, 0, 0, "Left", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 17, 0, "Down15", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 34, 0, "Down30", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 51, 0, "Down45", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 68, 0, "Down60", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 17, 17, "Up15", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 34, 17, "Up30", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 51, 17, "Up45", 16, 16, Vector2(0, 0));
    this->addAnimation(1, 68, 17, "Up60", 16, 16, Vector2(0, 0));

    this->addAnimation(1, 0, 35, "LeftRed", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 17, 35, "Down15Red", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 34, 35, "Down30Red", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 51, 35, "Down45Red", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 68, 35, "Down60Red", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 17, 55, "Up15Red", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 34, 55, "Up30Red", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 51, 55, "Up45Red", 16, 18, Vector2(0, -2));
    this->addAnimation(1, 68, 55, "Up60Red", 16, 18, Vector2(0, -2));

    this->playAnimation("Left");
}


void SniperTower::update(int elapsedTime, Graphics &graphics, std::vector<Enemy*> *enemies)
{
    //Attack enemy, change animation & draw line here
    int centerX = this->getBoundingBox().getCenterX();
    int centerY = this->getBoundingBox().getCenterY();

    //Initialize target to just behind tower, which will cause tower
    //to face left by default
    int targetX = centerX - 1;
    int targetY = centerY;
    if(!this->_dragged && enemies->size() > 0 && enemies->at(0)->getBoundingBox().getRight() < this->_x)
    {
        targetX = enemies->at(0)->getBoundingBox().getCenterX();
        targetY = enemies->at(0)->getBoundingBox().getCenterY();

        if( this->_lastFireTime > this->_fireCoolDown )
        {
            this->_lastFireTime = 0;
            this->_laserLifespan = 200;

            //Deal _damage to enemy
            enemies->at(0)->reduceHealth(this->_damage);
        }
        else
        {
            this->_lastFireTime += elapsedTime;
        }
    }
    else
    {
        this->_laserLifespan = 0;
    }

    if(this->_laserLifespan > 0)
    {
        int offsetX = graphics.getCameraOffsetX();
        int offsetY = graphics.getCameraOffsetY();

        //Draw laser
        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
        SDL_RenderDrawLine(graphics.getRenderer(), this->_barrelX + offsetX, this->_barrelY + offsetY, targetX + offsetX, targetY + offsetY);
        SDL_RenderPresent(graphics.getRenderer());

        this->_laserLifespan -= elapsedTime;
    }

    float angleToTarget = functions::getAngle(centerX, centerY, targetX, targetY);

    if(angleToTarget > 175 || angleToTarget < -175 || abs(angleToTarget) < 90)
    {
        this->_barrelX = this->_x + 8;
        this->_barrelY = this->_y + 14;
        this->playAnimation("Left");
    }
    else if(angleToTarget <= 175 && angleToTarget > 160)
    {
        this->_barrelX = this->_x + 8;
        this->_barrelY = this->_y + 10;
        this->playAnimation("Up15");
    }
    else if(angleToTarget <= 160 && angleToTarget > 140)
    {
        this->_barrelX = this->_x + 8;
        this->_barrelY = this->_y + 6;
        this->playAnimation("Up30");
    }
    else if(angleToTarget <= 140 && angleToTarget > 115)
    {
        this->_barrelX = this->_x + 10;
        this->_barrelY = this->_y + 2;
        this->playAnimation("Up45");
    }
    else if(angleToTarget <= 115 && angleToTarget > 90)
    {
        this->_barrelX = this->_x + 14;
        this->_barrelY = this->_y + 0;
        this->playAnimation("Up60");
    }
    else if(angleToTarget >= -175 && angleToTarget < -160)
    {
        this->_barrelX = this->_x + 8;
        this->_barrelY = this->_y + 20;
        this->playAnimation("Down15");
    }
    else if(angleToTarget >= -160 && angleToTarget < -140)
    {
        this->_barrelX = this->_x + 12;
        this->_barrelY = this->_y + 22;
        this->playAnimation("Down30");
    }
    else if(angleToTarget >= -140 && angleToTarget < -115)
    {
        this->_barrelX = this->_x + 12;
        this->_barrelY = this->_y + 28;
        this->playAnimation("Down45");
    }
    else if(angleToTarget >= -115 && angleToTarget < -90)
    {
        this->_barrelX = this->_x + 14;
        this->_barrelY = this->_y + 32;
        this->playAnimation("Down60");
    }

    if(this->_selected)
    {
        this->_sourceRect.h = 18;
        this->_currentAnimation += "Red";
        this->playAnimation( this->_currentAnimation );
    }
    else
    {
        this->_sourceRect.h = 16;
    }

    Sprite::update();
}

void SniperTower::draw(Graphics &graphics)
{
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

Projectile* SniperTower::fireProjectile(Graphics &graphics, int elapsedTime)
{
    return nullptr;
}

std::pair< TowerMenuItem*, TowerMenuItem* > SniperTower::getMenuItems(Graphics &graphics)
{
    TowerMenuItem* from = new TowerMenuItem("sniperStatic", 0, graphics, 58, 87, 40, 40, Vector2(200, globals::GAME_VIEWPORT_H), false);
    TowerMenuItem* to = new TowerMenuItem("sniperUpgrade", 300, graphics, 142, 87, 40, 40, Vector2(475, globals::GAME_VIEWPORT_H), true);

    return std::make_pair( from, to );
}
