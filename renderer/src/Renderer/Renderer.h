#pragma once
#include "RendererAPI.h"
#include "VertexArray.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Bottleneck {
    class Renderer {
        struct SceneData {
            glm::mat4 viewProjectionMatrix;
        };

        inline static SceneData* s_sceneData = new SceneData();

    public:
        static void BeginScene(const OrthographicCamera& camera); // set camera, lights, environments, etc
        static void EndScene();

        // Later it should take a shared ref to a mesh as parameter. Which would include the vertex array and the material (shader + uniforms)
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
}
