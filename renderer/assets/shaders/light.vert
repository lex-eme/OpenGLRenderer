#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStrength;

out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void) {
    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);
    textureCoord = texCoord;
    fragPos = vec3(model_matrix * vec4(position, 1.0));
    normal = aNormal;
}

mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            x, y, z, 1.0);
    return trans;
}

//  rotation around the X axis
mat4 buildRotateX(float rad)
{
    mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
            0.0, cos(rad), -sin(rad), 0.0,
            0.0, sin(rad), cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0);
    return xrot;
}

//  rotation around the Y axis
mat4 buildRotateY(float rad) {
    mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
            0.0, 1.0, 0.0, 0.0,
            -sin(rad), 0.0, cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0);
    return yrot;
}

//  rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), sin(rad), 0.0, 0.0,
            -sin(rad), cos(rad), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);
    return zrot;
}