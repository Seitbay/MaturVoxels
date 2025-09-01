#ifndef POINT_LIGHT_CLASS_H
#define POINT_LIGHT_CLASS_H

#include<LightSource.h>

class PointLight : public LightSource
{
private:
    float a = 0.3f; // attenuation constant
    float b = 0.1f; // attenuation linear
public:
    PointLight(const glm::vec4& lightColor, const  glm::vec3& position) : LightSource(lightColor, position) {};
    PointLight(const glm::vec4& lightColor, const  glm::vec3& position, float a, float b) : LightSource(lightColor, position), a(a), b(b) {};
    void Draw(Shader& shader, Camera& camera) override;

    float getA() const { return a; }
    float getB() const { return b; }
};

#endif