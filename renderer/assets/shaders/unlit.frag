#version 410 core

struct Material {
    sampler2D texture_diffuse1;
};

in vec2 textureCoord;

out vec4 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform Material material;

void main(void) {
    color = texture(material.texture_diffuse1, textureCoord);
}