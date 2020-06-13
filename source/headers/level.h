#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "tower.h"

#include "projectile.h"

class Graphics;
class Enemy;
class Player;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level
{
    public:
        Level();
        Level(std::string mapName, Graphics& graphics);
        ~Level();
        void update(Graphics &graphics, int elapsedTime);
        void draw(Graphics& graphics);
        inline SDL_Texture* getBackgroundTexture() const
        {
            return this->_backgroundTexture;
        }

        void addTower(Tower *tower);
        void addProjectile(Projectile *projectile);
        void addEnemy(Enemy *enemy);

        std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);

        /*  deleteTower erases the tower at index index from _towers, and
        *   frees the tower from memory
        */
        void deleteTower(int index);

        /*  getTowerAtMouse returns a pointer to a tower object located at
        *   mouseX, mouseY. The index of the tower in _towers is stored in
        *   &index. If there is no tower at that coordinate, nullptr is
        *   returned. As of now it shouldn't be possible for more than one
        *   tower to overlap, but if it comes to that, this function should
        *   return a vector<Tower*>.
        */
        Tower* getTowerAtMouse(int mouseX, int mouseY, int* index);

        const Vector2 getEnemySpawnPoint() const;

    private:
        //Don't really need all of these for a menu. Make another base class that both this and menu derive from? Perhaps.
        std::string _mapName;
        Vector2 _enemySpawnPoint;
        Vector2 _size;
        Vector2 _tileSize;
        SDL_Texture* _backgroundTexture;

        std::vector<Tile> _tileList;
        std::vector<Tileset> _tilesets;
        std::vector<Enemy*> _enemies; //Potential memory leak!
        std::vector<Tower*> _towers;
        std::vector<Projectile*> _projectiles;

        std::vector<Rectangle> _floors;
        std::vector<Rectangle> _walls;

        void loadMap(std::string mapName, Graphics &graphics);
};

//Tileset structure
struct Tileset
{
    SDL_Texture* texture;
    int firstGid;

    Tileset()
    {
        this->firstGid = -1;
    }

    Tileset(SDL_Texture* texture, int firstGid)
    {
        this->texture = texture;
        this->firstGid = firstGid;
    }
};

#endif // LEVEL_H
