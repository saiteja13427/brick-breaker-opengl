#include <vector>

#include "game_level.h"
#include "sprite_renderer.h"
#include "resourceman.h"
#include "ball_object.h"

enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOOSE
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

class Game {
    public:
        GameState                  state;
        bool                       keys[1024];
        unsigned int               width, height;
        std::vector<GameLevel>     levels;
        unsigned int               level;

        Game(unsigned int width, unsigned int height): width(width), height(height){}
        ~Game(){};

        void init();
        void processInput(float dt);
        void update(float dt);
        void render();
        void doCollision();
        bool isCollision(GameObject &one, GameObject &two);
        Collision isCollision(BallObject &one, GameObject &two);
        Direction collisionDirection(glm::vec2 target);
        void resetPlayer();
        void resetLevel();
};