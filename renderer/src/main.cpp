#include <fstream>

#include <glad/glad.h>
#include <iostream>

#include <glm/glm.hpp>

#include "Camera.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "mesh/Mesh.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Context.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/Window.hpp"

ShaderProgram mainShader;

int width = 1280, height = 720;
std::vector<Model> models;
Camera camera;

void loadModels() {
    Texture vinceTexture;
    Texture brickTexture;
    vinceTexture.loadFromFile("assets/textures/vince.png");
    brickTexture.loadFromFile("assets/textures/brick1.jpg");

    Mesh cube;
    Mesh pyramid;
    cube.loadFromFile("assets/meshes/Cube.obj");
    pyramid.loadFromFile("assets/meshes/Pyramid.obj");

    const glm::vec3 cubePositions[] = {
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    for (int i = 0; i < 10; i += 1) {
        glm::vec3 position = cubePositions[i] * 1.5f;

        Model model;
        model.init(i % 2 == 0 ? cube : pyramid, i % 3 == 0 ? vinceTexture : brickTexture);

        model.transform.setPosition(position);
        const float angle = 20.0f * static_cast<float>(i);
        glm::vec3 rotation = {glm::radians(angle), glm::radians(angle) * 0.3, glm::radians(angle) * 0.5f};
        model.transform.setRotation(rotation);
        models.push_back(model);
    }
}

void init() {
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    camera.init(aspect);
    loadModels();
    glClearColor(0.15f, 0.19f, 0.37f, 1.0f);
}

void display(const float currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    mainShader.setActive();

    camera.update();
    mainShader.setMat4Uniform("proj_matrix", camera.projectionMatrix);

    glEnable(GL_CULL_FACE);

    for (Model model: models) {
        model.draw(mainShader, camera.viewMatrix);
    }
}

void displayDebug(sf::RenderWindow& debugWindow, const sf::Time& deltaTime) {
    if (!debugWindow.isOpen()) {
        return;
    }

    debugWindow.setActive(true);

    while (const std::optional event = debugWindow.pollEvent()) {
        ImGui::SFML::ProcessEvent(debugWindow, *event);
        if (event->is<sf::Event::Closed>()) {
            debugWindow.close();
        }
    }
    ImGui::SFML::Update(debugWindow, deltaTime);

    //ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize({300, 300});
    ImGui::SetNextWindowPos({0, 0});
    ImGui::Begin("Hello, world!");
    camera.drawDebug();
    ImGui::End();

    debugWindow.clear();
    ImGui::SFML::Render(debugWindow);
    debugWindow.display();
}

int main() {
    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    context_settings.majorVersion = 4;
    context_settings.minorVersion = 1;
    context_settings.attributeFlags = sf::ContextSettings::Attribute::Core;
    context_settings.antiAliasingLevel = 0;

    sf::RenderWindow debugWindow;
    debugWindow.setVerticalSyncEnabled(true);

    sf::Window window(sf::VideoMode({static_cast<unsigned>(width), static_cast<unsigned>(height)}), "OpenGL",
                      sf::Style::Default, sf::State::Windowed, context_settings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        std::cerr << "Failed to load glad" << std::endl;
        return -1;
    }
    std::cout << "Version OpenGL : " << GLVersion.major << "." << GLVersion.minor << std::endl;

    init();

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    const sf::Clock clock;
    sf::Clock deltaClock;

    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool rightButtonPressed = false;

    sf::Vector2i lastMousePos;

    bool running = true;
    while (running) {
        constexpr float cameraSpeed = 10.0f;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                running = false;
            } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                const float aspect = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);
                camera.setAspectRatio(aspect);
                glViewport(0, 0, static_cast<GLsizei>(resized->size.x), static_cast<GLsizei>(resized->size.y));
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                switch (key->scancode) {
                    case sf::Keyboard::Scan::Escape:
                        running = false;
                        break;
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
                    case sf::Keyboard::Scan::P:
                        if (debugWindow.isOpen()) {
                            debugWindow.close();
                        } else {
                            debugWindow.create(sf::VideoMode({static_cast<unsigned>(300), static_cast<unsigned>(300)}),
                                               "Debug",
                                               sf::Style::Default, sf::State::Windowed);
                            debugWindow.setPosition({
                                window.getPosition().x + static_cast<int>(window.getSize().x), window.getPosition().y
                            });
                            ImGui::SFML::Init(debugWindow);
                        }
                        break;
                    default:
                        break;
                }
            } else if (const auto* key = event->getIf<sf::Event::KeyReleased>()) {
                switch (key->scancode) {
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

        window.setActive(true);
        const sf::Time deltaSfTime = deltaClock.restart();
        const float deltaTime = deltaSfTime.asSeconds();

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

        if (rightButtonPressed) {
            glm::vec2 delta;
            delta.x = sf::Mouse::getPosition(window).x - lastMousePos.x;
            delta.y = lastMousePos.y - sf::Mouse::getPosition(window).y;
            lastMousePos = sf::Mouse::getPosition(window);
            camera.rotate(delta);
        }

        const float currentTime = clock.getElapsedTime().asSeconds();
        display(currentTime);

        window.display();

        displayDebug(debugWindow, deltaSfTime);
    }

    ImGui::SFML::Shutdown();
}
