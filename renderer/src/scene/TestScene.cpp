#include "TestScene.h"

TestScene::TestScene(sf::Window& inWindow)
    : Scene(inWindow) {
}

void TestScene::load(const float width, const float height) {
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
    const float aspect = width / height;
    camera.init(aspect);

    Texture vinceTexture;
    vinceTexture.loadFromFile("assets/textures/vince.png");
    textures.push_back(vinceTexture);

    Mesh cube;
    cube.loadFromFile("assets/meshes/Cube.obj");
    meshes.push_back(cube);

    const glm::vec3 positions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(-2.0f, 0.0f, 0.0f),
    };

    for (int i = 0; i < 5; i += 1) {

        glm::vec3 position = positions[i] * 3.5f;

        Model model;
        model.init(cube, vinceTexture);

        model.transform.setPosition(position);
        models.push_back(model);
    }

    glClearColor(0.15f, 0.19f, 0.37f, 1.0f);
}
