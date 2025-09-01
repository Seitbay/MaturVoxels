#ifndef DIRECT_LIGHT_CLASS_H
#define DIRECT_LIGHT_CLASS_H

#include<LightSource.h>

class SpotLight : public LightSource
{
private:
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); // направление света (вниз)
    float innerCone = glm::cos(glm::radians(12.5f)); // внутренний угол конуса (в радианах)
    float outerCone = glm::cos(glm::radians(17.5f)); // внешний угол конуса (в радианах)
public:
    SpotLight(const glm::vec4& lightColor, const glm::vec3& position) : LightSource(lightColor, position) {};
    SpotLight(const glm::vec4& lightColor, const glm::vec3& position, const glm::vec3& direction) : LightSource(lightColor, position), direction(direction) {};
    SpotLight(const glm::vec4& lightColor, const glm::vec3& position, const glm::vec3& direction, float innerConeDeg, float outerConeDeg) 
        : LightSource(lightColor, position), direction(direction), 
          innerCone(glm::cos(glm::radians(innerConeDeg))), 
          outerCone(glm::cos(glm::radians(outerConeDeg))) 
    {
        if (innerCone > outerCone) {
            std::swap(innerCone, outerCone); // гарантируем, что innerCone <= outerCone
        }
    }
    
    void Draw(Shader& shader, Camera& camera) override {};

    glm::vec3 getDirection() const { return direction; }
    float getInnerCone() const { return innerCone; }
    float getOuterCone() const { return outerCone; }
};

#endif