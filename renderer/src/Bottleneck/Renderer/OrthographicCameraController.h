#pragma once
#include "Bottleneck/Renderer/OrthographicCamera.h"
#include "Bottleneck/Core/Timestep.h"
#include "Bottleneck/Event/ApplicationEvent.h"
#include "Bottleneck/Event/MouseEvent.h"

namespace Bottleneck {
    class OrthographicCameraController {
        float m_aspectRatio;
        float m_zoomLevel = 1.0f;
        OrthographicCamera m_camera;

        bool m_rotation;
        glm::vec3 m_cameraPosition{0.0f};
        float m_cameraTranslationSpeed{1.0f};

        float m_cameraRotation{0.0f};
        float m_cameraRotationSpeed{90.0f};

    public:
        explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

        void onUpdate(Timestep timestep);
        void onEvent(Event& event);

        OrthographicCamera& getCamera() { return m_camera; }
        const OrthographicCamera& getCamera() const { return m_camera; }

    private:
        bool onMouseScrolled(const MouseScrolledEvent& event);
        bool onWindowResized(const WindowResizeEvent& event);
    };
}
