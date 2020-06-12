#include <SDL.h>
#include <SDL_ttf.h>
#include "game.h"
#include "input.h"
#include "cursor.h"
#include "tower.h"
#include "enemy.h"
#include <iostream>

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
}

Game::~Game()
{
    TTF_Quit();
}

void Game::runGame()
{
    Graphics graphics;

    menuLoop(graphics);
    gameLoop(graphics);
}

void Game::menuLoop(Graphics &graphics)
{
    Input input;
    SDL_Event event;
    Cursor cursor = Cursor(graphics, Vector2(215, 300));

    //For some reason, it doesn't draw the red and blue tiles in levelTiles.png
    this->_level = Level("testMenu", graphics);

    int LAST_UPDATE_TIME = SDL_GetTicks(); //Maybe need this for arrow animation
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
                else if(event.type == SDL_QUIT)
                {
                    //Quitting doesn't end program, it moves on to gameLoop
                    return;
                }
            }
        }

        if(input.wasKeyPressed(SDL_SCANCODE_SPACE))
        {
            if(cursor.getPosition() == 300)
            {
                return;
            }
        }
        else if(input.wasKeyPressed(SDL_SCANCODE_W))
        {
            //Must be a less hacky way of checking which position the cursor is at. Enumeration maybe?
            if(cursor.getPosition() != 300)
            {
                cursor.setPosition(215, cursor.getPosition() - 30);
            }
        }
        else if(input.wasKeyPressed(SDL_SCANCODE_S))
        {
            if(cursor.getPosition() != 330)
            {
                cursor.setPosition(215, cursor.getPosition() + 30);
            }
        }
        else if(input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            return;
        }

        const int CURRENT_TIME = SDL_GetTicks();
        int ELAPSED_TIME = CURRENT_TIME - LAST_UPDATE_TIME;

        this->update(graphics, std::min( ELAPSED_TIME, MAX_FRAME_TIME ), input);

        LAST_UPDATE_TIME = CURRENT_TIME;

        graphics.clearScreen();

        this->_level.draw(graphics);
        cursor.draw(graphics);

        char* menuStartText = "Start";
        graphics.renderText(graphics.getSurface("testMenu"), menuStartText, 22, 300, 294, 0, 0, 0);

        char* menuSettingsText = "Settings";
        graphics.renderText(graphics.getSurface("testMenu"), menuSettingsText,  22, 300, 324, 0, 0, 0);

        graphics.flipScreen();
    }
}

void Game::gameLoop(Graphics &graphics)
{
    Input input;
    SDL_Event event;

    //bigLevel is twice as long as globals::SCREEN_WIDTH, but since the camera width
    //is globals::SCREEN_WIDTH, the camera can only move over globals::SCREEN_WIDTH
    //more pixels at most.
    //graphics.setCameraOffset( -(globals::GAME_VIEWPORT_W), 0);
    graphics.setCameraOffset( 0, 0);
    graphics.setMaxScroll(globals::GAME_VIEWPORT_W, 0);

    this->_menu = Menu(graphics);
    this->_level = Level("testLevel", graphics);
    //Replace 192 with getFloorY();
    this->_level.addEnemy(new BasicEnemy(graphics, 32, 192 * globals::SPRITE_SCALE));

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
                return;
            }
        }

        //Handle mouse events. Moving the mouse clears the SDL_MOUSEBUTTONDOWN flag for whatever reason,
        //so you have to use some tricks to keep track of both motion and button events. ATM I'm using
        //convoluted if statement structure, would probably be cleaner if I used a flag or something.

        if(event.type == SDL_MOUSEBUTTONDOWN)
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
        else //Mouse was either moved, which overwrites the event type, or a mouse button was released.
        {
            //If input->_leftClick is true but the SDL_Event did not poll a mouse click,
            //then the mouse button was released and _leftClick must be set to false
            if(input.getLeftClick())
            {
                if(event.type == SDL_MOUSEMOTION)
                {
                    //
                }
                else
                {
                    input.setLeftClick(false);
                    //_leftReleased will only be true for this frame because it gets set to
                    //false at the beginning of this while loop
                    input.setLeftReleased(true);
                }
            }

            //Same for right click
            if(input.getRightClick())
            {
                if(event.type == SDL_MOUSEMOTION)
                {
                    //
                }
                else
                {
                    input.setRightClick(false);
                    input.setRightReleased(true);
                }
            }
        }


        if(input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            return;
        }
        else if(input.isKeyHeld(SDL_SCANCODE_A))
        {
            graphics.moveScreen(1, 0);
        }
        else if(input.isKeyHeld(SDL_SCANCODE_D))
        {
            graphics.moveScreen(-1, 0);
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        const int CURRENT_TIME = SDL_GetTicks();
        int ELAPSED_TIME = CURRENT_TIME - LAST_UPDATE_TIME;

        this->_graphics = graphics;

        this->update(graphics, std::min( ELAPSED_TIME, MAX_FRAME_TIME ), input);

        LAST_UPDATE_TIME = CURRENT_TIME;

        this->draw(graphics);
    }
}

void Game::draw(Graphics &graphics)
{
    graphics.clearScreen();

    this->_menu.draw(graphics);
    this->_level.draw(graphics);

    //Draw _health and _money to menu
    std::string strHp = std::to_string(this->_health);
    graphics.renderText(graphics.getSurface("bigLevel"), strHp.c_str(), 20, 95, globals::GAME_VIEWPORT_H + 8, 255, 255, 255);

    std::string strMoney = std::to_string(this->_money);
    graphics.renderText(graphics.getSurface("bigLevel"), strMoney.c_str(), 20, 95, globals::GAME_VIEWPORT_H + 25, 255, 255, 255);

    graphics.flipScreen();
}

void Game::update(Graphics &graphics, float elapsedTime, Input &input)
{
    this->_menu.update(elapsedTime, input);
    this->_level.update(graphics, elapsedTime);

    //Check and handle mouse events with towers
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int towerIndex = 0;
    Tower* curTower = this->_level.getTowerAtMouse(mouseX - graphics.getCameraOffsetX(), mouseY - graphics.getCameraOffsetY(), &towerIndex);
    if(curTower != nullptr)
    {
        if(input.getLeftReleased() && !curTower->getDragged() && !curTower->getPlaced())
        {
            //Pick up tower (if it hasn't already been placed)
            curTower->setDragged(true);
        }
        else if(input.getLeftReleased() && curTower->getDragged())
        {
            //Place tower down
            curTower->setDragged(false);
            curTower->setPlaced(true);
            this->_money -= curTower->getPrice();
        }
        else if(input.getRightReleased() && curTower->getDragged())
        {
            //Delete tower
            this->_level.deleteTower(towerIndex);
        }
    }

    //Check and handle mouse events with menu
    Tower* tower = this->_menu.getTower(graphics, mouseX, mouseY, this->_money);
    if(tower != nullptr)
    {
        this->_level.addTower(tower);
    }
}
