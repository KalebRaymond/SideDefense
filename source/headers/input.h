#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <map>

class Input
{
    public:
        Input();
        void beginNewFrame();
        void keyUpEvent(const SDL_Event& event);
        void keyDownEvent(const SDL_Event& event);

        bool wasKeyPressed(SDL_Scancode key);
        bool wasKeyReleased(SDL_Scancode key);
        bool isKeyHeld(SDL_Scancode key);

        void setLeftClick(bool val);
        bool getLeftClick();
        void setLeftReleased(bool val);
        bool getLeftReleased();
        void setRightClick(bool val);
        bool getRightClick();
        void setRightReleased(bool val);
        bool getRightReleased();

    private:
        std::map< SDL_Scancode, bool> _heldKeys;
        std::map< SDL_Scancode, bool> _pressedKeys;
        std::map< SDL_Scancode, bool> _releasedKeys;

        bool _leftClicked;
        bool _leftReleased;
        bool _rightClicked;
        bool _rightReleased;
};

#endif
