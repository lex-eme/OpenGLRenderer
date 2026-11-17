#include "pch.h"

#include "Application.h"

#include <memory>
#include "Core.h"

#include "imgui/ImGuiLayer.h"

namespace Bottleneck {
    Application* Application::s_instance = nullptr;

    static GLenum shaderDataTypeToOpenGLBaseType(const ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
            default:
                assert(false);
        }
    }

    Application::Application() {
        assert(s_instance == nullptr);
        s_instance = this;
        m_window = std::unique_ptr<Window>(Window::create());
        m_window->setEventCallback(BN_BIND_EVENT_FN(Application::onEvent));

        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);

        const std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
            out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
                v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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

        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        const float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            BufferLayout layout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"},
            };

            m_vertexBuffer->setLayout(layout);
        }

        uint32_t index = 0;
        const auto& layout = m_vertexBuffer->getLayout();
        for (const auto& element: layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.getComponentCount(), shaderDataTypeToOpenGLBaseType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE, layout.getStride(),
                                  reinterpret_cast<const void*>(element.offset));
            index += 1;
        }

        const uint32_t indices[3] = {0, 1, 2};
        m_indexBuffer.reset(IndexBuffer::Create(indices, 3));
        m_shader.reset(Shader::Create(vertexSrc, fragmentSrc));

        glBindVertexArray(0);
        glDisable(GL_CULL_FACE);
    }

    void Application::run() {
        while (m_running) {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->bind();
            glBindVertexArray(m_vertexArray);
            glDrawElements(GL_TRIANGLES, static_cast<int>(m_indexBuffer->getCount()), GL_UNSIGNED_INT, nullptr);

            for (Layer* layer: m_layerStack) {
                layer->onUpdate();
            }

            m_imGuiLayer->begin();
            for (Layer* layer: m_layerStack) {
                layer->onImGuiRender();
            }
            m_imGuiLayer->end();

            m_window->onUpdate();
        }
    }

    void Application::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BN_BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BN_BIND_EVENT_FN(Application::onKeyPress));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it += 1) {
            if (event.handled) {
                break;
            }
            (*it)->onEvent(event);
        }
    }

    void Application::pushLayer(Layer* layer) {
        m_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* layer) {
        m_layerStack.pushOverlay(layer);
        layer->onAttach();
    }

    bool Application::onWindowClose(WindowCloseEvent& event) {
        m_running = false;
        return true;
    }

    bool Application::onKeyPress(const KeyPressedEvent& event) {
        switch (event.getKeyCode()) {
            case Key::Escape:
                m_running = false;
                return true;
            default: break;
        }

        return false;
    }
}
