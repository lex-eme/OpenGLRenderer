#include "TestLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>
#include <glm/ext/matrix_transform.hpp>

#include "../dependencies/imgui/imgui.h"
#include "core/Core.h"
#include "platform/openGL/OpenGLTexture.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

TestLayer::TestLayer()
    : Layer("Example"), m_camera(-1.0f, 1.0f, -1.0f, 1.0f), m_cameraPosition(0.0f) {
    const std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec4 v_Color;
            out vec2 v_TexCoord;

			void main()
			{
                v_Color = a_Color;
                v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

    const std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

            uniform sampler2D u_Texture;

            in vec4 v_Color;
            in vec2 v_TexCoord;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

    m_vertexArray.reset(Bottleneck::VertexArray::Create());

    const float vertices[3 * 9] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f, 0.5f, 1.0f
    };

    Bottleneck::Ref<Bottleneck::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Bottleneck::VertexBuffer::Create(vertices, sizeof(vertices)));
    Bottleneck::BufferLayout layout = {
        {Bottleneck::ShaderDataType::Float3, "a_Position"},
        {Bottleneck::ShaderDataType::Float4, "a_Color"},
        {Bottleneck::ShaderDataType::Float2, "a_TexCoord"},
    };
    vertexBuffer->setLayout(layout);
    m_vertexArray->addVertexBuffer(vertexBuffer);

    const uint32_t indices[3] = {0, 1, 2};
    Bottleneck::Ref<Bottleneck::IndexBuffer> indexBuffer;
    indexBuffer.reset(Bottleneck::IndexBuffer::Create(indices, 3));
    m_vertexArray->setIndexBuffer(indexBuffer);

    m_shader.reset(Bottleneck::Shader::Create(vertexSrc, fragmentSrc));

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

void TestLayer::onUpdate(const Bottleneck::TimeStep timeStep) {
    if (Bottleneck::Input::IsKeyPressed(Key::Left)) {
        m_cameraPosition.x -= m_cameraSpeed * timeStep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::Right)) {
        m_cameraPosition.x += m_cameraSpeed * timeStep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::Down)) {
        m_cameraPosition.y -= m_cameraSpeed * timeStep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::Up)) {
        m_cameraPosition.y += m_cameraSpeed * timeStep;
    }

    if (Bottleneck::Input::IsKeyPressed(Key::J)) {
        m_trianglePosition.x -= m_cameraSpeed * timeStep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::L)) {
        m_trianglePosition.x += m_cameraSpeed * timeStep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::K)) {
        m_trianglePosition.y -= m_cameraSpeed * timeStep;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::I)) {
        m_trianglePosition.y += m_cameraSpeed * timeStep;
    }

    if (Bottleneck::Input::IsKeyPressed(Key::A)) {
        m_cameraRotation -= m_cameraRotationSpeed * timeStep;
    }

    if (Bottleneck::Input::IsKeyPressed(Key::D)) {
        m_cameraRotation += m_cameraRotationSpeed * timeStep;
    }

    Bottleneck::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Bottleneck::RenderCommand::Clear();

    m_camera.setPosition(m_cameraPosition);
    m_camera.setRotation(m_cameraRotation);

    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_trianglePosition);

    Bottleneck::Renderer::BeginScene(m_camera);
    Bottleneck::Renderer::Submit(m_shader, m_vertexArray, transform);
    Bottleneck::Renderer::EndScene();
}

void TestLayer::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(BN_BIND_EVENT_FN(TestLayer::onKeyPressedEvent));
}

void TestLayer::onImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Triangle color", glm::value_ptr(m_triangleColor));
    ImGui::End();
}

bool TestLayer::onKeyPressedEvent(const KeyPressedEvent& event) {
    return false;
}
