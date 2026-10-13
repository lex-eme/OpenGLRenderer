#pragma once
#include "../Camera.h"
#include "../Model.h"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Window.hpp"

constexpr float cameraSpeed = 10.0f;

class Scene {
protected:
    std::vector<Model> models;
    Camera camera;
    ShaderProgram mainShader;
    sf::Window& window;
    std::vector<Mesh> meshes;
    std::vector<Texture> textures;

private:
    glm::vec3 pointLightPositions[4] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    glm::vec3 lightPos{0.0f, 15.0f, -3.7f};
    glm::vec3 lightDir{0.0f, -1.0f, 0.0f};
    glm::vec3 lightAmbient{0.2f, 0.2f, 0.2f};
    glm::vec3 lightDiffuse{0.5f, 0.5f, 0.5f};
    glm::vec3 lightSpecular{1.0f, 1.0f, 1.0f};
    glm::vec3 spotAmbient{0.2f, 0.2f, 0.2f};
    glm::vec3 spotDiffuse{0.3f, 0.5f, 0.3f};
    glm::vec3 spotSpecular{0.8f, 1.0f, 0.8f};
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    float innerCutOff = 12.5f;
    float outerCutOff = 15.0f;

    float shininess = 32.0f;

    sf::Vector2i lastMousePos;
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    bool rightButtonPressed = false;

public:
    explicit Scene(sf::Window& inWindow);
    virtual ~Scene();
    virtual void load(float width, float height);

    virtual void input(const std::optional<sf::Event>& event);
    virtual void update(float deltaTime);
    virtual void draw() const;
    virtual void drawDebug();

private:
    void clearMeshes();
    void clearTextures();
};
