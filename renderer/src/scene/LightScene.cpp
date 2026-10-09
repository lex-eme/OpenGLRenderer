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

    Texture brickTexture;
    brickTexture.loadFromFile("assets/textures/brick1.jpg");
    textures.push_back(brickTexture);

    Mesh cube;
    cube.loadFromFile("assets/meshes/Cube.obj");
    meshes.push_back(cube);

    Model box;
    box.init(cube, brickTexture, Sampler());
    box.transform.setPosition(glm::vec3(0.0f));
    models.push_back(box);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void LightScene::draw() const {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    mainShader.setActive();

    mainShader.setVec3Uniform("objectColor", objectColor);
    mainShader.setVec3Uniform("lightColor", lightColor);
    mainShader.setVec3Uniform("lightPos", lightPos);
    mainShader.setVec3Uniform("viewPos", camera.position);
    mainShader.setFloatUniform("ambientStrength", ambientStrength);
    mainShader.setMat4Uniform("proj_matrix", camera.projectionMatrix);
    mainShader.setMat4Uniform("view_matrix", camera.viewMatrix);

    glEnable(GL_CULL_FACE);

    for (Model model: models) {
        model.draw(mainShader);
    }
}

void LightScene::drawDebug() {
    ImGui::ColorEdit3("Object color", glm::value_ptr(objectColor));

    ImGui::DragFloat3("Light position", glm::value_ptr(lightPos), 0.1f);
    ImGui::ColorEdit3("Light color", glm::value_ptr(lightColor));

    ImGui::SliderFloat("Ambient strength", &ambientStrength, 0.0f, 1.0f);
}
