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

        void update(int elapsedTime, Input &input, Game &game);
        void draw(Graphics &graphics);

        /*  getTower() returns a pointer to a tower of the subclass corresponding to
        *   the towerMenuItem located at the current mouse position, but only if
        *   money is greater than the tower's price. Otherwise, nullptr is returned.
        */
        Tower* getTower(Graphics &graphics, int mouseX, int mouseY, int money);

        void addTowerMenuItem(TowerMenuItem t);

        /*  freeUpgradeItems deletes _upgradeTo and _upgradeFrom, and sets them
        *   both to nullptr
        */
        void freeUpgradeItems();
        void setUpgradeMenuItems( TowerMenuItem* fromItem, TowerMenuItem* toItem);

        MenuState getState();
        void setState(MenuState state);

    protected:
        std::vector<MenuItem> _menuItems;
        std::vector<MenuItem> _upgradeItems;
        std::vector<TowerMenuItem> _towerMenuItems;

        /*  These TowerMenuItems point to TowerMenuItems on the heap that
        *   are created when a tower is selected. _upgradeFrom references an
        *   uninteractable menu item depicting the currently selected tower,
        *   and _upgradeTo depicts the tower that the selected tower can upgrade
        *   to. When _upgradeTo is clicked, the selected tower will be dynamically
        *   casted to the appropriate subclass tower.
        */
        TowerMenuItem* _upgradeFrom;
        TowerMenuItem* _upgradeTo;

        MenuState _state;
};

#endif // MENU_H
