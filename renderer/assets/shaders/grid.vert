#version 410 core

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform vec3 cameraPos;

out vec2 uv;
out vec2 out_camPos;

float gridSize = 1000.0;

const vec3 positions[4] = vec3[4](
        vec3(-1.0, 0.0, -1.0),
        vec3( 1.0, 0.0, -1.0),
        vec3( 1.0, 0.0,  1.0),
        vec3(-1.0, 0.0,  1.0)
);

const int indices[6] = int[6](
        0, 1, 2, 2, 3, 0
);

void main(void) {
    int index = indices[gl_VertexID];
    vec3 position = positions[index] * gridSize;

    position.x += cameraPos.x;
    position.z += cameraPos.z;

    out_camPos = cameraPos.xz;

    gl_Position = projection * view * vec4(position, 1.0);
    uv = position.xz;
}
