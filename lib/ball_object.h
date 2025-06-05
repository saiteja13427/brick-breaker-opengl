#ifndef BALL_OBJECT_H_
#define BALL_OBJECT_H_

#include "game_object.h"
#include "texture.h"

class BallObject: public GameObject {
    public:
        float radius;
        bool isStuck; // stuck on player paddle or not
        BallObject();
        BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
        ~BallObject();

        glm::vec2 move(float dt, unsigned int window_width);
        void reset (glm::vec2 pos, glm::vec2 velocity);
};

#endif