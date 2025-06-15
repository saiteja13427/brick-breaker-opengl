#include <iostream>

#include "post_processor.h"

PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height): 
                postProcessingShader(shader),
                width (width),
                height (height),
                confuse(false),
                shake(false),
                chaos(false)
{
    glGenFramebuffers(1, &this->FBO);
    glGenFramebuffers(1, &this->MSBFO);
    glGenRenderbuffers(1, &this->RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, this->MSBFO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERROR::POSTPROCESSING: Failed to create multisampled buffer" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->texture.generate(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERROR::POSTPROCESSING: Failed to init FBO" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->initRenderData();
    this->postProcessingShader.setInteger("tex", 0, true);
    float offset = 1.0f/300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->postProcessingShader.ID, "offsets"), 9, (float*)offsets);

    int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };

    glUniform1iv(glGetUniformLocation(this->postProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
    glUniform1fv(glGetUniformLocation(this->postProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender(){
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSBFO);
    glClearColor(0.0f, 0.0f, 0.0f, .0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void PostProcessor::EndRender(){
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSBFO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(float time){
    this->postProcessingShader.use();
    this->postProcessingShader.setInteger("confuse", confuse);
    this->postProcessingShader.setInteger("chaos", chaos);
    this->postProcessingShader.setInteger("shake", shake);
    this->postProcessingShader.setFloat("time", time);
    glActiveTexture(GL_TEXTURE0);
    this->texture.bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessor::initRenderData(){
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}