#ifndef LIGHT_SOURCE_CLASS_H
#define LIGHT_SOURCE_CLASS_H

#include <glm/glm.hpp>

#include "Model.h"
#include "shaderClass.h"
#include "Camera.h"

class LightSource {
public:
    glm::vec4 lightColor;
    glm::vec3 lightPosition;

    LightSource(const glm::vec4& lightColor, 
                const glm::vec3& lightPosition = glm::vec3(0.0f, 0.0f, 0.0f))
        : lightColor(lightColor), lightPosition(lightPosition) {
        setupLightSource();
    }

    virtual ~LightSource() {            
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    // ❌ Запрещаем копирование
    LightSource(const LightSource&) = delete;
    LightSource& operator=(const LightSource&) = delete;

    // ✅ Разрешаем перемещение
    LightSource(LightSource&& other) noexcept {
        // перенимаем данные
        lightColor = other.lightColor;
        lightPosition = other.lightPosition;

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        // обнуляем у старого, чтобы деструктор не удалил ресурсы дважды
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }

    LightSource& operator=(LightSource&& other) noexcept {
        if (this != &other) {
            // сначала очищаем свои ресурсы
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            // перенимаем данные
            lightColor = other.lightColor;
            lightPosition = other.lightPosition;

            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;

            // обнуляем у старого
            other.VAO = 0;
            other.VBO = 0;
            other.EBO = 0;
        }
        return *this;
    }

    virtual void Draw(Shader& shader, Camera& camera) = 0;

protected:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    void setupLightSource();
};

#endif
