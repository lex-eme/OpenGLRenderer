#version 410 core

layout (location = 0) in vec3 position;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec3 worldSpacePos;

uniform vec3 cameraPos;
const float gridScale = 50.0;

void main(void) {
    vec3 localPos = vec3(position.x * gridScale, 0.0, position.z * gridScale);
    vec3 snappedWorldPos = localPos + vec3(cameraPos.x, 0.0, cameraPos.z);

    worldSpacePos = snappedWorldPos;
    gl_Position = projection * view * vec4(snappedWorldPos, 1.0);
}
