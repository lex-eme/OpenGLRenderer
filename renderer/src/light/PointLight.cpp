#include "../pch.h"

#include "PointLight.h"


void PointLight::drawDebug() {
    if (ImGui::CollapsingHeader("Point light", ImGuiTreeNodeFlags_None)) {
        ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01f);
        ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
        ImGui::DragFloat3("Attributes", &constant, 0.0001f, 0.0000001f, 2.0f, "%.6f");
    }
}
