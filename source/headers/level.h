#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>
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
        void addEnemyProjectile(Projectile *projectile);

        /*  deleteTower erases the tower at index index from _towers, and
        *   frees the tower from memory
        */
        void deleteTower(int index);

        /*  getTowerAtMouse returns the address of the Tower pointer in _towers
        *   object located at mouseX, mouseY. The index of the tower in _towers
        *   is stored in &index. If there is no tower at that coordinate, nullptr is
        *   returned and index is given the value -1.
        */
        Tower** getTowerAtMouse(int mouseX, int mouseY, int* index);

        const std::vector<Rectangle> getFloors() const;

        const Vector2 getSize() const;

        const int getPlayerMoney() const;
        void reduceMoney(int price);
        void increaseMoney(int value);
        const int getPlayerHealth() const;
        void reduceHealth(int dmg);

        /*  hasEnemies returns true if _enemies.size() > 0. This means there
        *   are currently enemies that have yet to be defeated.
        */
        bool hasEnemies();


        /*  freeMemory() frees and erases every pointer in _enemies, _towers, and
        *   _projectiles
        */
        void freeMemory();

    private:
        void loadMap(std::string mapName, Graphics &graphics);

        std::vector<Tile> _tileList;
        std::vector<Tileset> _tilesets;
        std::vector<Enemy*> _enemies;
        /*  Since _selectedTower in Game points to an element in
        *   _towers, _towers must not erase elements when they
        *   become nullptr, or else it will cause _selectedTower to
        *   point at garbage when the element gets erased and all other
        *   elements in the vector are shifted around.
        */
        std::vector<Tower*> _towers;
        std::vector<Projectile*> _projectiles;
        std::vector<Projectile*> _enemyProjectiles;

        std::vector<Rectangle> _floors;
        std::vector<Rectangle> _walls;
        std::vector<Rectangle> _ceilings;

        /*  _activeFloors contains the y-values of each floor
        *   on which there is at least one enemy. Towers that
        *   are built only on those floors will fire projectiles.
        */
        std::map<int, bool> _activeFloors;

        std::vector<AnimatedSprite> _doors;
        int _doorOpenTime;

        SDL_Texture* _backgroundTexture;

        Vector2 _size;
        Vector2 _tileSize;

        std::string _mapName;

        int _money;
        int _health;
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
