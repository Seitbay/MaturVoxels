#include "LightManager.h"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

// ------------------- Constructor / Destructor -------------------

LightManager::LightManager() = default;

// ------------------- Add lights -------------------

void LightManager::addLight(std::unique_ptr<PointLight> light) {
    pointLights.push_back(std::move(light));
}

void LightManager::addLight(std::unique_ptr<DirectionalLight> light) {
    directionalLights.push_back(std::move(light));
}

void LightManager::addLight(std::unique_ptr<SpotLight> light) {
    spotLights.push_back(std::move(light));
}

// ------------------- Update UBOs -------------------

void LightManager::updatePointLightsUBO() {
    PointLightBlock block{};
    block.numPoint = std::min(static_cast<int>(pointLights.size()), MAX_LIGHTS);
    block._pad1 = glm::vec3(0.0f);

    for (int i = 0; i < block.numPoint; i++) {
        auto& l = pointLights[i];
        block.lights[i].position = glm::vec4(l->lightPosition, 1.0f);
        block.lights[i].color    = glm::vec4(l->lightColor.x, l->lightColor.y, l->lightColor.z, 1.0f);
        block.lights[i].params   = glm::vec4(l->getA(), l->getB(), 0.0f, 0.0f);
    }

    pointLightUBO.Update(&block, sizeof(block));
}

void LightManager::updateDirectionalLightsUBO() {
    DirectionalLightBlock block{};
    block.numDir = std::min(static_cast<int>(directionalLights.size()), MAX_LIGHTS);

    for (int i = 0; i < block.numDir; i++) {
        auto& l = directionalLights[i];
        block.lights[i].direction = glm::vec4(l->getDirection(), 0.0f);
        block.lights[i].color     = glm::vec4(l->lightColor.x, l->lightColor.y, l->lightColor.z, 1.0f);
    }

    directionalLightUBO.Update(&block, sizeof(block));
}

void LightManager::updateSpotLightsUBO() {
    SpotLightBlock block{};
    block.numSpot = std::min(static_cast<int>(spotLights.size()), MAX_LIGHTS);

    for (int i = 0; i < block.numSpot; i++) {
        auto& l = spotLights[i];
        block.lights[i].position  = glm::vec4(l->lightPosition, 1.0f);
        block.lights[i].direction = glm::vec4(l->getDirection(), 0.0f);
        block.lights[i].color     = glm::vec4(l->lightColor.x, l->lightColor.y, l->lightColor.z, 1.0f);
        block.lights[i].cone      = glm::vec4(l->getInnerCone(), l->getOuterCone(), 0.0f, 0.0f);
    }

    spotLightUBO.Update(&block, sizeof(block));
}

void LightManager::updateShaderUBOs() {
    updatePointLightsUBO();
    updateDirectionalLightsUBO();
    updateSpotLightsUBO();
}

// ------------------- Draw -------------------

void LightManager::DrawAllLights(Shader& shader, Camera& camera) {
    for (auto& light : pointLights) {
        light->Draw(shader, camera);
    }
    for (auto& light : directionalLights) {
        light->Draw(shader, camera);
    }
    for (auto& light : spotLights) {
        light->Draw(shader, camera);
    }
}

// ------------------- Bind / Unbind -------------------

void LightManager::BindAllLights(Shader& shader) {
    pointLightUBO.BindToShader(shader.ID, "PointLights", 0);
    directionalLightUBO.BindToShader(shader.ID, "DirectionalLights", 1);
    spotLightUBO.BindToShader(shader.ID, "SpotLights", 2);
}

void LightManager::UnbindAllLights() {
    pointLightUBO.Unbind();
    directionalLightUBO.Unbind();
    spotLightUBO.Unbind();
}
