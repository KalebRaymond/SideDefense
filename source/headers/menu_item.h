#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include "sprite.h"
#include <string>

class Graphics;
class Input;
class Tower;
class Game;

class MenuItem : public Sprite
{
    public:
        MenuItem();
        MenuItem(std::string name, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 location, bool interactable);

        virtual void update(int elapsedTime, Input &input, Game &game);
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
        int _sourceX, _sourceY;
        bool _clicked;

        /*  If _interactable is false, MenuItem will not interact with mouse at all.
        *   Obviously, I couldn't use the word static.
        */
        bool _interactable;
};

class TowerMenuItem : public MenuItem
{
    public:
        TowerMenuItem();
        TowerMenuItem(std::string name, int price, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 location, bool interactable);

        void update(int elapsedTime, Input &input, Game &game);
        void onHover();

        const int inline getPrice() const
        {
            return this->_price;
        }

        /*  createTower() returns a pointer to a tower subclass determined by this->_name
        *   initialized to the mouse position, if money is greater than this->_price.
        *   Otherwise, nullptr is returned.
        */
        Tower* createTower(Graphics &graphics, int mouseX, int mouseY, int money);

    private:
        int _price;
};

#endif // MENU_ITEM_H
