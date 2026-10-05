#include "Scene.h"

Scene::Scene(sf::Window& inWindow)
    : camera(), mainShader(), window(inWindow) {
}

Scene::~Scene() {
    clearMeshes();
    clearTextures();
}

void Scene::load(float width, float height) {
    mainShader = ShaderProgram::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
    const float aspect = width / height;
    camera.init(aspect);

    Texture vinceTexture;
    Texture brickTexture;
    vinceTexture.loadFromFile("assets/textures/vince.png");
    brickTexture.loadFromFile("assets/textures/brick1.jpg");
    textures.push_back(vinceTexture);
    textures.push_back(brickTexture);

    Mesh cube;
    Mesh pyramid;
    cube.loadFromFile("assets/meshes/Cube.obj");
    pyramid.loadFromFile("assets/meshes/Pyramid.obj");
    meshes.push_back(cube);
    meshes.push_back(pyramid);

    const glm::vec3 positions[] = {
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
        glm::vec3 position = positions[i] * 1.5f;

        Model model;
        model.init(i % 2 == 0 ? cube : pyramid, i % 3 == 0 ? vinceTexture : brickTexture);

        model.transform.setPosition(position);
        const float angle = 20.0f * static_cast<float>(i);
        glm::vec3 rotation = {glm::radians(angle), glm::radians(angle) * 0.3, glm::radians(angle) * 0.5f};
        model.transform.setRotation(rotation);
        models.push_back(model);
    }

    glClearColor(0.15f, 0.19f, 0.37f, 1.0f);
}

void Scene::input(const std::optional<sf::Event>& event) {
    if (const auto* resized = event->getIf<sf::Event::Resized>()) {
        const float aspect = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);
        camera.setAspectRatio(aspect);
        glViewport(0, 0, static_cast<GLsizei>(resized->size.x), static_cast<GLsizei>(resized->size.y));
    } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
        switch (key->scancode) {
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

void Scene::update(const float deltaTime) {
    camera.update();

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
}

void Scene::draw() {
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

void Scene::drawDebug() {
    camera.drawDebug();
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
