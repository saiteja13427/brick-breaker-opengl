#ifndef POST_PROCESSOR_H_
#define POST_PROCESSOR_H_

#include "shader.h"
#include "texture.h"

class PostProcessor {
    public:
        Shader postProcessingShader;
        Texture2D texture;
        unsigned width, height; 

        bool confuse, chaos, shake;

        PostProcessor(Shader shader, unsigned int width, unsigned int height);

        void BeginRender();
        void EndRender();
        void Render(float time);
    private:
        unsigned int MSBFO, FBO;
        unsigned int RBO, VAO;
        void initRenderData();
};

#endif