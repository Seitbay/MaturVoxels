#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 camPos;

#define MAX_LIGHTS 64

// ----------- СТРУКТУРЫ СВЕТА -----------
struct PointLight {
    vec4 position; // xyz = pos, w = не используется
    vec4 color;    // rgba = цвет
    vec4 params;   // x = a, y = b, z,w = запас
    vec4 _pad;
};

struct DirectionalLight {
    vec4 direction; // xyz = направление, w = запас
    vec4 color;     // rgba
};

struct SpotLight {
    vec4 position;   // xyz = pos, w = запас
    vec4 direction;  // xyz = dir, w = запас
    vec4 color;      // rgba
    vec4 cone;       // x = innerCone, y = outerCone, z,w = запас
};
// ---------------------------------------

// ----------- БУФЕРЫ СВЕТА -----------
layout(std140) uniform PointLights {
    int numPoint;   // количество активных источников
    vec3 _pad1;     // паддинг (int в std140 выравнивается до vec4)
    PointLight pointLights[MAX_LIGHTS];
};

layout(std140) uniform DirectionalLights {
    int numDir;
    vec3 _pad2;
    DirectionalLight dirLights[MAX_LIGHTS];
};

layout(std140) uniform SpotLights {
    int numSpot;
    vec3 _pad3;
    SpotLight spotLights[MAX_LIGHTS];
};
// ---------------------------------------


vec4 CalcPointLight(PointLight light)
{
    vec3 lightVec = light.position.xyz - crntPos;
    float dist = length(lightVec);
    
    // Параметры из структуры
    float a = light.params.x; // 0.3
    float b = light.params.y; // 0.1
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    // diffuse lighting
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 halfwayVec = normalize(viewDirection + lightDirection);
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        specular = specAmount * specularLight;
    }
    return (texture(diffuse0, texCoord) * (diffuse * inten) + 
            texture(specular0, texCoord).r * specular * inten) * light.color;
}

vec4 CalcDirectionalLight(DirectionalLight light)
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light.direction.xyz); // направление к объекту

    float diffuse = max(dot(normal, lightDir), 0.0f);

    float specular = 0.0f;
    if (diffuse > 0.0f) {
        vec3 viewDir = normalize(camPos - crntPos);
        vec3 halfway = normalize(viewDir + lightDir);
        float specAmount = pow(max(dot(normal, halfway), 0.0f), 16);
        specular = specAmount * 0.5f;
    }

    float ambient = 0.2f;

    return (texture(diffuse0, texCoord) * (diffuse + ambient) +
            texture(specular0, texCoord).r * specular) * light.color;
}

vec4 CalcSpotLight(SpotLight light)
{
    vec3 lightVec = light.position.xyz - crntPos;
    vec3 lightDir = normalize(lightVec);

    vec3 normal = normalize(Normal);
    float diffuse = max(dot(normal, lightDir), 0.0f);

    float specular = 0.0f;
    if (diffuse > 0.0f) {
        vec3 viewDir = normalize(camPos - crntPos);
        vec3 halfway = normalize(viewDir + lightDir);
        float specAmount = pow(max(dot(normal, halfway), 0.0f), 16);
        specular = specAmount * 0.5f;
    }

    // угол (проверка, попадает ли фрагмент в конус)
    float theta = dot(normalize(-light.direction.xyz), lightDir);
    float epsilon = (light.cone.x - light.cone.y); // inner - outer
    float intensity = clamp((theta - light.cone.y) / epsilon, 0.0f, 1.0f);

    float ambient = 0.2f;

    return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) +
            texture(specular0, texCoord).r * specular * intensity) * light.color;
}	

void main()
{
    vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1.0);

    vec4 result = ambientColor * 0.2; // базовый ambient

    for (int i = 0; i < numPoint; i++)
        result += CalcPointLight(pointLights[i]); // используйте новые имена

    for (int i = 0; i < numDir; i++)
        result += CalcDirectionalLight(dirLights[i]); // используйте новые имена

    for (int i = 0; i < numSpot; i++)
        result += CalcSpotLight(spotLights[i]); // используйте новые имена

    FragColor = result;
}