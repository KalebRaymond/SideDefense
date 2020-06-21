#include <SDL.h>
#include <SDL_ttf.h>
#include "game.h"
#include "input.h"
#include "cursor.h"
#include "tower.h"
#include "enemy.h"
#include <iostream>
#include <string>
#include <math.h>

namespace
{
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    this->_selectedTower = nullptr;
    this->_towerAtMouse = nullptr;
}

Game::~Game()
{
    SDL_Quit();
    TTF_Quit();
}

void Game::runGame()
{
    Graphics graphics;

    if(mainMenuLoop(graphics) < 0) { return; }

    std::vector< std::pair< int, Enemy* > > enemySpawnStack; //Enemies are spawned starting from end of vector

    //Normally I'd maintain all these enemy spawns in an external file, but this works for proof of concept purposes.
    /// Level 1
    do
    {
        //Final wave
        enemySpawnStack.emplace_back( 500, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 3000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 12000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );


        //Wave 2
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 3000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 10000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );

        //Wave 1
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 8000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 3000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 7000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
    } while (gameLoop(graphics, enemySpawnStack, "level1") < 0);

    /// Level 2
    do
    {
        //Final wave
        enemySpawnStack.emplace_back( 200, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 200, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 200, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 200, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 200, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 200, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 200, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 200, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 10000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );

        //Wave 3
        enemySpawnStack.emplace_back( 4000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 4000, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 10000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );

        //Wave 2
        enemySpawnStack.emplace_back( 1000, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 6000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 8000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );

        //Wave 1
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 4000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
    } while (gameLoop(graphics, enemySpawnStack, "level2") < 0);

    /// Level 3
    do
    {
        //Final wave
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 6000, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 600, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 600, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 3000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 12000, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );

        //Wave 4
        enemySpawnStack.emplace_back( 0, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 6000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 2000, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 4000, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 6000, new FireEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );

        //Wave 3
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 500, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 500, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 7000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 2000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 700, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 1500, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 300, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 7000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 1000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 10000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );

        //Wave 2
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 8000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 8000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 8000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 8000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 10000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );

        //Wave 1
        enemySpawnStack.emplace_back( 5000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 500, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 0, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 0, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 320) );
        enemySpawnStack.emplace_back( 10000, new ToughEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 1000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
        enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, globals::GAME_VIEWPORT_H - 96) );
    } while (gameLoop(graphics, enemySpawnStack, "level3") < 0);

    //You win!
    if(this->winGameLoop(graphics) < 0) { return; }
}

int Game::mainMenuLoop(Graphics &graphics)
{
    Input input;
    SDL_Event event;
    Cursor cursor = Cursor(graphics);
    Sprite titleCard = Sprite(graphics, "content/sprites/titleCard.png", 0, 0, 215, 68, 100, 70);

    this->_level = Level("blankScreen", graphics);

    int LAST_UPDATE_TIME = SDL_GetTicks();
    while(true)
    {
        input.beginNewFrame();

        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.repeat == 0)
                {
                    input.keyDownEvent(event);
                }
            }
            else if(event.type == SDL_QUIT)
            {
                return -1;
            }
        }

        if(input.wasKeyPressed(SDL_SCANCODE_SPACE))
        {
            if(cursor.getPosition() == 300)
            {
                this->_level.freeMemory();
                return 1;
            }
        }
        else if(input.wasKeyPressed(SDL_SCANCODE_W))
        {
            if(cursor.getPosition() != START)
            {
                cursor.setPosition(START);
            }
        }
        else if(input.wasKeyPressed(SDL_SCANCODE_S))
        {
            if(cursor.getPosition() != SETTINGS)
            {
                cursor.setPosition(SETTINGS);
            }
        }
        else if(input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            return -1;
        }

        const int CURRENT_TIME = SDL_GetTicks();
        int ELAPSED_TIME = CURRENT_TIME - LAST_UPDATE_TIME;

        this->update(graphics, std::min( ELAPSED_TIME, MAX_FRAME_TIME ), input);

        LAST_UPDATE_TIME = CURRENT_TIME;

        graphics.clearScreen();

        this->_level.draw(graphics);
        cursor.draw(graphics);
        titleCard.draw(graphics);

        char* menuStartText = "Start";
        graphics.renderText(menuStartText, 22, 300, 294, 0, 0, 0);

        char* menuSettingsText = "Settings";
        graphics.renderText(menuSettingsText,  22, 300, 324, 0, 0, 0);

        graphics.flipScreen();
    }

    this->_level.freeMemory();

    return -1;
}

int Game::startLevelTransition(Graphics &graphics)
{
    const int START_TIME = SDL_GetTicks();
    int curTime = START_TIME;
    //Pause for half a second
    while(curTime - START_TIME < 500)
    {
        this->draw(graphics);
        curTime = SDL_GetTicks();
    }

    float cameraX = 0.0;
    float maxScrollX = -1 * (float)graphics.getMaxScroll().x;
    SDL_Event e;
    while(true)
    {
        SDL_PollEvent(&e);
        if(e.type == SDL_QUIT)
        {
            return -1;
        }

        if(abs(maxScrollX - cameraX) <= 3)
        {
            graphics.setCameraOffset( maxScrollX, 0 );
            this->draw(graphics);
            return 1;
        }

        //graphics.moveScreen will prevent the cameraOffset from exceeding maxScrollX.
        graphics.moveScreen(-3, 0);
        cameraX = graphics.getCameraOffsetX();
        this->draw(graphics);
    }

    return 0;
}

int Game::loseLevelTransition(Graphics &graphics)
{
    this->_level.draw(graphics);
    graphics.renderText("LEVEL OVER", 20, 100, 100, 0, 0, 0);
    graphics.flipScreen();

    //Wait 1.5 seconds ...for dramatic effect
    const int START_TIME = SDL_GetTicks();
    int currentTime = START_TIME;
    while(currentTime - START_TIME < 1500)
    {
        currentTime = SDL_GetTicks();
    }

    return 0;
}

int Game::gameLoop(Graphics &graphics, std::vector< std::pair< int, Enemy* > > &enemySpawnStack, std::string levelName)
{
    Input input;
    SDL_Event event;

    this->_menu = Menu(graphics);

    int lastSpawnElapsedTime = 0;

    this->_level = Level(levelName, graphics);
    graphics.setCameraOffset(0, 0);
    graphics.setMaxScroll(this->_level.getSize().x * 16 * globals::SPRITE_SCALE - globals::GAME_VIEWPORT_W, 0);

    //Play intro transition (camera sweeping from left to right)
    if(graphics.getMaxScroll().x != 0)
    {
        if(this->startLevelTransition(graphics) == -1) { return -1; }
    }

    int LAST_UPDATE_TIME = SDL_GetTicks();
    //Game loop start
    while(true)
    {
        input.beginNewFrame();
        //Reset mouse button release flags
        input.setLeftReleased(false);
        input.setRightReleased(false);

        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.repeat == 0)
                {
                    input.keyDownEvent(event);
                }
            }
            else if(event.type == SDL_KEYUP)
            {
                input.keyUpEvent(event);
            }
            else if(event.type == SDL_QUIT)
            {
                return -1;
            }
            else if(event.button.type == SDL_MOUSEBUTTONDOWN)
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    input.setLeftClick(true);
                }

                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    input.setRightClick(true);
                }
            }
            else if(event.button.type == SDL_MOUSEBUTTONUP)
            {
                if(input.getLeftClick())
                {
                    input.setLeftClick(false);
                    //_leftReleased will only be true for this frame because it gets set to
                    //false at the beginning of this while loop
                    input.setLeftReleased(true);
                }

                //Same for right click
                if(input.getRightClick())
                {
                    input.setRightClick(false);
                    input.setRightReleased(true);
                }
            }
        }

        if(input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            for(int i = 0; i < enemySpawnStack.size(); ++i)
            {
                delete enemySpawnStack.at(i).second;
            }
            enemySpawnStack.clear();

            this->_level.freeMemory();
            this->_selectedTower = nullptr;

            return -1;
        }
        else if(input.isKeyHeld(SDL_SCANCODE_A))
        {
            graphics.moveScreen(2, 0);
        }
        else if(input.isKeyHeld(SDL_SCANCODE_D))
        {
            graphics.moveScreen(-2, 0);
        }
        else if(input.isKeyHeld(SDL_SCANCODE_X))
        {
            break;
        }

        const int CURRENT_TIME = SDL_GetTicks();
        int ELAPSED_TIME = CURRENT_TIME - LAST_UPDATE_TIME;

        //Summon enemy
        if(enemySpawnStack.size() > 0 && lastSpawnElapsedTime > enemySpawnStack.back().first)
        {
            this->_level.addEnemy( enemySpawnStack.back().second );
            enemySpawnStack.pop_back();
            lastSpawnElapsedTime = 0;
        }
        else
        {
            lastSpawnElapsedTime += ELAPSED_TIME;
        }

        this->update(graphics, std::min( ELAPSED_TIME, MAX_FRAME_TIME ), input);
        LAST_UPDATE_TIME = CURRENT_TIME;

        if(enemySpawnStack.size() == 0 && !this->_level.hasEnemies())
        {
            //Level complete
            break;
        }
        if(this->_level.getPlayerHealth() == 0)
        {
            this->loseLevelTransition(graphics);

            for(int i = 0; i < enemySpawnStack.size(); ++i)
            {
                delete enemySpawnStack.at(i).second;
            }
            enemySpawnStack.clear();

            this->_level.freeMemory();
            this->_selectedTower = nullptr;

            return -1;
        }

        this->draw(graphics);
    }

    //If program makes it here, the level was won
    this->_selectedTower = nullptr;
    this->_level.freeMemory();

    this->_level.draw(graphics);
    graphics.renderText("LEVEL COMPLETE", 20, 100, 100, 0, 0, 0);
    graphics.flipScreen();

    //Wait 1.5 seconds ...for dramatic effect
    const int START_TIME = SDL_GetTicks();
    int currentTime = START_TIME;
    while(currentTime - START_TIME < 1500)
    {
        currentTime = SDL_GetTicks();
    }

    return 1;
}

int Game::winGameLoop(Graphics &graphics)
{
    graphics.setCameraOffset(0, 0);
    Input input;
    SDL_Event event;

    this->_level = Level("blankScreen", graphics);

    int LAST_UPDATE_TIME = SDL_GetTicks();
    while(true)
    {
        input.beginNewFrame();

        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.repeat == 0)
                {
                    input.keyDownEvent(event);
                }
            }
            else if(event.type == SDL_QUIT)
            {
                return -1;
            }
        }

        if(input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            return -1;
        }

        const int CURRENT_TIME = SDL_GetTicks();
        int ELAPSED_TIME = CURRENT_TIME - LAST_UPDATE_TIME;

        this->update(graphics, std::min( ELAPSED_TIME, MAX_FRAME_TIME ), input);

        LAST_UPDATE_TIME = CURRENT_TIME;

        graphics.clearScreen();

        this->_level.draw(graphics);

        char* winText = "You win!";
        graphics.renderText(winText, 22, 275, 280, 0, 0, 0);

        graphics.flipScreen();
    }

    this->_level.freeMemory();

    return -1;
}

Level* Game::getCurrentLevel()
{
    return &this->_level;
}

Tower** Game::getSelectedTower()
{
    return this->_selectedTower;
}

void Game::nullifySelectedTower()
{
    this->_selectedTower = nullptr;
}

void Game::draw(Graphics &graphics)
{
    graphics.clearScreen();

    this->_menu.draw(graphics);
    this->_level.draw(graphics);

    //Draw _health and _money to menu
    std::string strHp = std::to_string(this->_level.getPlayerHealth());
    graphics.renderText(strHp.c_str(), 20, 95, globals::GAME_VIEWPORT_H + 8, 255, 255, 255);

    std::string strMoney = std::to_string(this->_level.getPlayerMoney());
    graphics.renderText(strMoney.c_str(), 20, 95, globals::GAME_VIEWPORT_H + 25, 255, 255, 255);

    //If the upgrades menu is open, also draw hp and trade-in value of tower
    if(this->_selectedTower != nullptr)
    {
        std::string strHp = std::to_string( (*this->_selectedTower)->getCurrentHealth());
        graphics.renderText(strHp.c_str(), 20, 363, globals::GAME_VIEWPORT_H + 8, 255, 255, 255);

        std::string strMoney = std::to_string( (int)(ceil( (*this->_selectedTower)->getPrice() * 0.8 ) ) );
        graphics.renderText(strMoney.c_str(), 20, 363, globals::GAME_VIEWPORT_H + 25, 255, 255, 255);
    }

    graphics.flipScreen();
}

void Game::update(Graphics &graphics, int elapsedTime, Input &input)
{
    if(this->_selectedTower != nullptr)
    {
        if((*this->_selectedTower)->shallBeDestroyed())
        {
            //Tower will be destroyed on next frame, deselect to avoid crashing
            this->_selectedTower = nullptr;
            this->_menu.setState(DEFAULT);
            this->_menu.freeUpgradeItems();
        }
        else
        {
            this->_menu.setState(UPGRADE);
            std::pair< TowerMenuItem*, TowerMenuItem* > items = (*this->_selectedTower)->getMenuItems(graphics);
            this->_menu.setUpgradeMenuItems( items.first, items.second );
        }
    }
    else
    {
        if(this->_menu.getState() == UPGRADE)
        {
            this->_menu.freeUpgradeItems();
        }
        this->_menu.setState(DEFAULT);
    }
    this->_menu.update(elapsedTime, input, *this);
    this->_level.update(graphics, elapsedTime);

    //Get mouse location
    int mouseX, mouseY;
    int gameMouseX, gameMouseY; //Mouse location corrections for use inside game viewport
    SDL_GetMouseState(&mouseX, &mouseY);
    gameMouseX = mouseX;
    gameMouseY = mouseY;
    if(gameMouseX <= 32)
    {
        //If mouse goes inside left wall, curTower will return nullptr because the tower will
        //be drawn next to the wall (where the mouse is not)
        gameMouseX += 32;
    }
    if(gameMouseX >= globals::GAME_VIEWPORT_W - 32)
    {
        //Correct mouseX for right wall
        gameMouseX -= 32;
    }
    if(gameMouseY >= globals::GAME_VIEWPORT_H - 32)
    {
        //Correct mouseY for floor
        gameMouseY -= 32;
    }

    //Check mouse collisions with towers
    int towerIndex = 0;
    this->_towerAtMouse = this->_level.getTowerAtMouse(gameMouseX - graphics.getCameraOffsetX(), gameMouseY - graphics.getCameraOffsetY(), &towerIndex);
    //getTowerAtMouse either returns an address or nullptr. If _towerAtMouse is not nullptr, it is pointing to a tower (hopefully)
    if(this->_towerAtMouse != nullptr)
    {
        if(input.getLeftReleased() && !(*this->_towerAtMouse)->getDragged())
        {
            //If tower is not currently being dragged, deselect currently
            //selected tower and select the clicked tower instead
            if(this->_selectedTower != nullptr)
            {
                (*this->_selectedTower)->setSelected(false);
            }
            (*this->_towerAtMouse)->setSelected(true);
            this->_selectedTower = this->_towerAtMouse;

        }
        else if(input.getLeftReleased() && (*this->_towerAtMouse)->getDragged() && (*this->_towerAtMouse)->isPositionValid())
        {
            //Place tower down
            (*this->_towerAtMouse)->setDragged(false);
            (*this->_towerAtMouse)->setPlaced(true);

            int closestFloor = 448;
            int minDistToFloor = globals::GAME_VIEWPORT_H;
            std::vector<Rectangle> floors = this->_level.getFloors();
            for(int i = 0; i < floors.size(); ++i)
            {
                int floorY = floors.at(i).getTop();
                if(floorY - mouseY > 0 && floorY - mouseY < minDistToFloor)
                {
                    minDistToFloor = floorY - mouseY;
                    closestFloor = floorY;
                }
            }

            (*this->_towerAtMouse)->setFloor(closestFloor);

            this->_level.reduceMoney( (*this->_towerAtMouse)->getPrice() );
        }
        else if(input.getRightReleased() && (*this->_towerAtMouse)->getDragged())
        {
            //Delete tower
            this->_level.deleteTower(towerIndex);
        }
    }
    //If user clicks anywhere else in game viewport besides a tower, deselect the currently selected tower
    else if(this->_towerAtMouse == nullptr && input.getLeftReleased() && this->_selectedTower != nullptr && mouseY <= globals::GAME_VIEWPORT_H - 32)
    {
        (*this->_selectedTower)->setSelected(false);
        this->_selectedTower = nullptr;
    }

    //Retrieve tower from menu if user currently is not dragging another tower
    int mouseTowerIndex = -1;
    if(this->_level.getTowerAtMouse(mouseX - graphics.getCameraOffsetX(), mouseY - graphics.getCameraOffsetY(), &mouseTowerIndex) == nullptr)
    {
        Tower* tower = this->_menu.getTower(graphics, mouseX, mouseY, this->_level.getPlayerMoney());
        if(tower != nullptr)
        {
            //If _selectedTower is not null, then the selected tower is being upgraded.
            //Replace the selected tower with a new tower
            if(this->_selectedTower != nullptr)
            {
                tower->setX( (*this->_selectedTower)->getX() );
                tower->setY( (*this->_selectedTower)->getY() );
                tower->setDragged(false);
                tower->setPlaced(true);
                tower->setSelected(true);
                tower->setFloor( (*this->_selectedTower)->getFloor() );

                //Delete selected tower object in heap
                Tower* oldTower = *this->_selectedTower;
                delete oldTower;

                //Cast tower pointer in _level->_towers to derived class and
                //have it point to new tower object
                *this->_selectedTower = dynamic_cast<Tower*>(tower);

                this->_level.reduceMoney( (*this->_selectedTower)->getPrice() );
            }
            else
            {
                this->_level.addTower(tower);
            }
        }
    }
}
