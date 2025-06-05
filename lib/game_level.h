#ifndef GAME_LEVEL_H_
#define GAME_LEVEL_H_

#include<vector>

#include "game_object.h"

class GameLevel {
    public:
        std::vector<GameObject> bricks;

        GameLevel(){};
        void load(const char* file, unsigned int levelHeight, unsigned int levelWidth);
        void draw(SpriteRenderer &renderer);
        bool isCompleted();
    private:
        void init(std::vector<std::vector<unsigned int>> tiles, unsigned int levelHeight, unsigned int levelWidth);
};

#endif