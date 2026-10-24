#include "ColorMaterial.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.inl"

void ColorMaterial::bind() const {
    Material::bind();
    shader.setVec3Uniform("inColor", color);
}

void ColorMaterial::create(const ShaderProgram inShader, const glm::vec3 inColor) {
    shader = inShader;
    color = inColor;
}

void ColorMaterial::drawDebug() {
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_None)) {
        ImGui::Text("Name: %s", name.c_str());
        ImGui::ColorEdit3("Color", glm::value_ptr(color));
    }
}
