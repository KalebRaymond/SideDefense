#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "globals.h"

Graphics::Graphics()
{
    this->_window = SDL_CreateWindow("TowerDefense", 100, 100, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0);
    this->_renderer = SDL_CreateRenderer(this->_window, 0, 0);

    this->_cameraOffsetX = 0;
    this->_cameraOffsetY = 0;
    this->_maxScrollX = globals::GAME_VIEWPORT_W;
    this->_maxScrollY = globals::GAME_VIEWPORT_H;

    SDL_Rect viewport = {0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT};
    SDL_RenderSetViewport(this->_renderer, &viewport);
}

Graphics::~Graphics()
{
    SDL_DestroyWindow(this->_window);
    SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath)
{
    if(this->_spriteSheets.count(filePath) == 0)
    {
        this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }

    return this->_spriteSheets[filePath];
}

void Graphics::blitGame(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle)
{
    SDL_Rect cameraRect = { destRectangle->x + this->_cameraOffsetX,
                            destRectangle->y + this->_cameraOffsetY,
                            destRectangle->w,
                            destRectangle->h};

    blitSurface(source, sourceRectangle, &cameraRect);
}

void Graphics::blitMenu(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle)
{
    blitSurface(source, sourceRectangle, destRectangle);
}

void Graphics::blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle)
{
    SDL_RenderCopy(this->_renderer, source, sourceRectangle, destRectangle);
}

void Graphics::renderText(const char* text, int fontSize, int x, int y, int fR, int fG, int fB)
{
    TTF_Font* font = TTF_OpenFont("content/fonts/VT323-Regular.ttf", fontSize);
    SDL_Color textColor = {fR, fB, fG};

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->_renderer, textSurface);
    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textDimensions = {x, y, textWidth, textHeight};
    SDL_RenderCopy(this->_renderer, texture, NULL, &textDimensions);

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

void Graphics::flipScreen()
{
    SDL_RenderPresent(this->_renderer);
}

void Graphics::clearScreen()
{
    SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const
{
    return this->_renderer;
}

void Graphics::moveScreen(int dx, int dy)
{
    int newX = this->_cameraOffsetX + dx;
    if(newX < -(this->_maxScrollX) || newX > 0)
    {
        newX = this->_cameraOffsetX;
    }
    this->_cameraOffsetX = newX;

    int newY = this->_cameraOffsetY + dy;
    if(newY < -(this->_maxScrollY) || newY > 0)
    {
        newY = this->_cameraOffsetY;
    }
    this->_cameraOffsetY = newY;
}

void Graphics::setCameraOffset(int x, int y)
{
    this->_cameraOffsetX = x;
    this->_cameraOffsetY = y;
}

int Graphics::getCameraOffsetX()
{
    return this->_cameraOffsetX;
}

int Graphics::getCameraOffsetY()
{
    return this->_cameraOffsetY;
}

void Graphics::setMaxScroll(int maxX, int maxY)
{
    this->_maxScrollX = maxX;
    this->_maxScrollY = maxY;
}

Vector2 Graphics::getMaxScroll()
{
    return Vector2(this->_maxScrollX, this->_maxScrollY);
}
