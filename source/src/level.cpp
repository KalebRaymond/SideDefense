#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "tinyxml2.h"
#include "rectangle.h"
#include "enemy.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level()
{

}

Level::Level(std::string mapName, Graphics& graphics)
    :   _size(Vector2(0, 0)),
        _mapName(mapName),
        _doorOpenTime(4500),
        _health(100),
        _money(200)
{
    this->loadMap(mapName, graphics);
}

Level::~Level()
{

}

void Level::loadMap(std::string mapName, Graphics& graphics)
{
    //Add doors to level
    AnimatedSprite door = AnimatedSprite(graphics, "content/sprites/misc.png", 0, 7, 18, 40, 32, globals::GAME_VIEWPORT_H - 64 - 50, 100);
    door.addAnimation(4, 0, 7, "Opening", 19, 40, Vector2(0, 0));
    door.addAnimation(1, 57, 7, "Opened", 19, 40, Vector2(0, 0));
    door.addAnimation(1, 0, 0, "Closed", 0, 0, Vector2(0, 0));
    door.playAnimation("Closed");
    this->_doors.push_back(door);

    if(mapName == "level3")
    {
        door.setY(globals::GAME_VIEWPORT_H - 288 - 50);
        this->_doors.push_back(door);
    }

    //Parse tmx file
    XMLDocument doc;
    std::stringstream ss;
    ss << "content/maps/" << mapName << ".tmx";
    doc.LoadFile( ss.str().c_str() );

    XMLElement* mapNode = doc.FirstChildElement("map");

    //Get width and height of whole map and store it in _size
    int width, height;
    mapNode->QueryAttribute("width", &width);
    mapNode->QueryAttribute("height", &height);
    this->_size = Vector2(width, height);

    //Get the width and height of tiles and store it in _tileSize
    int tileWidth, tileHeight;
    mapNode->QueryAttribute("tilewidth", &tileWidth);
    mapNode->QueryAttribute("tileheight", &tileHeight);
    this->_tileSize = Vector2(tileWidth, tileHeight);

    //Load the tilesets
    XMLElement* pTileset = mapNode->FirstChildElement("tileset");
    if(pTileset != NULL)
    {
        while(pTileset)
        {
            /*The filename of the tileset source is saved in a custom property in <properties>
            / under "value". I only intend on ever adding this one custom property, so here I
            / can get away with not doing a while loop, but there will be errors if I forget
            / to add the property in Tiled, or if I add more than one custom property.*/
            XMLElement* pProperty = mapNode->FirstChildElement("properties")->FirstChildElement("property");
            std::string source = "content/tilesets/";
            source.append(pProperty->Attribute("value"));

            int firstGid;
            pTileset->QueryAttribute("firstgid", &firstGid);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(source.c_str()));
            this->_tilesets.push_back( Tileset(tex, firstGid) );

            pTileset = pTileset->NextSiblingElement("tileset");
        }
    }

    //Load the layers
    XMLElement* pLayer = mapNode->FirstChildElement("layer");
    if(pLayer != NULL)
    {
        while(pLayer)
        {
            //Load the data element
            XMLElement* pData = pLayer->FirstChildElement("data");
            if(pData != NULL)
            {
                while(pData)
                {
                    //Load the tile element
                    XMLElement* pTile = pData->FirstChildElement("tile");
                    if(pTile != NULL)
                    {
                        int tileCounter = 0;
                        while(pTile)
                        {
                            //Build each individual tile
                            if(pTile->IntAttribute("gid") == 0)
                            {
                                tileCounter++;
                                if(pTile->NextSiblingElement("tile"))
                                {
                                    pTile = pTile->NextSiblingElement("tile");
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            //Get tileset for specific gid
                            int gid = pTile->IntAttribute("gid");
                            Tileset tls;
                            int closest = 0;
                            for(int i = 0; i < this->_tilesets.size(); ++i)
                            {
                                if(this->_tilesets[i].firstGid <= gid)
                                {
                                    if(this->_tilesets[i].firstGid > closest)
                                    {
                                        closest = this->_tilesets[i].firstGid;
                                        tls = this->_tilesets.at(i);
                                    }
                                }
                            }

                            if(tls.firstGid == -1)
                            {
                                //No tileset found for this gid
                                tileCounter++;
                                if(pTile->NextSiblingElement("tile"))
                                {
                                    pTile = pTile->NextSiblingElement("tile");
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            //Get the position of the tile in the level
                            int xx = 0;
                            int yy = 0;
                            xx = tileCounter % width;
                            xx *= tileWidth;

                            yy += tileHeight * (tileCounter / width);
                            Vector2 finalTilePosition = Vector2(xx, yy);

                            //Calculate the position of the tile in the tileset
                            int tilesetWidth, tilesetHeight;
                            SDL_QueryTexture(tls.texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
                            int tsxx = gid % (tilesetWidth / tileWidth) - 1;
                            tsxx *= tileWidth;
                            tsxx = (tsxx < 0) ? 15 * tileWidth : tsxx;

                            int tsyy = 0;
                            int amount = (gid / (tilesetWidth / tileWidth));
                            tsyy = tileHeight * amount;
                            Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

                            //Build the tile and add to level's tile list
                            Tile tile(tls.texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
                            this->_tileList.push_back(tile);
                            tileCounter++;

                            pTile = pTile->NextSiblingElement("tile");
                        }
                    }

                    pData = pData->NextSiblingElement("data");
                }
            }
            pLayer = pLayer->NextSiblingElement("layer");
        }
    }

    //Parse collisions defined in tmx file
    XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
    if(pObjectGroup != NULL)
    {
        while(pObjectGroup)
        {
            const char* name = pObjectGroup->Attribute("name");
            std::stringstream ss;
            ss << name;
            if(ss.str() == "collisions")
            {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if(pObject != NULL)
                {
                    while(pObject)
                    {
                        float x, y, width, height;
                        x = pObject->FloatAttribute("x");
                        y = pObject->FloatAttribute("y");
                        width = pObject->FloatAttribute("width");
                        height = pObject->FloatAttribute("height");
                        const char* name = pObject->Attribute("name");
                        std::stringstream ss;
                        ss << name;

                        if(ss.str() == "floor")
                        {
                            this->_floors.push_back( Rectangle(
                                                std::ceil(x) * globals::SPRITE_SCALE,
                                                std::ceil(y) * globals::SPRITE_SCALE,
                                                std::ceil(width) * globals::SPRITE_SCALE,
                                                std::ceil(height) * globals::SPRITE_SCALE));
                        }
                        else if(ss.str() == "wall")
                        {
                            this->_walls.push_back( Rectangle(
                                                std::ceil(x) * globals::SPRITE_SCALE,
                                                std::ceil(y) * globals::SPRITE_SCALE,
                                                std::ceil(width) * globals::SPRITE_SCALE,
                                                std::ceil(height) * globals::SPRITE_SCALE));
                        }
                        else if(ss.str() == "ceiling")
                        {
                            this->_ceilings.push_back( Rectangle(
                                                std::ceil(x) * globals::SPRITE_SCALE,
                                                std::ceil(y) * globals::SPRITE_SCALE,
                                                std::ceil(width) * globals::SPRITE_SCALE,
                                                std::ceil(height) * globals::SPRITE_SCALE));
                        }


                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }

            pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
        }
    }
}

void Level::update(Graphics &graphics, int elapsedTime)
{
    if(this->_mapName != "blankScreen")
    {
        if(this->_doorOpenTime <= 0)
        {
            std::string curAnimation = this->_doors.at(0).getCurrentAnimation();
            if(curAnimation == "Closed")
            {
                for(int i = 0; i < this->_doors.size(); ++i)
                {
                    this->_doors.at(i).playAnimation("Opening", true);
                }
            }
            else if(curAnimation == "Opening" && this->_doors.at(0).getFrameIndex() == 3)
            {
                for(int i = 0; i < this->_doors.size(); ++i)
                {
                    this->_doors.at(i).playAnimation("Opened");
                }
            }
        }
        else
        {
            this->_doorOpenTime -= elapsedTime;
        }
    }

    for(int i = 0; i < this->_doors.size(); ++i)
    {
        this->_doors.at(i).update(elapsedTime);
    }

    this->_activeFloors.clear();

    //Update enemies first
    for(int i = 0; i < this->_enemies.size(); ++i)
    {
        Enemy* curEnemy = this->_enemies.at(i);
        curEnemy->update(elapsedTime);

        this->_activeFloors[curEnemy->getBoundingBox().getBottom()] = true;

        Projectile* projectile = curEnemy->fireProjectile(graphics, elapsedTime);
        //fireProjectile will return nullptr if not enough time has passed since the enemy's last fire
        if(projectile != nullptr)
        {
            this->addEnemyProjectile(projectile);
        }

        bool stopAttacking = true;
        int enemyRight = curEnemy->getBoundingBox().getRight();
        int enemyLeft = curEnemy->getBoundingBox().getLeft();
        int enemyBottom = curEnemy->getBoundingBox().getBottom();

        //Loop over towers to check if there's a tower directly in front of enemy
        for(int j = 0; j < this->_towers.size(); ++j)
        {
            if(this->_towers.at(j) != nullptr)
            {
                int left = this->_towers.at(j)->getBoundingBox().getLeft();
                int right = this->_towers.at(j)->getBoundingBox().getRight();
                int bottom = this->_towers.at(j)->getBoundingBox().getBottom();

                if(!this->_towers.at(j)->getDragged() && enemyRight >= left && enemyRight < right && enemyBottom == bottom)
                {
                    //Found a tower directly in front of enemy
                    stopAttacking = false;

                    //Handle enemy if it is already attacking the tower
                    if(curEnemy->getCurrentAnimation() == "AttackRight")
                    {
                        curEnemy->attack(this->_towers.at(j));
                    }
                    else
                    {
                        //If enemy is not already attacking the tower, change enemy's animation.
                        //If enemy's sprite clipped into tower's, move enemy back so that the hitboxes
                        //are side-by-side.
                        curEnemy->setX( curEnemy->getX() - (enemyRight - left) );
                        curEnemy->setSpeed(0, 0);
                        curEnemy->playAnimation("AttackRight");
                    }
                }
            }
        }

        //Loop over walls to check if there's a wall directly in front of enemy
        for(int j = 0; j < this->_walls.size(); ++j)
        {
            int left = this->_walls.at(j).getLeft();
            int right = this->_walls.at(j).getRight();

            if(enemyRight >= left && enemyLeft < right)
            {
                stopAttacking = false;

                //Handle enemy if it is already attacking the wall
                if(curEnemy->getCurrentAnimation() == "AttackRight")
                {
                    curEnemy->handleWallCollision(*this);
                }
                else
                {
                    //If enemy is not already attacking the wall, change enemy's animation.
                    //If enemy's sprite clipped into tower's, move enemy back so that the hitboxes
                    //are side-by-side.
                    curEnemy->setX( curEnemy->getX() - (enemyRight - left) );
                    curEnemy->setSpeed(0, 0);
                    curEnemy->playAnimation("AttackRight");
                }
            }
        }

        //If enemy is not in front of a tower or wall, resume walking or firing projectile
        if(stopAttacking && curEnemy->getCurrentAnimation() != "Fire")
        {
            this->_enemies.at(i)->setSpeed(0.04, 0);
            this->_enemies.at(i)->playAnimation("WalkRight");
        }

        //Loop over projectiles to handle any collisions
        for(int j = 0; j < this->_projectiles.size(); ++j)
        {
            Projectile* curProjectile = this->_projectiles.at(j);

            if(curProjectile->getBoundingBox().collidesWith(curEnemy->getBoundingBox()) )
            {
                curEnemy->handleProjectileCollision( curProjectile );
                curProjectile->handleEnemyCollision( curEnemy );
            }
        }

        //If enemy runs out of HP, defer destroying it until next frame
        if(this->_enemies.at(i)->shallBeDestroyed())
        {
            delete this->_enemies.at(i);
            this->_enemies.at(i) = nullptr;
            this->_enemies.erase(this->_enemies.begin() + i);
            --i;
        }
        else if(curEnemy->getCurrentHealth() <= 0)
        {
            curEnemy->setDestroy(true);
            this->increaseMoney( curEnemy->getValue() );
        }
    }

    //Update towers, handle dragging and dropping, delete if health is zero
    for(int i = 0; i < this->_towers.size(); ++i)
    {
        if(this->_towers.at(i) != nullptr)
        {
            Tower* curTower = this->_towers.at(i);
            curTower->update(elapsedTime, graphics, &this->_enemies);

            //If tower is being dragged, update position based on mouse position
            if(curTower->getDragged())
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                curTower->setX( mouseX - graphics.getCameraOffsetX() - 16 );
                curTower->setY( mouseY - graphics.getCameraOffsetY() - 16 );

                curTower->setPositionValid(false);

                //Prevent tower from going below floor
                //This will create problems when there's more than one floor
                for(int j = 0; j < this->_floors.size(); ++j)
                {
                    //Add 32px padding above and on sides of floor
                    Rectangle floorHitBox(  this->_floors.at(j).getLeft(),
                                            this->_floors.at(j).getTop() - 32,
                                            this->_floors.at(j).getWidth(),
                                            this->_floors.at(j).getHeight() + 32);

                    Rectangle mouseHitBox( mouseX - graphics.getCameraOffsetX(), mouseY - graphics.getCameraOffsetY(), 0, 0);

                    if(mouseHitBox.collidesWith(floorHitBox))
                    {
                        //Place tower on top of floor
                        curTower->setX( ((mouseX - graphics.getCameraOffsetX()) / 32) * 32);
                        curTower->setY( this->_floors.at(j).getTop() - 32 );

                        //If tower can be built on floors, mark current position as valid
                        //Towers can only be built on either floors or walls; no tower can
                        //be built on both.
                        if(!curTower->canPlaceOnWall())
                        {
                            curTower->setPositionValid(true);
                        }
                    }
                }

                //Prevent tower from going inside walls
                for(int j = 0; j < this->_walls.size(); ++j)
                {
                    //Add 32px padding on sides of wall
                    Rectangle wallHitBox(  this->_walls.at(j).getLeft() - 32,
                                            this->_walls.at(j).getTop(),
                                            this->_walls.at(j).getWidth() +  64,
                                            this->_walls.at(j).getHeight());

                    Rectangle mouseHitBox( mouseX - graphics.getCameraOffsetX(), mouseY - graphics.getCameraOffsetY() , 0, 0);

                    if(mouseHitBox.collidesWith(wallHitBox))
                    {
                        curTower->setY( ((mouseY - graphics.getCameraOffsetY()) / 32) * 32);
                        //Place tower next to wall
                        //If wall is left boundary, put on right side. If wall is right boundary, left side. Otherwise, getCollisionSide
                        if(this->_walls.at(j).getLeft() == 0)
                        {
                            curTower->setX( this->_walls.at(j).getRight());
                            curTower->setPositionValid(false);
                        }
                        else if(this->_walls.at(j).getRight() == this->_size.x * 16 * globals::SPRITE_SCALE)
                        {
                            curTower->setX( this->_walls.at(j).getLeft() - 32);

                            if(curTower->canPlaceOnWall())
                            {
                                curTower->setPositionValid(true);
                            }
                        }
                    }
                }

                //Prevent tower from going inside ceilings
                for(int j = 0; j < this->_ceilings.size(); ++j)
                {
                    //Add 32px padding on sides of wall
                    Rectangle ceilingHitBox(    this->_ceilings.at(j).getLeft(),
                                                this->_ceilings.at(j).getTop(),
                                                this->_ceilings.at(j).getWidth(),
                                                this->_ceilings.at(j).getHeight() + 32);

                    Rectangle mouseHitBox( mouseX - graphics.getCameraOffsetX(), mouseY - graphics.getCameraOffsetY() , 0, 0);

                    if(mouseHitBox.collidesWith(ceilingHitBox))
                    {
                        //Place tower underneath ceiling
                        curTower->setX( ((mouseX - graphics.getCameraOffsetX()) / 32) * 32);
                        curTower->setY( this->_ceilings.at(j).getBottom());

                        curTower->setPositionValid(false);
                    }
                }
            }
            else if(!curTower->getDragged())
            {
                //Fire projectile if there is an enemy in the tower's line of sight.
                //If an enemy is on the same plane as the tower, then the enemy sprite's
                //bottom will be equal to the tower's bottom.
                if(this->_activeFloors[curTower->getBoundingBox().getBottom()] == true)
                {
                    Projectile* projectile = curTower->fireProjectile(graphics, elapsedTime);

                    //fireProjectile will return nullptr if not enough time has passed since the tower's last fire
                    if(projectile != nullptr)
                    {
                        this->addProjectile(projectile);
                    }
                }

            }

            //Loop over enemy projectiles to handle collisions
            for(int j = 0; j < this->_enemyProjectiles.size(); ++j)
            {
                Projectile* curEnmyProj = this->_enemyProjectiles.at(j);

                if(!curTower->getDragged() && curTower->getBoundingBox().collidesWith( curEnmyProj->getBoundingBox() ))
                {
                    curEnmyProj->handleTowerCollision(curTower);
                }
            }

            //If tower runs out of HP, defer destroying it until next frame
            if(this->_towers.at(i)->shallBeDestroyed())
            {
                delete this->_towers.at(i);
                this->_towers.at(i) = nullptr;
            }
            else if(curTower->getCurrentHealth() <= 0)
            {
                curTower->setDestroy(true);
            }
        }
    }

    //Loop over projectiles once more to delete them if necessary.
    //This has to have it's own separate O(n) loop or else the projectiles
    //won't update, and they will all freeze in place
    for(int i = 0; i < this->_projectiles.size(); ++i)
    {
        Projectile* curProjectile = this->_projectiles.at(i);
        curProjectile->update(elapsedTime, &this->_enemies);

        if(curProjectile->shallBeDestroyed())
        {
            delete this->_projectiles.at(i);
            _projectiles.at(i) = nullptr;
            this->_projectiles.erase(this->_projectiles.begin() + i);
            curProjectile = nullptr;
            --i;
        }
    }

    for(int i = 0; i < this->_enemyProjectiles.size(); ++i)
    {
        Projectile* curEnmyProj = this->_enemyProjectiles.at(i);
        curEnmyProj->update(elapsedTime, &this->_enemies);

        int projCenter = curEnmyProj->getBoundingBox().getCenterX();
         //Loop over walls to check if there's a wall directly in front of enemy
        for(int j = 0; j < this->_walls.size(); ++j)
        {
            int left = this->_walls.at(j).getLeft();
            int right = this->_walls.at(j).getRight();

            if(projCenter >= left && projCenter < right)
            {
                curEnmyProj->handleWallCollision(*this);
                curEnmyProj->setDestroy(true);
            }
        }

        if(curEnmyProj->shallBeDestroyed())
        {
            delete this->_enemyProjectiles.at(i);
            _enemyProjectiles.at(i) = nullptr;
            this->_enemyProjectiles.erase(this->_enemyProjectiles.begin() + i);
            curEnmyProj = nullptr;
            --i;
        }
    }
}

void Level::draw(Graphics& graphics)
{
    for(int i = 0; i < this->_tileList.size(); ++i)
    {
        this->_tileList.at(i).draw(graphics);
    }

    for(int i = 0; i < this->_enemies.size(); ++i)
    {
        this->_enemies.at(i)->draw(graphics);
    }

    for(int i = 0; i < this->_projectiles.size(); ++i)
    {
        this->_projectiles.at(i)->draw(graphics);
    }

    for(int i = 0; i < this->_towers.size(); ++i)
    {
        if(this->_towers.at(i) != nullptr)
        {
            this->_towers.at(i)->draw(graphics);
        }
    }

    for(int i = 0; i < this->_enemyProjectiles.size(); ++i)
    {
        this->_enemyProjectiles.at(i)->draw(graphics);
    }

    for(int i = 0; i < this->_doors.size(); ++i)
    {
        this->_doors.at(i).draw(graphics, this->_doors.at(i).getX(), this->_doors.at(i).getY());
    }

}

void Level::addTower(Tower *tower)
{
    this->_towers.push_back(tower);
}

void Level::addProjectile(Projectile *projectile)
{
    this->_projectiles.push_back(projectile);
}

void Level::addEnemy(Enemy *enemy)
{
    this->_enemies.push_back(enemy);
}

void Level::addEnemyProjectile(Projectile *projectile)
{
    this->_enemyProjectiles.push_back(projectile);
}

void Level::deleteTower(int index)
{
    if(index >= 0 && index < this->_towers.size())
    {
        delete this->_towers.at(index);
        this->_towers.erase( this->_towers.begin() + index);
    }
}

Tower** Level::getTowerAtMouse(int mouseX, int mouseY, int* index)
{
    *index = -1;
    for(int i = 0; i < this->_towers.size(); ++i)
    {
        if(this->_towers.at(i) != nullptr && this->_towers.at(i)->pointCollides(mouseX, mouseY))
        {
            *index = i;
            return &this->_towers.at(i);
        }
    }

    return nullptr;
}

const std::vector<Rectangle> Level::getFloors() const
{
    return this->_floors;
}

const Vector2 Level::getSize() const
{
    return this->_size;
}

const int Level::getPlayerMoney() const
{
    return this->_money;
}

void Level::reduceMoney(int price)
{
    this->_money -= price;
}

void Level::increaseMoney(int value)
{
    this->_money += value;
}

const int Level::getPlayerHealth() const
{
    return this->_health;
}

void Level::reduceHealth(int dmg)
{
    this->_health -= dmg;

    if(this->_health < 0)
    {
        this->_health = 0;
    }
}

bool Level::hasEnemies()
{
    return (this->_enemies.size() > 0);
}

void Level::freeMemory()
{
    for(int i = 0; i < this->_enemies.size(); ++i)
    {
        delete this->_enemies.at(i);
        this->_enemies.at(i) = nullptr;
    }
    this->_enemies.clear();

    for(int i = 0; i < this->_towers.size(); ++i)
    {
        if(this->_towers.at(i) != nullptr)
        {
            delete this->_towers.at(i);
            this->_towers.at(i) = nullptr;
        }
    }
    this->_towers.clear();

    for(int i = 0; i < this->_projectiles.size(); ++i)
    {
        delete this->_projectiles.at(i);
        this->_projectiles.at(i) = nullptr;
    }
    this->_projectiles.clear();

    for(int i = 0; i < this->_enemyProjectiles.size(); ++i)
    {
        delete this->_enemyProjectiles.at(i);
        this->_enemyProjectiles.at(i) = nullptr;
    }
    this->_enemyProjectiles.clear();
}
