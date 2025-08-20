#ifndef POINT_LIGHT_CLASS_H
#define POINT_LIGHT_CLASS_H

#include<LightSource.h>

class PointLight : public LightSource
{
private:

public:
    PointLight(const glm::vec4& lightColor, glm::vec3& position) : LightSource(lightColor, position) {};
    void Draw(Shader& shader, Camera& camera) override;
};

#endif