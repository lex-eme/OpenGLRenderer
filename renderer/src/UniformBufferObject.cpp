#include "UniformBufferObject.h"

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.inl"


void UniformBufferObject::create() {
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, ubo, 0, 2 * sizeof(glm::mat4));
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
}

void UniformBufferObject::setProjection(const glm::mat4& matrix) const {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(matrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::setView(const glm::mat4& matrix) const {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
