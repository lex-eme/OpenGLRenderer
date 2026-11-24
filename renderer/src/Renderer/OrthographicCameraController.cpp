#include "pch.h"

#include "OrthographicCameraController.h"

#include "core/Core.h"
#include "core/Input.h"

namespace Bottleneck {
    OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool rotation)
        : m_aspectRatio(aspectRatio),
          m_camera(-aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel),
          m_rotation(rotation) {
    }

    void OrthographicCameraController::onUpdate(const Timestep timestep) {
        if (Input::IsKeyPressed(Key::A)) {
            m_cameraPosition.x -= m_cameraTranslationSpeed * timestep;
        }
        if (Input::IsKeyPressed(Key::D)) {
            m_cameraPosition.x += m_cameraTranslationSpeed * timestep;
        }
        if (Input::IsKeyPressed(Key::S)) {
            m_cameraPosition.y -= m_cameraTranslationSpeed * timestep;
        }
        if (Input::IsKeyPressed(Key::W)) {
            m_cameraPosition.y += m_cameraTranslationSpeed * timestep;
        }

        if (m_rotation) {
            if (Input::IsKeyPressed(Key::Q)) {
                m_cameraRotation -= m_cameraRotationSpeed * timestep;
            }
            if (Input::IsKeyPressed(Key::E)) {
                m_cameraRotation += m_cameraRotationSpeed * timestep;
            }

            m_camera.setRotation(m_cameraRotation);
        }

        m_camera.setPosition(m_cameraPosition);
    }

    void OrthographicCameraController::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseScrolledEvent>(BN_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(BN_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
    }

    bool OrthographicCameraController::onMouseScrolled(const MouseScrolledEvent& event) {
        m_zoomLevel -= event.getYOffset() * 0.25f;
        m_zoomLevel = std::max(m_zoomLevel, 0.25f);
        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        m_cameraTranslationSpeed = m_zoomLevel;
        return false;
    }

    bool OrthographicCameraController::onWindowResized(const WindowResizeEvent& event) {
        m_aspectRatio = static_cast<float>(event.getWidth()) / static_cast<float>(event.getHeight());
        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        BN_CORE_INFO("Camera resized");
        return false;
    }
}
