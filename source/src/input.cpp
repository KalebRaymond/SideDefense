#include "input.h"

Input::Input()
    :   _leftClicked(false),
        _rightClicked(false)
{
}

void Input::beginNewFrame()
{
    this->_pressedKeys.clear();
    this->_releasedKeys.clear();
}

//Called whenever a key is pressed
void Input::keyDownEvent(const SDL_Event& event)
{
    this->_pressedKeys[event.key.keysym.scancode] = true;
    this->_heldKeys[event.key.keysym.scancode] = true;
}

//Called whenever a key is released
void Input::keyUpEvent(const SDL_Event& event)
{
    this->_releasedKeys[event.key.keysym.scancode] = true;
    this->_heldKeys[event.key.keysym.scancode] = false;
}

//Returns whether key was pressed during current frame
bool Input::wasKeyPressed(SDL_Scancode key)
{
    return this->_pressedKeys[key];
}

//Returns whether key was released during current frame
bool Input::wasKeyReleased(SDL_Scancode key)
{
    return this->_releasedKeys[key];
}

//Check if key is currently held down
bool Input::isKeyHeld(SDL_Scancode key)
{
    return this->_heldKeys[key];
}

void Input::setLeftClick(bool val)
{
    this->_leftClicked = val;
}

bool Input::getLeftClick()
{
    return this->_leftClicked;
}

void Input::setLeftReleased(bool val)
{
    this->_leftReleased = val;
}

bool Input::getLeftReleased()
{
    return this->_leftReleased;
}

void Input::setRightClick(bool val)
{
    this->_rightClicked = val;
}

bool Input::getRightClick()
{
    return this->_rightClicked;
}

void Input::setRightReleased(bool val)
{
    this->_rightReleased = val;
}

bool Input::getRightReleased()
{
    return this->_rightReleased;
}
