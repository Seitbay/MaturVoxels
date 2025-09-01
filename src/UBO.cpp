#include "UBO.h"
#include <iostream>

UBO::UBO(Type type)
{
    glGenBuffers(1, &ID);

    switch (type)
    {
        case Type::PointLight:       
            bufferSize = sizeof(PointLightBlock); 
            break;
        case Type::DirectionalLight: 
            bufferSize = sizeof(DirectionalLightBlock); 
            break;
        case Type::SpotLight:        
            bufferSize = sizeof(SpotLightBlock); 
            break;
    }

    std::cout << "UBO buffer size: " << bufferSize << " bytes" << std::endl;

    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Привязка UBO к конкретному шейдеру и binding point
void UBO::BindToShader(GLuint shaderID, const char* blockName, GLuint binding)
{
    GLuint blockIndex = glGetUniformBlockIndex(shaderID, blockName);
    glUniformBlockBinding(shaderID, blockIndex, binding);
    glBindBufferRange(GL_UNIFORM_BUFFER, binding, ID, 0, bufferSize);
}

void UBO::Update(const void* data, GLsizeiptr size, GLintptr offset)
{
    if (size > bufferSize) {
        std::cout << "ERROR: Update size " << size << " > buffer size " << bufferSize << std::endl;
        return;
    }
    
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::Delete()
{
    glDeleteBuffers(1, &ID);
}

void UBO::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
}

void UBO::Unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
