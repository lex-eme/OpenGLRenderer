#include "DirectionalLight.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.inl"


void DirectionalLight::drawDebug() {
    if (ImGui::CollapsingHeader("Directional light", ImGuiTreeNodeFlags_None)) {
        ImGui::DragFloat3("Direction", glm::value_ptr(direction), 0.01f);
        ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
    }
}
