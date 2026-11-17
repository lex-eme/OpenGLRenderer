#pragma once
#include "RendererAPI.h"
#include "VertexArray.h"

namespace Bottleneck {
    class Renderer {
    public:
        static void BeginScene(); // set camera, lights, environments, etc
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
}
