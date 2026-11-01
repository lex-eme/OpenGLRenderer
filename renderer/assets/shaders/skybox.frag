#version 410 core

in vec3 textureDir;

out vec4 color;

uniform samplerCube skybox;

void main(void) {
    color =  texture(skybox, textureDir);
}