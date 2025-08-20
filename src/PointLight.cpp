#include "PointLight.h"

void PointLight::Draw(Shader &shader, Camera &camera)
{

    shader.Activate();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(lightPosition));
    model = glm::scale(model, glm::vec3(0.2f));
    
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1.0f);
    camera.Matrix(shader, "camMatrix");

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}