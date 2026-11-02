#version 410 core

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec3 textureDir;

const vec3 positions[8] = vec3[8](
        vec3(1.0, 1.0, -1.0),
        vec3(1.0, -1.0, -1.0),
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, -1.0, 1.0),
        vec3(-1.0, 1.0, -1.0),
        vec3(-1.0, -1.0, -1.0),
        vec3(-1.0, 1.0, 1.0),
        vec3(-1.0, -1.0, 1.0)
);

const int indices[36] = int[36](
        0, 2, 4,
        3, 7, 2,
        7, 5, 6,
        5, 7, 1,
        1, 3, 0,
        5, 1, 4,
        2, 6, 4,
        7, 6, 2,
        5, 4, 6,
        7, 3, 1,
        3, 2, 0,
        1, 0, 4
);

void main(void) {
    mat4 noTranslation = mat4(mat3(view));
    int index = indices[gl_VertexID];
    vec3 position = positions[index];
    vec4 pos = projection * noTranslation * vec4(position, 1.0);
    gl_Position = pos.xyww;
    textureDir = position;
}