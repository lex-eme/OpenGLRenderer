#include "Scene.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.inl"

Scene::Scene(sf::Window& inWindow)
    : mainShader(), window(inWindow) {
}

Scene::~Scene() {
    clearMeshes();
    clearTextures();
}

void Scene::load(const float width, const float height) {
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

    const glm::vec3 positions[] = {
        glm::vec3(0.0f, -5.0f, 0.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    Model ground;
    ground.init(cube, diffuseTex, specularTex);
    ground.transform.setPosition({0.0f, -5.0f, 0.0f});
    ground.transform.setScale({50.0f, 1.0f, 50.0f});
    models.push_back(ground);

    for (int i = 0; i < 10; i += 1) {
        Model model;
        model.init(cube, diffuseTex, specularTex);

        glm::vec3 position = positions[i] * 1.5f;
        model.transform.setPosition(position);
        const float angle = 20.0f * static_cast<float>(i);
        glm::vec3 rotation = {angle, angle * 0.3, angle * 0.5f};
        model.transform.setRotation(rotation);
        models.push_back(model);
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Scene::input(const std::optional<sf::Event>& event) {
    if (const auto* resized = event->getIf<sf::Event::Resized>()) {
        const float aspect = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);
        camera.setAspectRatio(aspect);
        glViewport(0, 0, static_cast<GLsizei>(resized->size.x), static_cast<GLsizei>(resized->size.y));
    } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->scancode) {
            case sf::Keyboard::Scan::W:
                moveForward = true;
                break;
            case sf::Keyboard::Scan::S:
                moveBackward = true;
                break;
            case sf::Keyboard::Scan::A:
                moveLeft = true;
                break;
            case sf::Keyboard::Scan::D:
                moveRight = true;
                break;
            case sf::Keyboard::Scan::E:
                moveUp = true;
                break;
            case sf::Keyboard::Scan::Q:
                moveDown = true;
                break;
            default:
                break;
        }
    } else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
        switch (keyReleased->scancode) {
            case sf::Keyboard::Scan::W:
                moveForward = false;
                break;
            case sf::Keyboard::Scan::S:
                moveBackward = false;
                break;
            case sf::Keyboard::Scan::A:
                moveLeft = false;
                break;
            case sf::Keyboard::Scan::D:
                moveRight = false;
                break;
            case sf::Keyboard::Scan::E:
                moveUp = false;
                break;
            case sf::Keyboard::Scan::Q:
                moveDown = false;
                break;
            default:
                break;
        }
    } else if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (pressed->button == sf::Mouse::Button::Right) {
            rightButtonPressed = true;
            lastMousePos = pressed->position;
        }
    } else if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Right) {
            rightButtonPressed = false;
        }
    }
}

void Scene::update(const float deltaTime) {
    if (moveForward) {
        camera.move(cameraSpeed * camera.getForward() * deltaTime);
    }

    if (moveBackward) {
        camera.move(-cameraSpeed * camera.getForward() * deltaTime);
    }

    if (moveLeft) {
        camera.move(camera.getRight() * -cameraSpeed * deltaTime);
    }

    if (moveRight) {
        camera.move(camera.getRight() * cameraSpeed * deltaTime);
    }

    if (moveUp) {
        camera.move(glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed * deltaTime);
    }

    if (moveDown) {
        camera.move(glm::vec3(0.0f, -1.0f, 0.0f) * cameraSpeed * deltaTime);
    }

    if (rightButtonPressed) {
        glm::vec2 delta;
        delta.x = sf::Mouse::getPosition(window).x - lastMousePos.x;
        delta.y = lastMousePos.y - sf::Mouse::getPosition(window).y;
        lastMousePos = sf::Mouse::getPosition(window);
        camera.rotate(delta);
    }

    camera.update();
}

void Scene::draw() const {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    mainShader.setActive();

    mainShader.setFloatUniform("material.shininess", shininess);

    mainShader.setVec3Uniform("dirLight.direction", lightDir);
    mainShader.setVec3Uniform("dirLight.ambient", lightAmbient);
    mainShader.setVec3Uniform("dirLight.diffuse", lightDiffuse);
    mainShader.setVec3Uniform("dirLight.specular", lightSpecular);

    mainShader.setVec3Uniform("pointLights[0].position", pointLightPositions[0]);
    mainShader.setVec3Uniform("pointLights[0].ambient", lightAmbient);
    mainShader.setVec3Uniform("pointLights[0].diffuse", lightDiffuse);
    mainShader.setVec3Uniform("pointLights[0].specular", lightSpecular);
    mainShader.setFloatUniform("pointLights[0].constant", constant);
    mainShader.setFloatUniform("pointLights[0].linear", linear);
    mainShader.setFloatUniform("pointLights[0].quadratic", quadratic);

    mainShader.setVec3Uniform("pointLights[1].position", pointLightPositions[1]);
    mainShader.setVec3Uniform("pointLights[1].ambient", lightAmbient);
    mainShader.setVec3Uniform("pointLights[1].diffuse", lightDiffuse);
    mainShader.setVec3Uniform("pointLights[1].specular", lightSpecular);
    mainShader.setFloatUniform("pointLights[1].constant", constant);
    mainShader.setFloatUniform("pointLights[1].linear", linear);
    mainShader.setFloatUniform("pointLights[1].quadratic", quadratic);

    mainShader.setVec3Uniform("pointLights[2].position", pointLightPositions[2]);
    mainShader.setVec3Uniform("pointLights[2].ambient", lightAmbient);
    mainShader.setVec3Uniform("pointLights[2].diffuse", lightDiffuse);
    mainShader.setVec3Uniform("pointLights[2].specular", lightSpecular);
    mainShader.setFloatUniform("pointLights[2].constant", constant);
    mainShader.setFloatUniform("pointLights[2].linear", linear);
    mainShader.setFloatUniform("pointLights[2].quadratic", quadratic);

    mainShader.setVec3Uniform("pointLights[3].position", pointLightPositions[3]);
    mainShader.setVec3Uniform("pointLights[3].ambient", lightAmbient);
    mainShader.setVec3Uniform("pointLights[3].diffuse", lightDiffuse);
    mainShader.setVec3Uniform("pointLights[3].specular", lightSpecular);
    mainShader.setFloatUniform("pointLights[3].constant", constant);
    mainShader.setFloatUniform("pointLights[3].linear", linear);
    mainShader.setFloatUniform("pointLights[3].quadratic", quadratic);

    mainShader.setVec3Uniform("spotLight.position", lightPos);
    mainShader.setVec3Uniform("spotLight.direction", lightDir);
    mainShader.setVec3Uniform("spotLight.ambient", spotAmbient);
    mainShader.setVec3Uniform("spotLight.diffuse", spotDiffuse);
    mainShader.setVec3Uniform("spotLight.specular", spotSpecular);
    mainShader.setFloatUniform("spotLight.constant", constant);
    mainShader.setFloatUniform("spotLight.linear", linear);
    mainShader.setFloatUniform("spotLight.quadratic", quadratic);
    mainShader.setFloatUniform("spotLight.cutOff", innerCutOff);
    mainShader.setFloatUniform("spotLight.outerCutOff", outerCutOff);

    mainShader.setVec3Uniform("viewPos", camera.position);
    mainShader.setMat4Uniform("proj_matrix", camera.projectionMatrix);
    mainShader.setMat4Uniform("view_matrix", camera.viewMatrix);

    glEnable(GL_CULL_FACE);

    for (Model model: models) {
        model.draw(mainShader);
    }
}

void Scene::drawDebug() {
    camera.drawDebug();

    ImGui::SeparatorText("Light");
    ImGui::PushID(1);
    ImGui::DragFloat3("Position1", glm::value_ptr(pointLightPositions[0]), 0.1f);
    ImGui::DragFloat3("Position2", glm::value_ptr(pointLightPositions[1]), 0.1f);
    ImGui::DragFloat3("Position3", glm::value_ptr(pointLightPositions[2]), 0.1f);
    ImGui::DragFloat3("Position4", glm::value_ptr(pointLightPositions[3]), 0.1f);
    ImGui::DragFloat3("Direction", glm::value_ptr(lightDir), 0.01f);
    ImGui::ColorEdit3("Ambient", glm::value_ptr(lightAmbient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(lightDiffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(lightSpecular));
    ImGui::DragFloat3("Attributes", &constant, 0.0001f, 0.0000001f, 2.0f, "%.6f");
    ImGui::DragFloat3("SpotPos", glm::value_ptr(lightPos), 0.1f);
    ImGui::DragFloat("Inner cutoff", &innerCutOff, 0.1f);
    ImGui::DragFloat("Outer cutoff", &outerCutOff, 0.1f);
    ImGui::PopID();

    ImGui::PushID(2);
    ImGui::SeparatorText("Material");
    ImGui::DragFloat("Shininess", &shininess, 1.0f, 2.0f, 128.0f);
    ImGui::PopID();
}

void Scene::clearMeshes() {
    for (Mesh& mesh: meshes) {
        mesh.unload();
    }
}

void Scene::clearTextures() {
    for (Texture& texture: textures) {
        texture.unload();
    }
}
