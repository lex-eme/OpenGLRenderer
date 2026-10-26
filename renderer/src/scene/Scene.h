#pragma once
#include "../AssetManager.h"
#include "../Camera.h"
#include "../FrameBuffer.h"
#include "../Model.h"
#include "../UniformBufferObject.h"
#include "../light/DirectionalLight.h"
#include "../shader/LitShader.h"
#include "GLFW/glfw3.h"

constexpr float cameraSpeed = 10.0f;

class Scene final {
public:
    inline static AssetManager assetManager;
    FrameBuffer frameBuffer{};

protected:
    std::vector<Model> models;
    Camera camera;
    LitShader litShader;
    UniformBufferObject cameraUbo{};
    ShaderProgram screenShader{};
    GLFWwindow* window = nullptr;
    std::vector<Mesh> meshes;
    std::vector<Texture> textures;
    float offset{0.003};

private:
    glm::vec3 pointLightPositions[4] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

    DirectionalLight directionalLight;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;

    glm::vec2 lastMousePos{};
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    bool rightButtonPressed = false;

public:
    Scene();
    virtual ~Scene();
    virtual void load(GLFWwindow* inWindow, int width, int height);

    virtual void keyPressed(int key);
    virtual void keyReleased(int key);
    virtual void mouseButtonPressed(int button);
    virtual void mouseButtonReleased(int button);
    virtual void windowResized(int width, int height);
    virtual void update(float deltaTime);
    virtual void draw();
    virtual void drawDebug();

private:
    void clearMeshes();
    void clearTextures();
};
