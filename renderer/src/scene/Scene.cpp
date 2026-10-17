#include "Scene.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.inl"

Scene::Scene()
    : mainShader(), litShader() {
}

Scene::~Scene() {
    clearMeshes();
    clearTextures();
}

void Scene::load(GLFWwindow* inWindow, const float width, const float height) {
    window = inWindow;
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/unlit.vert", "assets/shaders/unlit.frag");
    litShader = LitShader::createShaderProgram("assets/shaders/lit.vert", "assets/shaders/lit.frag");
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

    for (int i = 0; i < 4; i += 1) {
        pointLights.emplace_back();
    }

    for (int i = 0; i < 4; i += 1) {
        spotLights.emplace_back();
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Scene::keyPressed(const int key) {
    switch (key) {
        case GLFW_KEY_W:
            moveForward = true;
            break;
        case GLFW_KEY_S:
            moveBackward = true;
            break;
        case GLFW_KEY_A:
            moveLeft = true;
            break;
        case GLFW_KEY_D:
            moveRight = true;
            break;
        case GLFW_KEY_E:
            moveUp = true;
            break;
        case GLFW_KEY_Q:
            moveDown = true;
            break;
        default:
            break;
    }
}

void Scene::keyReleased(const int key) {
    switch (key) {
        case GLFW_KEY_W:
            moveForward = false;
            break;
        case GLFW_KEY_S:
            moveBackward = false;
            break;
        case GLFW_KEY_A:
            moveLeft = false;
            break;
        case GLFW_KEY_D:
            moveRight = false;
            break;
        case GLFW_KEY_E:
            moveUp = false;
            break;
        case GLFW_KEY_Q:
            moveDown = false;
            break;
        default:
            break;
    }
}

void Scene::mouseButtonPressed(const int button) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightButtonPressed = true;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        lastMousePos = lastMousePos = {x, y};;
    }
}

void Scene::mouseButtonReleased(const int button) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightButtonPressed = false;
    }
}

void Scene::windowResized(int width, int height) {
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    camera.setAspectRatio(aspect);
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
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
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec2 delta;
        delta.x = x - lastMousePos.x;
        delta.y = lastMousePos.y - y;
        lastMousePos = {x, y};
        camera.rotate(delta);
    }

    camera.update();
}

void Scene::draw() const {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    litShader.setActive();

    litShader.setFloatUniform("material.shininess", shininess);

    litShader.setDirectionalLight(directionalLight);
    litShader.setPointLights(pointLights);
    litShader.setSpotLights(spotLights);

    litShader.setVec3Uniform("viewPos", camera.position);
    litShader.setMat4Uniform("proj_matrix", camera.projectionMatrix);
    litShader.setMat4Uniform("view_matrix", camera.viewMatrix);

    glEnable(GL_CULL_FACE);

    for (Model model: models) {
        model.draw(litShader);
    }
}

void Scene::drawDebug() {
    camera.drawDebug();

    ImGui::SeparatorText("Light");
    directionalLight.drawDebug();
    int i = 0;
    for (PointLight& light: pointLights) {
        ImGui::PushID(i);
        light.drawDebug();
        ImGui::PopID();
        i += 1;
    }

    for (SpotLight& light: spotLights) {
        ImGui::PushID(i);
        light.drawDebug();
        ImGui::PopID();
        i += 1;
    }
    ImGui::SeparatorText("Material");
    ImGui::DragFloat("Shininess", &shininess, 1.0f, 2.0f, 128.0f);
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
