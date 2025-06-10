#include "particle.h"
    
ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount): shader(shader), texture(texture), amount(amount){
    this->init();
}

void ParticleGenerator::update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset){
    for(int i=0; i<newParticles; i++){
        unsigned int unusedParticle = firstUnusedParticle();
        respawnParticle(particles[unusedParticle], object, offset);
    }

    for(auto &particle: particles){
        particle.life -= dt;
        if(particle.life <= 0.0f) continue;
        particle.position -= particle.velocity * dt;
        particle.color.a -= dt * 2.5f;
    }

}
void ParticleGenerator::draw(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for(auto &particle: this->particles){
        if(particle.life > 0.0f){
            this->shader.setVector2f("offset", particle.position);
            this->shader.setVector4f("particleColor", particle.color);
            this->texture.bind();
            glBindVertexArray(this->VAO);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init(){
    unsigned int VBO;
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);

    glBindVertexArray(0);

    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle(){
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}
void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset){
    float random = ((rand() % 100) - 50) /10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.position = object.position + random + offset;
    particle.color = glm::vec4(glm::vec3(rColor), 1.0f);
    particle.velocity = object.velocity * 0.1f;
    particle.life = 1.0f;
}