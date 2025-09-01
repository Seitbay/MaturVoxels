#ifndef SPOT_LIGHT_CLASS_H
#define SPOT_LIGHT_CLASS_H

#include<LightSource.h>

class DirectionalLight : public LightSource
{
private:
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); // направление света

public:
    DirectionalLight(const glm::vec4& lightColor, const  glm::vec3& position) : LightSource(lightColor, position) {};
    DirectionalLight(const glm::vec4& lightColor, const  glm::vec3& position, const glm::vec3& direction) : LightSource(lightColor, position), direction(direction) {};
    
    void Draw(Shader& shader, Camera& camera) override {};

    glm::vec3 getDirection() const { return direction; }
};

#endif

