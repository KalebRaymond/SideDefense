#include "menu.h"
#include "menu_item.h"
#include "globals.h"

Menu::Menu()
{

}

Menu::Menu(Graphics &graphics)
{
    //Populate _menuItems with MenuItem objects
    /*
    this->_menuItems.emplace_back("save", graphics, 69, 3, 16, 16, Vector2(100, 100) );
    this->_menuItems.emplace_back("music", graphics, 69, 21, 16, 16, Vector2(100, 100) );
    this->_menuItems.emplace_back("sfx", graphics, 69, 39, 16, 16, Vector2(100, 100) );
    */

    this->_menuItems.push_back( MenuItem("save", graphics, 46, 3, 16, 16, Vector2( 32, globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 ))) );
    this->_menuItems.push_back( MenuItem("music", graphics, 46, 21, 16, 16, Vector2( 32 + (40 * 1) , globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64 ))) );
    this->_menuItems.push_back( MenuItem("sfx", graphics, 46, 39, 16, 16, Vector2( 32 + (40 * 2), globals::GAME_VIEWPORT_H + (globals::MENU_VIEWPORT_H - 64))) );
    this->_menuItems.push_back( MenuItem("bulletCard", graphics, 3, 3, 40, 40, Vector2(200, globals::GAME_VIEWPORT_H)) );

}

void Menu::update(int elapsedTime)
{
    for(int i = 0; i < this->_menuItems.size(); ++i)
    {
        this->_menuItems.at(i).update(elapsedTime);
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
}
