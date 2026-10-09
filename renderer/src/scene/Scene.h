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
