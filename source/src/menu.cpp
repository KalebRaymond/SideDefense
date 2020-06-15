#include "menu.h"
#include "menu_item.h"
#include "tower.h"
#include "input.h"
#include "globals.h"

Menu::Menu()
{

}

Menu::Menu(Graphics &graphics)
{
    //Populate _menuItems with MenuItem objects
    this->_menuItems.emplace_back("hp", graphics, 4, 74, 14, 8, Vector2( 32, globals::GAME_VIEWPORT_H + 10 ));
    this->_menuItems.emplace_back("cash", graphics, 4, 81, 26, 8, Vector2( 32, globals::GAME_VIEWPORT_H + 28 ));
    this->_menuItems.emplace_back("save", graphics, 3, 3, 16, 16, Vector2( 32, globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 )));
    this->_menuItems.emplace_back("music", graphics, 3, 21, 16, 16, Vector2( 32 + (40 * 1) , globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 )) );
    this->_menuItems.emplace_back("sfx", graphics, 3, 39, 16, 16, Vector2( 32 + (40 * 2), globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 )) );

    //Populate _towerMenuItems with TowerMenuItems
    this->_towerMenuItems.emplace_back("bulletTower", 100, graphics, 58, 3, 40, 40, Vector2(200, globals::GAME_VIEWPORT_H));
    this->_towerMenuItems.emplace_back("rocketTower", 250, graphics, 58, 45, 40, 40, Vector2(300, globals::GAME_VIEWPORT_H));
    this->_towerMenuItems.emplace_back("sniperTower", 75, graphics, 58, 87, 40, 40, Vector2(400, globals::GAME_VIEWPORT_H));
}

void Menu::update(int elapsedTime, Input &input)
{
    for(int i = 0; i < this->_menuItems.size(); ++i)
    {
        this->_menuItems.at(i).update(elapsedTime, input);
    }

      for(int i = 0; i < this->_towerMenuItems.size(); ++i)
    {
        this->_towerMenuItems.at(i).update(elapsedTime, input);
    }
}

void Menu::draw(Graphics &graphics)
{
    for(int i = 0; i < this->_menuItems.size(); ++i)
    {
        int x = this->_menuItems.at(i).getX();
        int y = this->_menuItems.at(i).getY();
        this->_menuItems.at(i).draw(graphics, x, y);
    }

    for(int i = 0; i < this->_towerMenuItems.size(); ++i)
    {
        int x = this->_towerMenuItems.at(i).getX();
        int y = this->_towerMenuItems.at(i).getY();
        this->_towerMenuItems.at(i).draw(graphics, x, y);
    }
}

Tower* Menu::getTower(Graphics &graphics, int mouseX, int mouseY, int money)
{
    Tower* tower = nullptr;

    for(int i = 0; i < this->_towerMenuItems.size(); ++i)
    {
        if(this->_towerMenuItems.at(i).isClicked() && money >= this->_towerMenuItems.at(i).getPrice())
        {
            //If the player has enough money to purchase the tower corresponding with _towerMenuItems.at(i)._name,
            //tower will point to a new tower subclass. Otherwise, TowerMenuItem::createTower will return nullptr.
            tower = this->_towerMenuItems.at(i).createTower(graphics, mouseX, mouseY, money);

            if(tower != nullptr)
            {
                return tower;
            }
        }
    }

    tower = nullptr;
    return nullptr;
}

void Menu::addTowerMenuItem(TowerMenuItem t)
{
    this->_towerMenuItems.push_back(t);
}
