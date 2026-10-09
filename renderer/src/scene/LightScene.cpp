#include "LightScene.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.inl"

LightScene::LightScene(sf::Window& inWindow)
    : Scene(inWindow) {
}

void LightScene::load(const float width, const float height) {
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/light.vert", "assets/shaders/light.frag");
    const float aspect = width / height;
    camera.init(aspect);

    Texture diffuseTex;
    diffuseTex.loadFromFile("assets/textures/container.png");
    textures.push_back(diffuseTex);
    Texture specularTex;
    specularTex.loadFromFile("assets/textures/container_specular.png");
    textures.push_back(specularTex);

    Mesh cube;
    cube.loadFromFile("assets/meshes/Cube.obj");
    meshes.push_back(cube);

    Model box;
    box.init(cube, diffuseTex, specularTex);
    box.transform.setPosition(glm::vec3(0.0f));
    models.push_back(box);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void LightScene::draw() const {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    mainShader.setActive();

    mainShader.setFloatUniform("material.shininess", shininess);

    mainShader.setVec3Uniform("light.position", lightPos);
    mainShader.setVec3Uniform("light.ambient", lightAmbient);
    mainShader.setVec3Uniform("light.diffuse", lightDiffuse);
    mainShader.setVec3Uniform("light.specular", lightSpecular);

    mainShader.setVec3Uniform("viewPos", camera.position);
    mainShader.setMat4Uniform("proj_matrix", camera.projectionMatrix);
    mainShader.setMat4Uniform("view_matrix", camera.viewMatrix);

    glEnable(GL_CULL_FACE);

    for (Model model: models) {
        model.draw(mainShader);
    }
}

void LightScene::drawDebug() {
    ImGui::SeparatorText("Light");
    ImGui::PushID(1);
    ImGui::DragFloat3("Position", glm::value_ptr(lightPos), 0.1f);
    ImGui::ColorEdit3("Ambient", glm::value_ptr(lightAmbient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(lightDiffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(lightSpecular));
    ImGui::PopID();

    ImGui::PushID(2);
    ImGui::SeparatorText("Material");
    ImGui::DragFloat("Shininess", &shininess, 1.0f, 2.0f, 128.0f);
    ImGui::PopID();
}
