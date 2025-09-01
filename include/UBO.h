#ifndef UBO_CLASS_H 
#define UBO_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>

constexpr int MAX_LIGHTS = 64; // должно совпадать с GLSL

// ----------- CPU-структуры под std140 -----------

// PointLight
struct PointLightData {
    alignas(16) glm::vec4 position; // 16
    alignas(16) glm::vec4 color;    // 16
    alignas(16) glm::vec4 params;   // 16
    alignas(16) glm::vec4 _pad;     // 16  => ИТОГО 64
};

struct PointLightBlock {
    alignas(16) int numPoint;
    alignas(16) glm::vec3 _pad1;
    PointLightData lights[MAX_LIGHTS]; // Теперь элементы будут правильно выровнены
};

// DirectionalLight
struct DirectionalLightData {
    glm::vec4 direction;
    glm::vec4 color;
};

struct DirectionalLightBlock {
    int numDir;
    glm::vec3 _pad2;
    DirectionalLightData lights[MAX_LIGHTS];
};

// SpotLight
struct SpotLightData {
    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 color;
    glm::vec4 cone; // x = inner, y = outer
};

struct SpotLightBlock {
    int numSpot;
    glm::vec3 _pad3;
    SpotLightData lights[MAX_LIGHTS];
};

// ----------- UBO класс -----------


class UBO
{
public:
    enum class Type { PointLight, DirectionalLight, SpotLight };

    GLuint ID;
    GLsizeiptr bufferSize = 0;

    UBO() = default;
    ~UBO() { Delete(); }

    UBO(Type type);

    void BindToShader(GLuint shaderID, const char* blockName, GLuint binding);

    void Update(const void* data, GLsizeiptr size, GLintptr offset = 0);
    void Delete();
    
    void Bind();
    void Unbind();

    // non copyable
    UBO(const UBO&) = delete;
    UBO& operator=(const UBO&) = delete;

    UBO(UBO&& other) noexcept {
        ID = other.ID;
        bufferSize = other.bufferSize;
        other.ID = 0;
    }

    UBO& operator=(UBO&& other) noexcept {
        if (this != &other) {
            Delete();
            ID = other.ID;
            bufferSize = other.bufferSize;
            other.ID = 0;
        }
        return *this;
    }

private:
    /* data */
};

#endif