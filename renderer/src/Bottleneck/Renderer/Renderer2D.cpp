#include "pch.h"

#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Bottleneck {
    struct Renderer2DStorage {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init() {
        assert(s_Data == nullptr);
        s_Data = new Renderer2DStorage();

        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[4 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->setLayout({
            {ShaderDataType::Float3, "a_Position"}
        });
        s_Data->QuadVertexArray->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->setIndexBuffer(squareIB);

        s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
        s_Data->FlatColorShader->setMat4("u_Transform", glm::mat4(1.0f));
    }

    void Renderer2D::Shutdown() {
        delete s_Data;
        s_Data = nullptr;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        s_Data->FlatColorShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        s_Data->FlatColorShader->bind();
        s_Data->FlatColorShader->setFloat4("u_Color", color);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->FlatColorShader->setMat4("u_Transform", transform);

        s_Data->QuadVertexArray->bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}
