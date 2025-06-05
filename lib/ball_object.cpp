#include "ball_object.h"

BallObject::BallObject():
            GameObject(),
            radius(12.5f), 
            isStuck(true)
{

}

BallObject::BallObject(
            glm::vec2 pos, 
            float radius, 
            glm::vec2 velocity, 
            Texture2D sprite):
                GameObject(glm::vec2(radius * 2.0f, radius * 2.0f), pos, sprite, glm::vec3(1.0f), velocity), radius(radius), isStuck(true)
{

}

glm::vec2 BallObject::move(float dt, unsigned int window_width)
{
    if(!isStuck){
        position += velocity * dt;

        if(position.x < 0.0f){
            velocity.x = -velocity.x;
            position.x = 0.0f;
        } else if (position.x + size.x > window_width) {
            velocity.x = -velocity.x;
            position.x = window_width - size.x;
        } else if (position.y < 0) {
            velocity.y = -velocity.y;
            position.y = 0.0f;
        }
    }
    return position;
}

void BallObject::reset (glm::vec2 pos, glm::vec2 velocity){}