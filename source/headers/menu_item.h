#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include "sprite.h"
#include <string>

class Graphics;

class MenuItem : public Sprite
{
    public:
        MenuItem();
        MenuItem(std::string name, Graphics &graphics, int sourceX, int sourceY, int width, int height, Vector2 location);

        void update(int elapsedTime);
        void draw(Graphics &graphics, int x, int y);

        void setSprite(int sourceX, int sourceY, int width, int height);

        const inline std::string getName() const
        {
            return this->_name;
        }

    private:
        std::string _name;
};

#endif // MENU_ITEM_H
