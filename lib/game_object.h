#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>

#include "glad.h"
#include "sprite_renderer.h"
#include "texture.h"


class GameObject {
    public:
        glm::vec2 velocity, size, position;
        glm::vec3 color;
        float rotation;
        bool isSolid;
        bool isDestroyed;

        Texture2D sprite;

        GameObject();
        GameObject(glm::vec2 size, glm::vec2 position, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));

        virtual void draw(SpriteRenderer &renderer);
};

#endif