    #ifndef LIGHT_SOURCE_CLASS_H
    #define LIGHT_SOURCE_CLASS_H

    #include<glm/glm.hpp>

    #include"Model.h"
    #include"shaderClass.h"
    #include"Camera.h"

    class LightSource {
    public:
        glm::vec4 lightColor;
        glm::vec3 lightPosition;

        LightSource(const glm::vec4& lightColor, const glm::vec3& lightPosition = glm::vec3(0.0f, 0.0f, 0.0f)) : lightColor(lightColor), lightPosition(lightPosition) {setupLightSource();}
        virtual ~LightSource() {            
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        virtual void Draw(Shader& shader, Camera& camera) = 0;

    protected:
        unsigned int VAO, VBO, EBO;
        void setupLightSource();
    };

    #endif