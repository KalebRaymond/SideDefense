#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <memory>
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

        /*  addTower inserts the passed tower pointer into this->_towers
        *   addProjectile and addEnemy work similarly
        */
        void addTower(Tower *tower);
        void addProjectile(Projectile *projectile);
        void addEnemy(Enemy *enemy);

        /*  deleteTower erases the tower at index index from _towers, and
        *   frees the tower from memory
        */
        void deleteTower(int index);

        /*  getTowerAtMouse returns a pointer to a tower object located at
        *   mouseX, mouseY. The index of the tower in _towers is stored in
        *   &index. If there is no tower at that coordinate, nullptr is
        *   returned and index is given the value -1.
        */
        Tower* getTowerAtMouse(int mouseX, int mouseY, int* index);

        /*  getFloorY() returns the y value in pixels of the floor. It time,
        *   this will probably return a vector when there are multiple floors
        */
        const int getFloorY() const;

        /*  freeMemory() frees and erases every pointer in _enemies, _towers, and
        *   _projectiles
        */
        void freeMemory();

    private:
        std::vector<Tile> _tileList;
        std::vector<Tileset> _tilesets;
        std::vector<Enemy*> _enemies;
        std::vector<Tower*> _towers;
        std::vector<Projectile*> _projectiles;

        std::vector<Rectangle> _floors;
        std::vector<Rectangle> _walls;

        SDL_Texture* _backgroundTexture;

        Vector2 _size;
        Vector2 _tileSize;

        std::string _mapName;

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
