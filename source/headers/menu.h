#ifndef MENU_H
#define MENU_H

#include "menu_item.h"
#include <vector>

enum MenuState
{
    DEFAULT,
    UPGRADE
};

class Graphics;

class Menu
{
    public:
        Menu();
        Menu(Graphics &graphics);

        void update(int elapsedTime, Input &input);
        void draw(Graphics &graphics);

        /*  getTower() returns a pointer to a tower of the subclass corresponding to
        *   the towerMenuItem located at the current mouse position, but only if
        *   money is greater than the tower's price. Otherwise, nullptr is returned.
        */
        Tower* getTower(Graphics &graphics, int mouseX, int mouseY, int money);

        void addTowerMenuItem(TowerMenuItem t);

        void setUpgradeMenuItems( TowerMenuItem* fromItem, TowerMenuItem* toItem);
        void setState(MenuState state);

    protected:
        std::vector<MenuItem> _menuItems;
        std::vector<MenuItem> _upgradeItems;
        std::vector<TowerMenuItem> _towerMenuItems;

        /*
        */
        TowerMenuItem* _upgradeFrom;
        TowerMenuItem* _upgradeTo;

        MenuState _state;
};

#endif // MENU_H
