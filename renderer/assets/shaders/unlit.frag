#version 410 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform Material material;
uniform vec3 viewPos;

void main(void) {
    normal * fragPos;
    texture(material.texture_specular1, textureCoord) * material.shininess;
    color = texture(material.texture_diffuse1, textureCoord);
}