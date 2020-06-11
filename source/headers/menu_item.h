#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include "sprite.h"
#include <string>

class Graphics;
class Input;
class Tower;

class MenuItem : public Sprite
{
    public:
        MenuItem();
        MenuItem(std::string name, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 location);

        virtual void update(int elapsedTime, Input &input);
        void draw(Graphics &graphics, int x, int y);

        void setSprite(std::string name, int sourceX, int sourceY, int width, int height);

        /*  onHover is virtual because in both MenuItem and TowerMenuItem, onHover simply changes
        *   the sourceRect of the object's current sprite. Due to their layout in the sprite sheet,
        *   different formulas for locating the correct sprite are needed depending on the class.
        *
        *   onClick is fundamentally more different between the classes. onClick does not return
        *   anything when a MenuItem is clicked. In contrast, when a TowerMenuItem is clicked, it
        *   returns a Tower* to be pushed into Level::_towers. The logic for TowerMenuItem::onClick
        *   is handled completely by Menu::update and Menu::getTower.
        */
        virtual void onHover();
        void onClick();

        const inline std::string getName() const
        {
            return this->_name;
        }

        const inline bool isClicked() const
        {
            return this->_clicked;
        }

    protected:
        std::string _name;
        bool _clicked;
};

class TowerMenuItem : public MenuItem
{
    public:
        TowerMenuItem();
        TowerMenuItem(std::string name, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 location);

        void update(int elapsedTime, Input &input);
        void onHover();

        Tower* createTower(Graphics &graphics, int mouseX, int mouseY);
};

#endif // MENU_ITEM_H
