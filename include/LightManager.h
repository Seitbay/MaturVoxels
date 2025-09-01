#ifndef LIGHTMANAGER_CLASS_H
#define LIGHTMANAGER_CLASS_H

#include <vector>
#include <memory>

#include "LightSource.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

#include "UBO.h"

class LightManager
{
public:
    LightManager();
    ~LightManager() = default;

    void updateShaderUBOs(); // обновление всех UBO

    // добавляем свет через unique_ptr
    void addLight(std::unique_ptr<PointLight> light);
    void addLight(std::unique_ptr<DirectionalLight> light);
    void addLight(std::unique_ptr<SpotLight> light);

    void DrawAllLights(Shader& shader, Camera& camera);
    void BindAllLights(Shader& shader);
    void UnbindAllLights();

private:
    std::vector<std::unique_ptr<PointLight>> pointLights;
    std::vector<std::unique_ptr<DirectionalLight>> directionalLights;
    std::vector<std::unique_ptr<SpotLight>> spotLights;

    UBO pointLightUBO{ UBO::Type::PointLight };
    UBO directionalLightUBO{ UBO::Type::DirectionalLight };
    UBO spotLightUBO{ UBO::Type::SpotLight };

    void updatePointLightsUBO();
    void updateDirectionalLightsUBO();
    void updateSpotLightsUBO();
};

#endif
