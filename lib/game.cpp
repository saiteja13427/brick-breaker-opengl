#include<iostream>
#include<tuple>

#include "game.h"
#include <GLFW/glfw3.h>

std::vector<GameLevel> levels;
GameLevel level1, level2, level3, level4;
SpriteRenderer *renderer;
GameObject *player;
BallObject *ball;


glm::vec2 PLAYER_SIZE = glm::vec2(120.0f, 20.0f);
float PLAYER_VELOCITY = 500.0f;

const glm::vec2 INITIAL_BALL_VELOCITY = glm::vec2(200.0f, -250.0f);
const float BALL_RADIUS = 12.5f;

void Game::init(){
    ResourceManager::loadShader("/home/saiteja/Qualcomm/preparation/breakout-opengl/shaders/sprite.vs", "/home/saiteja/Qualcomm/preparation/breakout-opengl/shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", proj);

    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    ResourceManager::loadTexture("/home/saiteja/Qualcomm/preparation/breakout-opengl/image/awesomeface.png", true, "face");
    ResourceManager::loadTexture("/home/saiteja/Qualcomm/preparation/breakout-opengl/image/block.png", false, "block");
    ResourceManager::loadTexture("/home/saiteja/Qualcomm/preparation/breakout-opengl/image/block_solid.png", false, "block_solid");
    ResourceManager::loadTexture("/home/saiteja/Qualcomm/preparation/breakout-opengl/image/background.jpg", false, "background");
    ResourceManager::loadTexture("/home/saiteja/Qualcomm/preparation/breakout-opengl/image/paddle.png", true, "player");

    level1.load("/home/saiteja/Qualcomm/preparation/breakout-opengl/levels/one.lvl", this->height/2, this->width);
    level2.load("/home/saiteja/Qualcomm/preparation/breakout-opengl/levels/two.lvl", this->height/2, this->width);
    level3.load("/home/saiteja/Qualcomm/preparation/breakout-opengl/levels/three.lvl", this->height/2, this->width);
    level4.load("/home/saiteja/Qualcomm/preparation/breakout-opengl/levels/four.lvl", this->height/2, this->width);
    levels.push_back(level1);
    levels.push_back(level2);
    levels.push_back(level3);
    levels.push_back(level4);
    level = 0;
    state = GAME_ACTIVE;

    glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    player = new GameObject(PLAYER_SIZE, playerPos, ResourceManager::getTexture("player"));

    glm::vec2 ballPos = playerPos + glm::vec2((PLAYER_SIZE.x / 2) - BALL_RADIUS, -2.0f * BALL_RADIUS);
    ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));
}

void Game::processInput(float dt){
    float velocity = PLAYER_VELOCITY * dt;

    if(keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]){
        if(player->position.x > 0.0f)
            player->position.x -= velocity;
    } else if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]){
        if(player->position.x + player->size.x < this->width )
            player->position.x += velocity;
    } else if (keys[GLFW_KEY_SPACE]) {
        if(ball->isStuck) ball->isStuck = false;
    }

}

void Game::update(float dt){
    ball->move(dt, this->width);
    doCollision();
    if(ball->position.y > this->height){
        resetPlayer();
        resetLevel();
    }
}

void Game::resetPlayer(){
    ball->isStuck = true;
    ball->velocity = INITIAL_BALL_VELOCITY;
    glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    player->position = playerPos;
    glm::vec2 ballPos = playerPos + glm::vec2((PLAYER_SIZE.x / 2) - BALL_RADIUS, -2.0f * BALL_RADIUS);
    ball->position = ballPos;
}

void Game::resetLevel(){
    for(auto &brick: levels[level].bricks){
        brick.isDestroyed = false;
    }
}

void Game::render(){
    renderer->DrawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0, 0.0), glm::vec2(this->width, this->height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    if(state == GAME_ACTIVE){
        levels[level].draw(*renderer);
        ball->draw(*renderer);
    }
    player->draw(*renderer);
}

void Game::doCollision(){
    for(auto &brick: levels[level].bricks){
        if(!brick.isDestroyed){
            Collision collision = isCollision(*ball, brick);
            if(std::get<0>(collision)){
                if(!brick.isSolid)
                    brick.isDestroyed = true;

                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vec = std::get<2>(collision);
                
                if(dir == LEFT || dir == RIGHT){
                    ball->velocity.x = -ball->velocity.x;
                    float penetration = ball->radius - std::abs(diff_vec.x);

                    if(dir == LEFT) ball->position.x += penetration;
                    else ball->position.x -= penetration;
                }else{
                    ball->velocity.y = -ball->velocity.y;
                    float penetration = ball->radius - std::abs(diff_vec.y);

                    if(dir == UP) ball->position.y -= penetration;
                    else ball->position.y += penetration;
                }

            }
        }
    }

    Collision playerBallColl = isCollision(*ball, *player);
    if(!ball->isStuck && std::get<0>(playerBallColl)){
        float centerBoard = player->position.x + player->size.x / 2.0f;
        float distance = (ball->position.x + ball->size.x / 2.0f) - centerBoard;
        float percentage = (distance / (player->size.x / 2.0f));

        float strength = 2.0f;
        glm::vec2 oldVelocity = ball->velocity;
        ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->velocity.y = -1.0f * abs(ball->velocity.y);

        ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
    }
}

bool Game::isCollision(GameObject &one, GameObject &two){
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
                        two.position.x + two.size.x >= one.position.x;

    bool collisionY = one.position.y + one.size.y >= two.position.y &&
                        two.position.y + two.size.y >= one.position.y;
    
    return collisionX && collisionY;
}

/*
* To detect if a box collides with a cirlce, this is the algo
1. Find the closes point on the box from the circle center. 
2. Calculate the distance to that point from the center. 
3. See if this distance is less than radius, if yes, there is a collision. 
*/
Collision Game::isCollision(BallObject &one, GameObject &two){
    glm::vec2 center(one.position + one.radius);

    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.position.x + aabb_half_extents.x,
    two.position.y + aabb_half_extents.y);

    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents,
    aabb_half_extents);

    glm::vec2 closest = aabb_center + clamped;

    difference = closest - center;
    if(glm::length(difference) < one.radius){
        return std::make_tuple(true, collisionDirection(difference), difference);
    }else{
        return std::make_tuple(false, UP, glm::vec2(0.0f));
    }
                         
}

Direction Game::collisionDirection(glm::vec2 target){
    glm::vec2 directions[] = {
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(0.0, -1.0),
        glm::vec2(0.0, 1.0),
    };
    float max = 0.0f;
    int dir = -1;
    for(int i=0; i<4; i++){
        float dot = glm::dot(glm::normalize(target), directions[i]);
        if(dot > max){
            dot = max;
            dir = i;
        }
    }

    return (Direction)dir;
}