#include "TestLayer.h"

#include "Core.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

TestLayer::TestLayer()
    : Layer("Example"), m_camera(-1.0f, 1.0f, -1.0f, 1.0f), m_cameraPosition(0.0f) {
    const std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

			out vec3 v_Position;
            out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
                v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

    const std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
            in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

    m_vertexArray.reset(Bottleneck::VertexArray::Create());

    const float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };

    std::shared_ptr<Bottleneck::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Bottleneck::VertexBuffer::Create(vertices, sizeof(vertices)));
    Bottleneck::BufferLayout layout = {
        {Bottleneck::ShaderDataType::Float3, "a_Position"},
        {Bottleneck::ShaderDataType::Float4, "a_Color"},
    };
    vertexBuffer->setLayout(layout);
    m_vertexArray->addVertexBuffer(vertexBuffer);

    const uint32_t indices[3] = {0, 1, 2};
    std::shared_ptr<Bottleneck::IndexBuffer> indexBuffer;
    indexBuffer.reset(Bottleneck::IndexBuffer::Create(indices, 3));
    m_vertexArray->setIndexBuffer(indexBuffer);

    m_shader.reset(Bottleneck::Shader::Create(vertexSrc, fragmentSrc));
}

void TestLayer::onAttach() {
    BN_INFO("Layer {0} attached", m_debugName);
}

void TestLayer::onDetach() {
    BN_INFO("Layer {0} detached", m_debugName);
}

void TestLayer::onUpdate() {
    if (Bottleneck::Input::IsKeyPressed(Key::Left)) {
        m_cameraPosition.x -= m_cameraSpeed;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::Right)) {
        m_cameraPosition.x += m_cameraSpeed;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::Down)) {
        m_cameraPosition.y -= m_cameraSpeed;
    }
    if (Bottleneck::Input::IsKeyPressed(Key::Up)) {
        m_cameraPosition.y += m_cameraSpeed;
    }

    if (Bottleneck::Input::IsKeyPressed(Key::A)) {
        m_cameraRotation -= m_cameraRotationSpeed;
    }

    if (Bottleneck::Input::IsKeyPressed(Key::D)) {
        m_cameraRotation += m_cameraRotationSpeed;
    }

    Bottleneck::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Bottleneck::RenderCommand::Clear();

    m_camera.setPosition(m_cameraPosition);
    m_camera.setRotation(m_cameraRotation);

    Bottleneck::Renderer::BeginScene(m_camera);
    Bottleneck::Renderer::Submit(m_shader, m_vertexArray);
    Bottleneck::Renderer::EndScene();
}

void TestLayer::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(BN_BIND_EVENT_FN(TestLayer::onKeyPressedEvent));
}

void TestLayer::onImGuiRender() {
}

bool TestLayer::onKeyPressedEvent(const KeyPressedEvent& event) {
    return false;
}
