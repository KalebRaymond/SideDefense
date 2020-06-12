#ifndef MENU_H
#define MENU_H

#include "menu_item.h"
#include <vector>

class Graphics;

class Menu
{
    public:
        Menu();
        Menu(Graphics &graphics);

        void update(int elapsedTime, Input &input);
        void draw(Graphics &graphics);

        Tower* getTower(Graphics &graphics, int mouseX, int mouseY, int money);

    protected:
        std::vector<MenuItem> _menuItems;
        std::vector<TowerMenuItem> _towerMenuItems;

};
#endif // MENU_H
