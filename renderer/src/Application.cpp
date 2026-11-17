#include "pch.h"

#include "Application.h"

#include <memory>
#include "Core.h"

#include "imgui/ImGuiLayer.h"

namespace Bottleneck {
    Application* Application::s_instance = nullptr;

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

        m_vertexArray.reset(VertexArray::Create());

        const float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
        vertexBuffer->setLayout(layout);
        m_vertexArray->addVertexBuffer(vertexBuffer);

        const uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, 3));
        m_vertexArray->setIndexBuffer(indexBuffer);

        m_shader.reset(Shader::Create(vertexSrc, fragmentSrc));
    }

    void Application::run() {
        while (m_running) {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->bind();
            m_vertexArray->bind();
            glDrawElements(GL_TRIANGLES, m_vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

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
