#include "TestLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>
#include <glm/ext/matrix_transform.hpp>

#include "../dependencies/imgui/imgui.h"
#include "Bottleneck/Renderer/RenderCommand.h"
#include "Bottleneck/Renderer/Renderer.h"

TestLayer::TestLayer()
    : Layer("Example"), m_cameraController(1.0f) {
    m_vertexArray = Bottleneck::VertexArray::Create();

    const float vertices[3 * 5] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f
    };

    Bottleneck::Ref<Bottleneck::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Bottleneck::VertexBuffer::Create(vertices, sizeof(vertices)));
    Bottleneck::BufferLayout layout = {
        {Bottleneck::ShaderDataType::Float3, "a_Position"},
        {Bottleneck::ShaderDataType::Float2, "a_TexCoord"},
    };
    vertexBuffer->setLayout(layout);
    m_vertexArray->addVertexBuffer(vertexBuffer);

    const uint32_t indices[3] = {0, 1, 2};
    Bottleneck::Ref<Bottleneck::IndexBuffer> indexBuffer;
    indexBuffer.reset(Bottleneck::IndexBuffer::Create(indices, 3));
    m_vertexArray->setIndexBuffer(indexBuffer);

    m_shader = m_shaderLibrary.load("assets/shaders/Texture.glsl");

    m_texture = Bottleneck::Texture2D::Create("assets/textures/vince.png");
    m_texture->bind(0);
    m_shader->uploadUniformInt("u_Texture", 0);
}

void TestLayer::onAttach() {
    BN_INFO("Layer {0} attached", m_debugName);
}

void TestLayer::onDetach() {
    BN_INFO("Layer {0} detached", m_debugName);
}

void TestLayer::onUpdate(const Bottleneck::Timestep timestep) {
    m_cameraController.onUpdate(timestep);
    if (Bottleneck::Input::IsKeyPressed(Key::J)) {
        m_trianglePosition.x -= m_triangleSpeed * timestep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::L)) {
        m_trianglePosition.x += m_triangleSpeed * timestep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::K)) {
        m_trianglePosition.y -= m_triangleSpeed * timestep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::I)) {
        m_trianglePosition.y += m_triangleSpeed * timestep;
    }

    Bottleneck::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Bottleneck::RenderCommand::Clear();

    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_trianglePosition);

    Bottleneck::Renderer::BeginScene(m_cameraController.getCamera());
    Bottleneck::Renderer::Submit(m_shader, m_vertexArray, transform);
    Bottleneck::Renderer::EndScene();
}

void TestLayer::onEvent(Event& event) {
    m_cameraController.onEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(BN_BIND_EVENT_FN(TestLayer::onKeyPressedEvent));
}

void TestLayer::onImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Triangle color", glm::value_ptr(m_triangleColor));
    ImGui::End();
}

bool TestLayer::onKeyPressedEvent(const KeyPressedEvent& event) {
    return false;
}
