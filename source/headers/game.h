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
        void menuLoop(Graphics &graphics);
        void gameLoop(Graphics &graphics);

    private:
        void draw(Graphics &graphics);
        void update(Graphics &graphics, float elapsedTime, Input &input);

        Level _level;
        Graphics _graphics;
        Menu _menu;

        int _money;
        int _health;
};

#endif // GAME_H
