#version 410 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out vec2 textureCoord;

void main(void) {
    gl_Position = projection * view * model * vec4(position, 1.0);
    textureCoord = texCoord;
}