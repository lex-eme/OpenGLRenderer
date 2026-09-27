#include <fstream>

#include <glad/glad.h>
#include <iostream>
#include <SOIL2.h>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SFML/Window/Context.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/Window.hpp"

float cameraX, cameraY, cameraZ;

ShaderProgram mainShader;

int width = 1280, height = 720;
glm::mat4 pMat, vMat;

std::stack<glm::mat4> mvStack;

Model sun;
Model earth;
Model moon;

GLuint loadTexture(const char* texImagePath) {
    const GLuint textureID =
            SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == 0) {
        std::cerr << "could not find texture file" << texImagePath << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    if (GL_EXT_texture_filter_anisotropic) {
        std::cout << "Anisotropic filtering extension supported" << std::endl;
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
    }
    return textureID;
}

void setupVertices() {
    const GLuint vinceTextureID = loadTexture("assets/textures/vince.png");
    const GLuint brickTextureID = loadTexture("assets/textures/brick1.jpg");

    Mesh cube;
    Mesh pyramid;

    cube.loadFromFile("assets/meshes/Cube.obj");
    pyramid.loadFromFile("assets/meshes/Pyramid.obj");

    sun.init(cube, vinceTextureID);
    earth.init(pyramid, vinceTextureID);
    moon.init(cube, brickTextureID);

    sun.transform.setScale({1.5f, 1.5f, 1.5f});
    earth.transform.setRotation({0.0f, 0.0f, 3.1415f});
    moon.transform.setScale({0.25f, 0.25f, 0.25f});
}

void init() {
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 14.0f;
    setupVertices();
    glClearColor(0.15f, 0.19f, 0.37f, 1.0f);
}

void display(const float currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    mainShader.setActive();

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(vMat);
    mainShader.setMat4Uniform("proj_matrix", pMat);

    glEnable(GL_CULL_FACE);

    // ----------------------  pyramid == sun
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    mvStack.push(mvStack.top());
    sun.transform.setRotation(glm::vec3(currentTime, 0.0f, 0.0f));
    sun.draw(mainShader, mvStack.top());
    mvStack.pop();

    //-----------------------  cube == planet
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0, 1.0, 0.0));
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 0.0f, 0.0f));
    mvStack.push(mvStack.top());
    earth.transform.setRotation(glm::vec3(0.0f, currentTime, 3.14159f));
    earth.draw(mainShader, mvStack.top());
    mvStack.pop();

    //-----------------------  smaller cube == moon
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0, 0.0, 1.0));
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    moon.transform.setRotation(glm::vec3(0.0, 0.0, currentTime));
    moon.draw(mainShader, mvStack.top());
    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
}

int main() {
    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    context_settings.majorVersion = 4;
    context_settings.minorVersion = 1;
    context_settings.attributeFlags = sf::ContextSettings::Attribute::Core;
    context_settings.antiAliasingLevel = 0;

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

    const sf::Clock clock;

    bool running = true;
    while (running) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                running = false;
            } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                const float aspect = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);
                pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
                glViewport(0, 0, static_cast<GLsizei>(resized->size.x), static_cast<GLsizei>(resized->size.y));
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                switch (key->scancode) {
                    case sf::Keyboard::Scan::Escape:
                        running = false;
                        break;
                    default:
                        break;
                }
            }
        }

        const float currentTime = clock.getElapsedTime().asSeconds();
        display(currentTime);

        window.display();
    }
}
