#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <vector>
#include <glm/glm.hpp>


#include "shader.h"
#include "texture.h"
#include "game_object.h"

struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    float life;
    Particle(): position(0.0f),velocity(0.0f), color(1.0f), life(0.0f) {}
};

class ParticleGenerator {
    public:
        // constructor
        ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
        // update all particles
        void update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
        // render all particles
        void draw();
    private:
        std::vector<Particle> particles;
        unsigned int amount;

        Shader shader;
        Texture2D texture;
        unsigned int VAO;

        void init();
        unsigned int firstUnusedParticle();
        void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif