#include "Scene.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.inl"

Scene::Scene()
    : litShader() {
}

Scene::~Scene() {
    clearMeshes();
    clearTextures();
}

void Scene::load(GLFWwindow* inWindow, const float width, const float height) {
    window = inWindow;
    litShader = LitShader::createShaderProgram("assets/shaders/lit.vert", "assets/shaders/lit.frag");
    const float aspect = width / height;
    camera.init(aspect);

    model = Model("assets/models/backpack/backpack.obj");

    for (int i = 0; i < 1; i += 1) {
        pointLights.emplace_back();
    }

    for (int i = 0; i < 1; i += 1) {
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

void Scene::draw() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    litShader.setActive();

    litShader.setFloatUniform("material.shininess", shininess);

    litShader.setDirectionalLight(directionalLight);
    litShader.setPointLights(pointLights);
    litShader.setSpotLights(spotLights);

    litShader.setVec3Uniform("viewPos", camera.position);
    litShader.setMat4Uniform("proj_matrix", camera.projectionMatrix);
    litShader.setMat4Uniform("view_matrix", camera.viewMatrix);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    model.draw(litShader);
}

void Scene::drawDebug() {
    camera.drawDebug();

    static glm::vec3 position{0.0f};
    if (ImGui::DragFloat3("Mesh position", glm::value_ptr(position), 0.01f)) {
        model.transform.setPosition(position);
    }

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
