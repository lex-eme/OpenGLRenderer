#include <glad/glad.h>

#include <fstream>
#include <iostream>

#include "imgui-SFML.h"
#include "imgui.h"
#include "scene/Scene.h"
#include "scene/SampleObjectScene.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Context.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/Window.hpp"

int width = 800, height = 600;

void displayDebug(sf::RenderWindow& debugWindow, const sf::Time& deltaTime, Scene& scene) {
    if (!debugWindow.isOpen()) {
        return;
    }

    debugWindow.setActive(true);


    while (const std::optional event = debugWindow.pollEvent()) {
        ImGui::SFML::ProcessEvent(debugWindow, *event);
        if (event->is<sf::Event::Closed>()) {
            debugWindow.close();
            return;
        }
    }
    ImGui::SFML::Update(debugWindow, deltaTime);

    //ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize({300, 300});
    ImGui::SetNextWindowPos({0, 0});
    ImGui::Begin("Hello, world!");
    scene.drawDebug();
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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    sf::Clock deltaClock;
    SampleObjectScene scene(window);
    scene.load(static_cast<float>(width), static_cast<float>(height));

    bool running = true;
    while (running) {
        while (const std::optional event = window.pollEvent()) {
            scene.input(event);

            if (event->is<sf::Event::Closed>()) {
                running = false;
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                switch (key->scancode) {
                    case sf::Keyboard::Scan::Escape:
                        running = false;
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
            }
        }

        window.setActive(true);
        const sf::Time deltaSfTime = deltaClock.restart();
        const float deltaTime = deltaSfTime.asSeconds();

        scene.update(deltaTime);
        scene.draw();
        window.display();

        displayDebug(debugWindow, deltaSfTime, scene);
    }

    ImGui::SFML::Shutdown();
}
