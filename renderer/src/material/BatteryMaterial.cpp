#include "BatteryMaterial.h"

#include <glad/glad.h>

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.inl"

void BatteryMaterial::bind() const {
    Material::bind();
    shader.setFloatUniform("material.chargeLevel", charge);
    shader.setVec3Uniform("material.chargeColor", chargeColor);

    shader.setIntUniform("material.albedo", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo.id);

    shader.setIntUniform("material.metalRoughEmissive", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, metalRoughEmissive.id);
}

void BatteryMaterial::create(const ShaderProgram inShader, const Texture inAlbedo, const Texture inMetalRoughEmissive) {
    shader = inShader;
    albedo = inAlbedo;
    metalRoughEmissive = inMetalRoughEmissive;
}

void BatteryMaterial::drawDebug() {
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_None)) {
        ImGui::Text("Name: %s", name.c_str());
        ImGui::DragFloat("Charge level", &charge, 0.01f, 0.0f, 1.0f);
        ImGui::ColorEdit3("Charge color", glm::value_ptr(chargeColor));
    }
}
