#include "Scene.h"

#include "../material/ColorMaterial.h"
#include "../material/Material.h"
#include "../material/UnlitMaterial.h"
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
    cameraUbo.create();

    assetManager.addTexture("backpack_diffuse", "assets/models/backpack/diffuse.jpg");
    assetManager.addTexture("battery_diffuse", "assets/models/battery/Battery_Albedo.tga");

    const ShaderProgram unlitShader = ShaderProgram::createShaderProgram(
        "assets/shaders/unlit.vert", "assets/shaders/unlit.frag");
    unlitShader.bindUniformBufferObject("Matrices", cameraUbo.bindingPoint);

    auto* backpackMaterial = new UnlitMaterial();
    backpackMaterial->create(unlitShader, assetManager.getTexture("backpack_diffuse"));

    auto* batteryMaterial = new UnlitMaterial();
    batteryMaterial->create(unlitShader, assetManager.getTexture("battery_diffuse"));

    const ShaderProgram colorShader = ShaderProgram::createShaderProgram(
        "assets/shaders/singleColor.vert", "assets/shaders/singleColor.frag");
    colorShader.bindUniformBufferObject("Matrices", cameraUbo.bindingPoint);

    auto* colorMaterial = new ColorMaterial();
    colorMaterial->create(colorShader, glm::vec3(0.22f, 0.03f, 0.49f));

    auto* colorMaterial2 = new ColorMaterial();
    colorMaterial2->create(colorShader, glm::vec3(0.12f, 0.73f, 0.69f));

    models.emplace_back("assets/models/backpack/backpack.obj");
    for (int i = 0; i < models[0].materials.size(); i += 1) {
        models[0].materials[i] = backpackMaterial;
    }


    models.emplace_back("assets/models/battery/battery.fbx");
    models[1].transform.setPosition({3.0f, 0.0f, 0.0f});
    for (int i = 0; i < models[1].materials.size(); i += 1) {
        models[1].materials[i] = batteryMaterial;
    }


    models.emplace_back("assets/models/Test.fbx");
    models[2].transform.setPosition({-3.0f, 0.0f, 0.0f});
    models[2].transform.setScale({0.01f, 0.01f, 0.01f});
    models[2].materials[0] = colorMaterial;
    models[2].materials[1] = colorMaterial2;

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

    //program.setFloatUniform("material.shininess", shininess);

    // litShader.setDirectionalLight(directionalLight);
    // litShader.setPointLights(pointLights);
    // litShader.setSpotLights(spotLights);
    // litShader.setVec3Uniform("viewPos", camera.position);
    cameraUbo.setProjection(camera.projectionMatrix);
    cameraUbo.setView(camera.viewMatrix);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    for (Model& model: models) {
        model.draw();
    }
}

void Scene::drawDebug() {
    if (ImGui::BeginTabBar("TheTabBar")) {
        if (ImGui::BeginTabItem("Camera")) {
            camera.drawDebug();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Models")) {
            int i = 0;
            for (Model& model: models) {
                model.drawDebug(i);
                i += 1;
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Lights")) {
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
            ImGui::EndTabItem();
        }
    }

    ImGui::EndTabBar();
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
