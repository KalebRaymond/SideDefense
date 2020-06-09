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

        void update(int elapsedTime);
        void draw(Graphics &graphics);

    protected:
        std::vector< MenuItem > _menuItems;

};
#endif // MENU_H
