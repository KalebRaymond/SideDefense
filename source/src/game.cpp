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

/*  Game class
*   Put info here
*/

namespace
{
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    this->_health = 100;
    this->_money = 500;

    this->_selectedTower = nullptr;
    this->_towerAtMouse = nullptr;
}

Game::~Game()
{
    //SDL_Quit();
    TTF_Quit();
}

void Game::runGame()
{
    /* BIG MEMORY LEAK HERE */
    //Need to clear all enemies, projectiles, and enemies (and whatever else) between each level.
    Graphics graphics;

    if(mainMenuLoop(graphics) < 0) { return; }
    if(gameLoop(graphics) < 0) { return; }
}

int Game::mainMenuLoop(Graphics &graphics)
{
    Input input;
    SDL_Event event;
    Cursor cursor = Cursor(graphics);
    SDL_Texture *blankTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("content/sprites/misc.png"));
    SDL_Rect blankRectangle = {0, 1, 1, 1}; //Grab a white pixel from misc.png
    SDL_Rect menuDest = {0, globals::GAME_VIEWPORT_H, globals::MENU_VIEWPORT_W, globals::MENU_VIEWPORT_H};

    //For some reason, it doesn't draw the red and blue tiles in levelTiles.png
    this->_level = Level("testMenu", graphics);

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

        char* menuStartText = "Start";
        graphics.renderText(menuStartText, 22, 300, 294, 0, 0, 0);

        char* menuSettingsText = "Settings";
        graphics.renderText(menuSettingsText,  22, 300, 324, 0, 0, 0);

        graphics.flipScreen();
    }

    this->_level.freeMemory();

    return 1;
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

int Game::gameLoop(Graphics &graphics)
{
    Input input;
    SDL_Event event;

    this->_menu = Menu(graphics);
    this->_level = Level("bigLevel", graphics);

    int lastSpawnElapsedTime = 0;
    std::vector< std::pair< int, Enemy* > > enemySpawnStack;
    //Enemies are spawned starting from end of vector
    enemySpawnStack.emplace_back( 5000, new BasicEnemy(graphics, 32, this->_level.getFloorY() - 64) );
    enemySpawnStack.emplace_back( 5000, new ToughEnemy(graphics, 32, this->_level.getFloorY() - 64) );

    //bigLevel is twice as long as globals::GAME_VIEWPORT_W, but since the camera width
    //is globals::GAME_VIEWPORT_W, the camera can only move over globals::GAME_VIEWPORT_W
    //more pixels at most.
    //graphics.setCameraOffset( -(globals::GAME_VIEWPORT_W), 0);
    graphics.setCameraOffset( 0, 0);
    graphics.setMaxScroll(globals::GAME_VIEWPORT_W, 0);

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
        //Put this in update?
        if(enemySpawnStack.size() > 0 && lastSpawnElapsedTime > enemySpawnStack.back().first)
        {
            //Check this for memory leak. Do I need to clear pointer in enemySpawnStack if it's in _enemies?
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

        this->draw(graphics);
    }

    this->_level.freeMemory();
    //also free enemySpawnQueue...

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

void Game::draw(Graphics &graphics)
{
    graphics.clearScreen();

    this->_menu.draw(graphics);
    this->_level.draw(graphics);

    //Draw _health and _money to menu
    std::string strHp = std::to_string(this->_health);
    graphics.renderText(strHp.c_str(), 20, 95, globals::GAME_VIEWPORT_H + 8, 255, 255, 255);

    std::string strMoney = std::to_string(this->_money);
    graphics.renderText(strMoney.c_str(), 20, 95, globals::GAME_VIEWPORT_H + 25, 255, 255, 255);

    //If the upgrades menu is open, also draw hp and trade-in value of tower
    if(this->_selectedTower != nullptr)
    {
        std::string strHp = std::to_string(this->_selectedTower->getCurrentHealth());
        graphics.renderText(strHp.c_str(), 20, 363, globals::GAME_VIEWPORT_H + 8, 255, 255, 255);

        std::string strMoney = std::to_string( (int)(ceil(this->_selectedTower->getPrice() * 0.8)) );
        graphics.renderText(strMoney.c_str(), 20, 363, globals::GAME_VIEWPORT_H + 25, 255, 255, 255);
    }

    graphics.flipScreen();
}

void Game::update(Graphics &graphics, int elapsedTime, Input &input)
{
    if(this->_selectedTower != nullptr)
    {
        this->_menu.setState(UPGRADE);
        std::pair< TowerMenuItem*, TowerMenuItem* > items = this->_selectedTower->getMenuItems(graphics);
        this->_menu.setUpgradeMenuItems( items.first, items.second );
    }
    else
    {
        this->_menu.setState(DEFAULT);
        this->_menu.setUpgradeMenuItems(nullptr, nullptr);
    }
    this->_menu.update(elapsedTime, input);
    this->_level.update(graphics, elapsedTime);

    //Check and handle mouse events with towers
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

    int towerIndex = 0;
    this->_towerAtMouse = this->_level.getTowerAtMouse(gameMouseX - graphics.getCameraOffsetX(), gameMouseY - graphics.getCameraOffsetY(), &towerIndex);
    if(this->_towerAtMouse != nullptr)
    {
        if(input.getLeftReleased() && !this->_towerAtMouse->getDragged())
        {
            //If tower is not currently being dragged, deselect currently
            //selected tower and select the clicked tower instead
            if(this->_selectedTower != nullptr)
            {
                this->_selectedTower->setSelected(false);
            }
            this->_towerAtMouse->setSelected(true);
            this->_selectedTower = this->_towerAtMouse;

        }
        else if(input.getLeftReleased() && this->_towerAtMouse->getDragged() && this->_towerAtMouse->isPositionValid())
        {
            //Place tower down
            this->_towerAtMouse->setDragged(false);
            this->_towerAtMouse->setPlaced(true);
            this->_money -= this->_towerAtMouse->getPrice();
        }
        else if(input.getRightReleased() && this->_towerAtMouse->getDragged())
        {
            //Delete tower
            this->_level.deleteTower(towerIndex);
        }
    }
    //If user clicks anywhere else in game viewport besides a tower, deselect the currently selected tower
    else if(this->_towerAtMouse == nullptr && input.getLeftReleased() && this->_selectedTower != nullptr && mouseY <= globals::GAME_VIEWPORT_H - 32)
    {
        this->_selectedTower->setSelected(false);
        this->_selectedTower = nullptr;
    }

    //Check and handle mouse events with menu
    Tower* tower = this->_menu.getTower(graphics, mouseX, mouseY, this->_money);
    if(tower != nullptr)
    {
        this->_level.addTower(tower);
    }
}
