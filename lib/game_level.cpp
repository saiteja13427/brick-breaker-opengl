#include<fstream>
#include <sstream>

#include "game_level.h"
#include "resourceman.h"

void GameLevel::load(const char* file, unsigned int levelHeight, unsigned int levelWidth){
    bricks.clear();

    std::string line;
    unsigned int type;
    std::ifstream fs(file);
    std::vector<std::vector<unsigned int>> tilesData;

    if(fs){
        while(std::getline(fs, line)){
            std::istringstream lineStream(line);
            std::vector<unsigned int> row;
            while(lineStream >> type){
                row.push_back(type);
            }
            tilesData.push_back(row);
        }

        if(tilesData.size() > 0){
            init(tilesData, levelHeight, levelWidth);
        }
    }

}
void GameLevel::draw(SpriteRenderer &renderer){
    for(auto &brick: bricks){
        if(!brick.isDestroyed)
            brick.draw(renderer);
    }
}

bool GameLevel::isCompleted(){
    for(auto &brick: bricks){
        if(!brick.isDestroyed)
            return false;
    }
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tiles, unsigned int levelHeight, unsigned int levelWidth){
    unsigned int height = tiles.size();
    unsigned int width = tiles[0].size();
    unsigned int unit_height = levelHeight / static_cast<float>(height);
    unsigned int unit_width = levelWidth / static_cast<float>(width);

    for(int y=0; y<height; y++){
        for(int x = 0; x<width; x++){
            if(tiles[y][x]==1){
                glm::vec2 size = glm::vec2(unit_width, unit_height);
                glm::vec2 position = glm::vec2(unit_width * x, unit_height * y);
                GameObject obj(size, position, ResourceManager::getTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.isSolid = true;
                bricks.push_back(obj);
            } else if(tiles[y][x] > 1){
                glm::vec2 size = glm::vec2(unit_width, unit_height);
                glm::vec2 position = glm::vec2(unit_width * x, unit_height * y);
                glm::vec3 color;
                if(tiles[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if(tiles[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if(tiles[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if(tiles[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                GameObject obj(size, position, ResourceManager::getTexture("block"), color);
                bricks.push_back(obj);
            }
        }
    }
}