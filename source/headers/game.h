#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "level.h"
#include "menu.h"

class Input;

class Game
{
    public:
        Game();
        ~Game();

        void runGame();

        /*  mainmenuLoop
        *   Returns 1 on selecting "Start game"
        *   Returns -1 on closing window or pressing esc key
        */
        int mainMenuLoop(Graphics &graphics);

        /*  gameLoop
        *   Returns 1 on defeating all enemies
        *   Returns -1 on closing window or pressing esc key
        */
        int gameLoop(Graphics &graphics, std::vector< std::pair< int, Enemy* > > &enemySpawnStack, std::string levelName);

        /*  winGameLoop
        *   Returns -1 on closing window or pressing esc key
        */
        int winGameLoop(Graphics &graphics);

        Level* getCurrentLevel();
        Tower** getSelectedTower();
        void nullifySelectedTower();

    private:
        void draw(Graphics &graphics);
        void update(Graphics &graphics, int elapsedTime, Input &input);

        /*  startLevelTransition() manipulates the camera to show the
        *   user the level layout before the level starts. Right now it
        *   sweeps the camera from left to right.
        *   Returns -1 if user quits program while this function is
        *   executing. Otherwise returns 1.
        */
        int startLevelTransition(Graphics &graphics);
        int loseLevelTransition(Graphics &graphics);

        Level _level;
        Graphics _graphics;
        Menu _menu;

        /*  _selectedTower and _towerAtMouse both are used to point to
        *   Tower* pointers located inside this->_level._towers.
        */
        Tower** _selectedTower;
        Tower** _towerAtMouse;
};

#endif // GAME_H
