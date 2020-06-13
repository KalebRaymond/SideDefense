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
#include <iostream>

using namespace tinyxml2;

Level::Level()
{

}

Level::Level(std::string mapName, Graphics& graphics)
    :   _mapName(mapName),
        _size(Vector2(0, 0))
{
    this->loadMap(mapName, graphics);
}

Level::~Level()
{

}

void Level::loadMap(std::string mapName, Graphics& graphics)
{
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
            std::cout << source;

            int firstGid;
            char* path;
            //std::stringstream ss;
            //ss << source;
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


                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }

            //Check other object groups here
            else if(ss.str() == "spawnpoints")
            {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if(pObject != NULL)
                {
                    while(pObject)
                    {
                        float x = pObject->FloatAttribute("x");
                        float y = pObject->FloatAttribute("y");
                        const char* name = pObject->Attribute("name");
                        std::stringstream ss;
                        ss << name;
                        if(ss.str() == "enemy")
                        {
                            this->_enemySpawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
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
    /*          Goal structure
    *   Towers:         Tower - Tower
    *                   Tower - Enemy
    *                   Tower - Projectile
    *                   Tower - Tile
    *
    *   Enemies:        Enemy - Enemy
    *                   Enemy - Projectile
    *                   Enemy - Tile
    *
    *   Projectiles:    Projectile - Projectile
    *                   Projectile - Tile
    *
    *   Tiles:          Tile - Tile
    */
    //O(n^2)... I wonder if this whole update function can be optimized.
    //Is it even a bad thing for it to be O(n^2)? is it worth it to strive
    //for a better run time? Acceptable practice for a game? Google that.
    //Update towers
    for(int i = 0; i < this->_towers.size(); ++i)
    {
        this->_towers.at(i)->update(elapsedTime, graphics, &this->_enemies);

        //If tower is being dragged, update position based on mouse position
        if(this->_towers.at(i)->getDragged())
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            this->_towers.at(i)->setX( mouseX - graphics.getCameraOffsetX() - 16 );
            this->_towers.at(i)->setY( mouseY - graphics.getCameraOffsetY() - 16 );

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
                    this->_towers.at(i)->setX( ((mouseX / 32) * 32) - graphics.getCameraOffsetX());
                    this->_towers.at(i)->setY( this->_floors.at(j).getTop() - 32 );
                    this->_towers.at(i)->setPositionValid(true);
                }
                else
                {
                    this->_towers.at(i)->setPositionValid(false);
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
                    this->_towers.at(i)->setY( ((mouseY / 32) * 32) - graphics.getCameraOffsetY());
                    //Place tower next to wall
                    //If wall is left boundary, put on right side. If wall is right boundary, left side. Otherwise, getCollisionSide
                    if(this->_walls.at(j).getLeft() == 0)
                    {
                        this->_towers.at(i)->setX( this->_walls.at(j).getRight());
                    }
                    else if(this->_walls.at(j).getRight() == this->_size.x * 16 * globals::SPRITE_SCALE)
                    {
                        this->_towers.at(i)->setX( this->_walls.at(j).getLeft() - 32);
                    }
                }
            }

            //Handle mouse going outside of game viewport
            if(this->_towers.at(i)->getY() > globals::GAME_VIEWPORT_H - 64)
            {
                //this->_towers.at(i)->setY(globals::GAME_VIEWPORT_H - 96);
            }
        }
        else if(!this->_towers.at(i)->getDragged())
        {
            //Only fire bullet if an enemy is in line of sight
            int left = this->_towers.at(i)->getBoundingBox().getLeft();
            int right = this->_towers.at(i)->getBoundingBox().getRight();
            int top = this->_towers.at(i)->getBoundingBox().getTop();
            int bottom = this->_towers.at(i)->getBoundingBox().getBottom();

            //Cycle through list of enemies to get positions
            int enemyX, enemyBottom, enemyRight;
            for(int j = 0; j < this->_enemies.size(); ++j)
            {
                enemyX = this->_enemies.at(j)->getX();
                enemyBottom = this->_enemies.at(j)->getBoundingBox().getBottom();
                enemyRight = this->_enemies.at(j)->getBoundingBox().getRight();

                //Fire projectile if there is an enemy in the tower's line of sight.
                //If an enemy is on the same plane as the tower, then the enemy sprite's
                //bottom will be equal to the tower's bottom.
                if(enemyX < left && enemyBottom == bottom)
                {
                    Projectile* projectile = this->_towers.at(i)->fireProjectile(graphics, elapsedTime);

                    //fireProjectile will return nullptr if not enough time has passed since the tower's last fire
                    if(projectile != nullptr)
                    {
                        this->addProjectile(projectile);
                    }

                    //If enemy is directly in front of tower, play the enemy's attack animation.
                    if(enemyRight >= left && enemyRight < right)
                    {
                        //Handle enemy if it already attacking tower
                        if(this->_enemies.at(j)->getCurrentAnimation() == "AttackRight")
                        {
                            this->_enemies.at(j)->attack( this->_towers.at(i) );
                        }
                        else
                        {
                            //If enemy is not currently attacking tower, change enemy's animation.
                            //If enemy's sprite clipped into tower's, move enemy back so that the hitboxes
                            //are side-by-side.
                            this->_enemies.at(j)->setX( this->_enemies.at(j)->getX() - (enemyRight - left) );
                            this->_enemies.at(j)->setSpeed(0, 0);
                            this->_enemies.at(j)->playAnimation("AttackRight");
                        }
                    }

                    //Only need one enemy to initiate projectile, so this for loop can
                    //break after the projectile is created.
                    break;
                }
            }
        }

        //Destroy tower if health is zero
        if(this->_towers.at(i)->getCurrentHealth() <= 0)
        {
            delete this->_towers.at(i);
            this->_towers.at(i) = NULL;
            this->_towers.erase(this->_towers.begin() + i);
            --i;
        }
    }

    for(int i = 0; i < this->_projectiles.size(); ++i)
    {
        //Replace _projectiles.at(i) with a pointer/reference?
        this->_projectiles.at(i)->update(elapsedTime, &this->_enemies);
        //Destroy projectile if it hits the left wall. Should actually handle collisions (rockets explode)
        if(this->_projectiles.at(i)->getX() < 32)
        {
            //Make this a function like deleteTower()
            delete this->_projectiles.at(i);
            _projectiles.at(i) = 0;
            this->_projectiles.erase(this->_projectiles.begin() + i);
            --i;
        }
        else
        {
            for(int j = 0; j < this->_enemies.size(); ++j)
            {
                Rectangle enemyCenter = {   this->_enemies.at(j)->getBoundingBox().getCenterX() - 3,
                                            this->_enemies.at(j)->getBoundingBox().getTop(),
                                            6,
                                            this->_enemies.at(j)->getBoundingBox().getTop(),};

                if(this->_projectiles.at(i)->getBoundingBox().collidesWith( enemyCenter ))
                {
                    this->_enemies.at(j)->handleProjectileCollision( this->_projectiles.at(i)->getDamage() );
                    this->_projectiles.at(i)->handleEnemyCollision();

                    if(this->_projectiles.at(i)->shallBeDestroyed())
                    {
                        delete this->_projectiles.at(i);
                        this->_projectiles.at(i) = 0;
                        this->_projectiles.erase(this->_projectiles.begin() + i);
                        --i;
                    }
                }
            }
        }
    }

    //Loop over enemies
    for(int i = 0; i < this->_enemies.size(); ++i)
    {
        this->_enemies.at(i)->update(elapsedTime);

        if(this->_enemies.at(i)->getCurrentAnimation() == "AttackRight")
        {
            bool stopAttacking = true;
            //Check to make sure there's a tower directly in front of enemy
            for(int j = 0; j < this->_towers.size(); ++j)
            {
                int left = this->_towers.at(j)->getBoundingBox().getLeft();
                int right = this->_towers.at(j)->getBoundingBox().getRight();
                int bottom = this->_towers.at(j)->getBoundingBox().getBottom();

                int enemyRight = this->_enemies.at(i)->getBoundingBox().getRight();
                int enemyBottom = this->_enemies.at(i)->getBoundingBox().getBottom();

                if(enemyRight >= left && enemyRight < right && enemyBottom == bottom)
                {
                    //Found a tower directly in front of enemy
                    stopAttacking = false;
                    break;
                }
            }

            //If enemy is not in front of a tower, resume walking
            if(stopAttacking)
            {
                this->_enemies.at(i)->setSpeed(0.05, 0);
                this->_enemies.at(i)->playAnimation("WalkRight");
            }
        }

        if(this->_enemies.at(i)->getCurrentHealth() <= 0)
        {
            delete this->_enemies.at(i);
            this->_enemies.at(i) = 0;
            this->_enemies.erase(this->_enemies.begin() + i);
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
        this->_towers.at(i)->draw(graphics);
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

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle &other)
{
    std::vector<Enemy*> others;
    for(int i = 0; i < this->_enemies.size(); ++i)
    {
        if(this->_enemies.at(i)->getBoundingBox().collidesWith(other))
        {
            others.push_back(this->_enemies.at(i));
        }
    }

    return others;
}

void Level::deleteTower(int index)
{
    if(index >= 0 && index < this->_towers.size())
    {
        delete this->_towers.at(index);
        this->_towers.erase( this->_towers.begin() + index);
    }
}

Tower* Level::getTowerAtMouse(int mouseX, int mouseY, int* index)
{
    *index = -1;
    for(int i = 0; i < this->_towers.size(); ++i)
    {
        if(this->_towers.at(i)->pointCollides(mouseX, mouseY))
        {
            *index = i;
            return this->_towers.at(i);
        }
    }

    return nullptr;
}

const Vector2 Level::getEnemySpawnPoint() const
{
    return this->_enemySpawnPoint;
}

