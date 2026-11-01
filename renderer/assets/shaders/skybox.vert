#version 410 core

layout (location = 0) in vec3 position;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec3 textureDir;

void main(void) {
    mat4 noTranslation = mat4(mat3(view));
    vec4 pos = projection * noTranslation * vec4(position, 1.0);
    gl_Position = pos.xyww;
    textureDir = position;
}