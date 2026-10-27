#include "../pch.h"

#include "Scene.h"

#include "../material/BatteryMaterial.h"
#include "../material/ColorMaterial.h"
#include "../material/Material.h"
#include "../material/UnlitMaterial.h"


Scene::Scene()
    : litShader() {
}

Scene::~Scene() {
    clearMeshes();
    clearTextures();
}

void Scene::load(GLFWwindow* inWindow, const int width, const int height) {
    window = inWindow;
    litShader = LitShader::createShaderProgram("assets/shaders/lit.vert", "assets/shaders/lit.frag");
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    camera.init(aspect);
    cameraUbo.create();

    assetManager.addTexture("backpack_diffuse", "assets/models/backpack/diffuse.jpg");
    assetManager.addTexture("battery_diffuse", "assets/models/battery/Battery_Albedo.tga");
    assetManager.addTexture("battery_mre", "assets/models/battery/Battery_MetalRoughEmissive.tga");
    assetManager.addTexture("grass", "assets/textures/grass.png");

    screenShader = ShaderProgram::createShaderProgram("assets/shaders/postProcess.vert",
                                                      "assets/shaders/postProcess.frag");
    screenShader.setIntUniform("screenTexture", 0);

    const ShaderProgram unlitShader = ShaderProgram::createShaderProgram(
        "assets/shaders/unlit.vert", "assets/shaders/unlit.frag");
    unlitShader.bindUniformBufferObject("Matrices", cameraUbo.bindingPoint);

    auto* backpackMaterial = new UnlitMaterial();
    backpackMaterial->create(unlitShader, assetManager.getTexture("backpack_diffuse"));
    backpackMaterial->name = "M_Backpack";
    assetManager.addMaterial(backpackMaterial->name, backpackMaterial);

    const ShaderProgram batteryShader = ShaderProgram::createShaderProgram(
        "assets/shaders/unlit.vert", "assets/shaders/battery.frag");
    batteryShader.bindUniformBufferObject("Matrices", cameraUbo.bindingPoint);
    auto* batteryMaterial = new BatteryMaterial();
    batteryMaterial->create(batteryShader, assetManager.getTexture("battery_diffuse"),
                            assetManager.getTexture("battery_mre"));
    batteryMaterial->name = "M_Battery";
    assetManager.addMaterial(batteryMaterial->name, batteryMaterial);

    const ShaderProgram colorShader = ShaderProgram::createShaderProgram(
        "assets/shaders/singleColor.vert", "assets/shaders/singleColor.frag");
    colorShader.bindUniformBufferObject("Matrices", cameraUbo.bindingPoint);

    auto* colorMaterial = new ColorMaterial();
    colorMaterial->create(colorShader, glm::vec3(0.22f, 0.03f, 0.49f));
    colorMaterial->name = "M_PurpleColor";
    assetManager.addMaterial(colorMaterial->name, colorMaterial);

    auto* colorMaterial2 = new ColorMaterial();
    colorMaterial2->create(colorShader, glm::vec3(0.12f, 0.73f, 0.69f));
    colorMaterial2->name = "M_BlueColor";
    assetManager.addMaterial(colorMaterial2->name, colorMaterial2);

    models.emplace_back("assets/models/backpack/backpack.obj", false);
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

    std::vector<glm::vec3> vegetation;
    vegetation.emplace_back(-1.5f, 0.0f, -0.48f);
    vegetation.emplace_back(1.5f, 0.0f, 0.51f);
    vegetation.emplace_back(0.0f, 0.0f, 0.7f);
    vegetation.emplace_back(-0.3f, 0.0f, -2.3f);
    vegetation.emplace_back(0.5f, 0.0f, -0.6f);

    const ShaderProgram alphaClipShader = ShaderProgram::createShaderProgram(
        "assets/shaders/unlit.vert", "assets/shaders/alphaClip.frag");
    alphaClipShader.bindUniformBufferObject("Matrices", cameraUbo.bindingPoint);
    UnlitMaterial* grassMaterial = new UnlitMaterial();
    grassMaterial->create(alphaClipShader, assetManager.getTexture("grass"));
    grassMaterial->name = "M_Grass";
    assetManager.addMaterial(grassMaterial->name, grassMaterial);

    for (glm::vec3& position: vegetation) {
        models.emplace_back("assets/models/Plane.fbx");
        models[models.size() - 1].transform.setPosition(position);
        models[models.size() - 1].materials[0] = grassMaterial;
    }

    frameBuffer.create(width, height);

    grid.create(&camera, cameraUbo);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void Scene::windowResized(const int width, const int height) {
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    camera.setAspectRatio(aspect);
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    frameBuffer.resize(width, height);
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
    // litShader.setDirectionalLight(directionalLight);
    // litShader.setPointLights(pointLights);
    // litShader.setSpotLights(spotLights);
    // litShader.setVec3Uniform("viewPos", camera.position);

    cameraUbo.setProjection(camera.projectionMatrix);
    cameraUbo.setView(camera.viewMatrix);

    frameBuffer.bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    for (Model& model: models) {
        model.draw();
    }

    grid.draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::drawDebug() {
    if (ImGui::BeginTabBar("TheTabBar")) {
        if (ImGui::BeginTabItem("Camera")) {
            camera.drawDebug();

            ImGui::DragFloat("Offset", &offset, 0.0001f, 0, 0, "%6f");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Models")) {
            int i = 0;
            for (Model& model: models) {
                ImGui::PushID(i);
                model.drawDebug();
                ImGui::PopID();
                i += 1;
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Materials")) {
            int i = 0;
            for (Material* material: assetManager.getMaterialMap() | std::views::values) {
                ImGui::PushID(i);
                material->drawDebug();
                ImGui::PopID();
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
