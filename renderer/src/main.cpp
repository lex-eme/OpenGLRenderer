#include <fstream>

#include <glad/glad.h>
#include <iostream>
#include <SOIL2.h>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SFML/Window/Context.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/Window.hpp"


#define numVAOs 1
float cameraX, cameraY, cameraZ;

ShaderProgram mainShader;
GLuint vao[numVAOs];

int width = 600, height = 600;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

std::stack<glm::mat4> mvStack;

Mesh sun;
Mesh earth;
Mesh moon;

GLuint loadTexture(const char* texImagePath) {
    const GLuint textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == 0) {
        std::cerr << "could not find texture file" << texImagePath << std::endl;
    }
    return textureID;
}

void setupVertices() {
    const float vertexPositions[108] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };

    const float pyramidPositions[54] =
    {
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
    };

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    sun.create(18, sizeof(pyramidPositions), pyramidPositions);
    earth.create(36, sizeof(vertexPositions), vertexPositions);
    moon.create(36, sizeof(vertexPositions), vertexPositions);

    sun.transform.setPosition(glm::vec3(0.0f));
    earth.transform.setPosition(glm::vec3(0.0f));
    moon.transform.setScale({0.25f, 0.25f, 0.25f});
}

void init() {
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 24.0f;
    setupVertices();
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
    sun.transform.setRotation(glm::vec3(currentTime, 0.0f, -currentTime));
    sun.draw(mainShader, mvStack.top());
    mvStack.pop();

    //-----------------------  cube == planet
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0, 1.0, 0.0));
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 0.0f, 0.0f));
    mvStack.push(mvStack.top());
    earth.transform.setRotation(glm::vec3(0.0f, currentTime, 0.0f));
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
            }
        }

        const float currentTime = clock.getElapsedTime().asSeconds();
        display(currentTime);

        window.display();
    }
}
