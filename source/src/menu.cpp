#include "menu.h"
#include "menu_item.h"
#include "tower.h"
#include "input.h"
#include "globals.h"

Menu::Menu()
{

}

Menu::Menu(Graphics &graphics)
    :   _state(DEFAULT)
{
    this->_upgradeFrom = nullptr;
    this->_upgradeTo = nullptr;

    //Populate _menuItems with MenuItem objects
    this->_menuItems.emplace_back("hp", graphics, 4, 74, 14, 8, Vector2( 32, globals::GAME_VIEWPORT_H + 10 ), false);
    this->_menuItems.emplace_back("cash", graphics, 4, 81, 26, 8, Vector2( 32, globals::GAME_VIEWPORT_H + 28 ), false);
    this->_menuItems.emplace_back("save", graphics, 3, 3, 16, 16, Vector2( 32, globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 )), true);
    this->_menuItems.emplace_back("music", graphics, 3, 21, 16, 16, Vector2( 32 + (40 * 1) , globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 )), true );
    this->_menuItems.emplace_back("sfx", graphics, 3, 39, 16, 16, Vector2( 32 + (40 * 2), globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 )), true );

    //Populate _upgradeItems with MenuItem objects
    this->_upgradeItems.emplace_back("towerHp", graphics, 4, 74, 14, 8, Vector2( 300, globals::GAME_VIEWPORT_H + 10 ), false);
    this->_upgradeItems.emplace_back("sell", graphics, 4, 89, 26, 7, Vector2( 300, globals::GAME_VIEWPORT_H + 28 ), false);
    this->_upgradeItems.emplace_back("arrow", graphics, 4, 103, 11, 8, Vector2( 425, globals::GAME_VIEWPORT_H + 28 ), false);

    //Populate _towerMenuItems with TowerMenuItems
    this->_towerMenuItems.emplace_back("bulletTower", 100, graphics, 58, 3, 40, 40, Vector2(200, globals::GAME_VIEWPORT_H), true);
    this->_towerMenuItems.emplace_back("rocketTower", 250, graphics, 58, 45, 40, 40, Vector2(300, globals::GAME_VIEWPORT_H), true);
    this->_towerMenuItems.emplace_back("sniperTower", 75, graphics, 58, 87, 40, 40, Vector2(400, globals::GAME_VIEWPORT_H), true);
}

void Menu::update(int elapsedTime, Input &input)
{
    for(int i = 0; i < this->_menuItems.size(); ++i)
    {
        this->_menuItems.at(i).update(elapsedTime, input);
    }

    switch(this->_state)
    {
        case DEFAULT:
            for(int i = 0; i < this->_towerMenuItems.size(); ++i)
            {
                this->_towerMenuItems.at(i).update(elapsedTime, input);
            }

            break;

        case UPGRADE:
            for(int i = 0; i < this->_upgradeItems.size(); ++i)
            {
                this->_upgradeItems.at(i).update(elapsedTime, input);
            }

            if(this->_upgradeFrom != nullptr)
            {
                this->_upgradeFrom->update(elapsedTime, input);
            }
            if(this->_upgradeTo != nullptr)
            {
                this->_upgradeTo->update(elapsedTime, input);
            }

            break;
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

    switch(this->_state)
    {
        case DEFAULT:
            for(int i = 0; i < this->_towerMenuItems.size(); ++i)
            {
                int x = this->_towerMenuItems.at(i).getX();
                int y = this->_towerMenuItems.at(i).getY();
                this->_towerMenuItems.at(i).draw(graphics, x, y);
            }

            break;

        case UPGRADE:
            for(int i = 0; i < this->_upgradeItems.size(); ++i)
            {
                int x = this->_upgradeItems.at(i).getX();
                int y = this->_upgradeItems.at(i).getY();
                this->_upgradeItems.at(i).draw(graphics, x, y);
            }

            if(this->_upgradeFrom != nullptr)
            {
                int x = this->_upgradeFrom->getX();
                int y = this->_upgradeFrom->getY();
                this->_upgradeFrom->draw(graphics, x, y);
            }
            if(this->_upgradeTo != nullptr)
            {
                int x = this->_upgradeTo->getX();
                int y = this->_upgradeTo->getY();
                this->_upgradeTo->draw(graphics, x, y);
            }
            break;
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

void Menu::setUpgradeMenuItems(TowerMenuItem* fromItem, TowerMenuItem* toItem)
{
    this->_upgradeFrom = fromItem;
    this->_upgradeTo = toItem;
}

void Menu::setState(MenuState state)
{
    this->_state = state;
}
