#include <fstream>

#include "glad/glad.h"
#include <iostream>

#include <glm/glm.hpp>
#include "SFML/Window/Context.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/Window.hpp"


#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

float x = 0.0f;
float inc = 0.01f;

// builds and returns a translation matrix
glm::mat4 buildTranslate(float x, float y, float z) {
    glm::mat4 trans = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                x, y, z, 1.0);
    return trans;
}

// builds and returns a matrix that performs a rotation around the X axis
glm::mat4 buildRotateX(float rad) {
    glm::mat4 xrot = glm::mat4(1.0, 0.0, 0.0, 0.0,
                               0.0, cos(rad), -sin(rad), 0.0,
                               0.0, sin(rad), cos(rad), 0.0,
                               0.0, 0.0, 0.0, 1.0);
    return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
glm::mat4 buildRotateY(float rad) {
    glm::mat4 yrot = glm::mat4(cos(rad), 0.0, sin(rad), 0.0,
                               0.0, 1.0, 0.0, 0.0,
                               -sin(rad), 0.0, cos(rad), 0.0,
                               0.0, 0.0, 0.0, 1.0);
    return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
glm::mat4 buildRotateZ(float rad) {
    glm::mat4 zrot = glm::mat4(cos(rad), -sin(rad), 0.0, 0.0,
                               sin(rad), cos(rad), 0.0, 0.0,
                               0.0, 0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0);
    return zrot;
}

// builds and returns a scale matrix
glm::mat4 buildScale(float x, float y, float z) {
    glm::mat4 scale = glm::mat4(x, 0.0, 0.0, 0.0,
                                0.0, y, 0.0, 0.0,
                                0.0, 0.0, z, 0.0,
                                0.0, 0.0, 0.0, 1.0);
    return scale;
}

std::string readShaderSource(const char* filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    std::string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*) malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        free(log);
    }
}

void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*) malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        std::cout << "Program Info Log: " << log << std::endl;
        free(log);
    }
}

bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        std::cout << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

GLuint createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = readShaderSource("assets/shaders/vertShader.glsl");
    std::string fragShaderStr = readShaderSource("assets/shaders/fragShader.glsl");
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader, 1, &vertShaderSrc, nullptr);
    glShaderSource(fShader, 1, &fragShaderSrc, nullptr);
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        std::cout << "vertex compilation failed" << std::endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        std::cout << "fragment compilation failed" << std::endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        std::cout << "linking failed" << std::endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

void init() {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.85f, 0.65f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    x += inc;
    if (x > 1.0f) {
        inc = -0.01f;
    }

    if (x < -1.0f) {
        inc = 0.01f;
    }

    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"
    glProgramUniform1f(renderingProgram, offsetLoc, x);

    glPointSize(30.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    context_settings.majorVersion = 4;
    context_settings.minorVersion = 1;
    context_settings.attributeFlags = sf::ContextSettings::Attribute::Core;

    sf::Window window(sf::VideoMode({600, 600}), "OpenGL", sf::Style::Default, sf::State::Windowed, context_settings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        std::cerr << "Echec de l'initialisation de GLAD !" << std::endl;
        return -1;
    }
    std::cout << "Version OpenGL : " << GLVersion.major << "." << GLVersion.minor << std::endl;

    init();

    bool running = true;
    while (running) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                running = false;
            } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                glViewport(0, 0, static_cast<GLsizei>(resized->size.x), static_cast<GLsizei>(resized->size.y));
            }
        }

        display();

        window.display();
    }
}
