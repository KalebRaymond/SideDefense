#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <string>
#include <map>
#include <vector>
#include "sprite.h"
#include "globals.h"

class Graphics;

class AnimatedSprite : public Sprite
{
    public:
        AnimatedSprite();
        AnimatedSprite(Graphics& graphics, const std::string& filepath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate);

        void playAnimation(std::string animation, bool once = false);
        void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);
        void setVisible(bool visible);

        void update(int elapsedTime);

        void draw(Graphics& graphics, int x, int y);

        const inline std::string getCurrentAnimation() const
        {
            return this->_currentAnimation;
        }

        const inline int getFrameIndex() const
        {
            return this->_frameIndex;
        }

        inline void setDestroy(bool destroy)
        {
            this->_destroy = destroy;
        }

        const inline bool shallBeDestroyed() const
        {
            return this->_destroy;
        }

    protected:
        int _timeToUpdate;
        bool _currentAnimationOnce;
        bool _animationDone;
        bool _destroy;
        std::string _currentAnimation;

        void resetAnimations();
        void stopAnimation();

    private:
        std::map< std::string, std::vector<SDL_Rect> > _animations;
        std::map< std::string, Vector2 > _offsets;

        int _timeElapsed;
        int _frameIndex;
        bool _visible;
};

#endif // ANIMATED_SPRITE_H
