#include "SampleObjectScene.h"

#include "imgui/imgui.h"

void SampleObjectScene::load(GLFWwindow* inWindow, const float width, const float height) {
    window = inWindow;
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
    const float aspect = width / height;
    camera.init(aspect);

    Texture vinceTexture;
    vinceTexture.loadFromFile("assets/textures/vince.png");
    textures.push_back(vinceTexture);

    Sampler samplers[7];

    samplers[0].setMinFilter(FilterType::Nearest);
    samplers[1].setMinFilter(FilterType::Linear);
    samplers[2].setMinMipmapFilter(MipmapFilterType::NearestNearest);
    samplers[3].setMinMipmapFilter(MipmapFilterType::LinearNearest);
    samplers[4].setMinMipmapFilter(MipmapFilterType::NearestLinear);
    samplers[5].setMinMipmapFilter(MipmapFilterType::LinearLinear);
    samplers[6].setMinMipmapFilter(MipmapFilterType::LinearLinear);
    samplers[6].setAnisotropicFilter(16.0f);

    Mesh cube;
    cube.loadFromFile("assets/meshes/Cube.obj");
    meshes.push_back(cube);

    const glm::vec3 positions[] = {
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f),
    };

    for (int i = 0; i < 7; i += 1) {

        glm::vec3 position = positions[i] * 3.0f;

        Model model;
        model.init(cube, vinceTexture, Texture(), samplers[i]);

        model.transform.setPosition(position);
        models.push_back(model);
    }

    glClearColor(0.15f, 0.19f, 0.37f, 1.0f);
}

void SampleObjectScene::draw() const {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    mainShader.setActive();

    mainShader.setMat4Uniform("proj_matrix", camera.projectionMatrix);

    glEnable(GL_CULL_FACE);

    for (Model model: models) {
        model.draw(mainShader, camera.viewMatrix);
    }
}

void SampleObjectScene::drawDebug() {
    if (ImGui::DragFloat("Angle", &angle, 1.0f, -90.0f, 90.0f)) {
        for (Model& model : models) {
            model.transform.setRotation({0.0f, angle, 0.0f});
        }
    }
}
