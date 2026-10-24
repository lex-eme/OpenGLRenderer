#version 410 core

struct Material {
    sampler2D albedo;
    sampler2D metalRoughEmissive;
    vec3 chargeColor;
    float chargeLevel;
};

in vec2 textureCoord;

out vec4 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform Material material;

void main(void) {
    vec4 mre = texture(material.metalRoughEmissive, textureCoord);
    vec4 albedo = texture(material.albedo, textureCoord);
    float charge = step(1 - mre.a, material.chargeLevel);
    vec3 c = charge * material.chargeColor * mre.b;
    color = mix(albedo, vec4(c, 1.0), mre.b);
}