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
        *   Returns 1 on... nothing yet
        */
        int gameLoop(Graphics &graphics);

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

        Level _level;
        Graphics _graphics;
        Menu _menu;

        Tower* _selectedTower;
        Tower* _towerAtMouse;

        int _money;
        int _health;
};

#endif // GAME_H
