#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include <string>
#include <SDL.h>

struct SDL_Window;
struct SDL_Renderer;
struct Vector2;

class Graphics
{
    public:
        Graphics();
        ~Graphics();

        /*  Adds an SDL_Surface from an image to the _spriteSheets map */
        SDL_Surface* loadImage(const std::string &filePath);

        /*  These functions take a portion of SDL_Texture* source defined by sourceRectangle and copy
        *   it to the window at destRectangle. destRectangle coordinates are relative to the window
        *
        *   The only difference between blitGame and blitMenu is that blitGame translates the
        *   destRectangle by the camera offsets. This has the effect that any sprites drawn by
        *   blitGame will scroll as the camera moves, while those drawn by blitMenu will stay static
        *   on the window. By default, Sprite::draw(), AnimatedSprite::(), and Tile::draw() use
        *   blitGame, while Menu::draw() and MenuItem::draw() use blitMenu.
        */
        void blitGame(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle);
        void blitMenu(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle);
        void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle);


        void renderText(const char* text, int fontSize, int x, int y, int fR, int fG, int fB);

        /*  Renders everything to the screen
        */
        void flipScreen();

        void clearScreen();
        SDL_Renderer* getRenderer() const;

        /*  moveScreen moves the camera by changing the camera offset, which
        *   is used in blitSurface to change the location of the destination
        *   SDL_Rect. I could have made a Camera class to handle all this, but
        *   I felt the camera is simple enough that it wasn't necessary. All it
        *   does it move left and right.
        */
        void moveScreen(int dx, int dy);
        void setCameraOffset(int x, int y);
        int getCameraOffsetX();
        int getCameraOffsetY();
        void setMaxScroll(int maxX, int maxY);
        Vector2 getMaxScroll();

    private:
        SDL_Window* _window;
        SDL_Renderer* _renderer;

        std::map< std::string, SDL_Surface*> _spriteSheets;

        int _cameraOffsetX, _cameraOffsetY;
        int _maxScrollX, _maxScrollY;
};

#endif // GRAPHICS_H
