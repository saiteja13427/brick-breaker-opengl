#include "game_object.h"

GameObject::GameObject():
    size(glm::vec2(1.0f)), 
    position(glm::vec2(0.0f)), 
    sprite(), 
    color(glm::vec3(1.0f)), 
    velocity(glm::vec2(0.0f)), 
    rotation(0.0f), 
    isDestroyed(false), 
    isSolid(false){}


GameObject::GameObject(
    glm::vec2 size, 
    glm::vec2 position, 
    Texture2D sprite, 
    glm::vec3 color, 
    glm::vec2 velocity):
    size(size), 
    position(position), 
    sprite(sprite), 
    color(color), 
    velocity(velocity), 
    rotation(0.0f), 
    isDestroyed(false), 
    isSolid(false){}

void GameObject::draw(SpriteRenderer &renderer){
    renderer.DrawSprite(this->sprite, position, size, rotation, color);
}