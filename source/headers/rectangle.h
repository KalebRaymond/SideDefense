#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

class Rectangle
{
    public:
        Rectangle()
        {

        }

        Rectangle(int x, int y, int width, int height)
            :   _x(x),
                _y(y),
                _width(width),
                _height(height)
        {

        }

        const int getCenterX() const
        {
            return _x + _width / 2;
        }

        const int getCenterY() const
        {
            return _y + _height / 2;
        }

        const int getLeft() const
        {
            return _x;
        }

        void setX(int x)
        {
            this->_x = x;
        }

        const int getRight() const
        {
            return _x + _width;
        }


        const int getTop() const
        {
            return _y;
        }

        void setY(int y)
        {
            this->_y = y;
        }

        const int getBottom() const
        {
            return _y + _height;
        }

        const int getWidth() const
        {
            return _width;
        }

        void setWidth(int w)
        {
            this->_width = w;
        }

        const int getHeight() const
        {
            return _height;
        }

        void setHeight(int h)
        {
            this->_height = h;
        }

        const int getSide(const sides::Side side) const
        {
            return
                side == sides::TOP ? this->getTop() :
                side == sides::BOTTOM ? this->getBottom() :
                side == sides::LEFT ? this->getLeft() :
                side == sides::RIGHT ? this->getRight() :
                sides::NONE;
        }

        const bool collidesWith(const Rectangle& other) const
        {
            return
                this->getRight() >= other.getLeft() &&
                this->getLeft() <= other.getRight() &&
                this->getTop() <= other.getBottom() &&
                this->getBottom() >= other.getTop();
        }

        const bool isValidRectangle() const
        {
            return ( _x >= 0 && _y >= 0 && _width >= 0 && _height >= 0 );
        }

        const inline Rectangle getRect() const
        {
            return *this;
        }
    private:
        int _x, _y, _width, _height;
};

#endif // RECTANGLE_H
