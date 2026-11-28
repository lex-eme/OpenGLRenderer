#include "pch.h"

#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Bottleneck {
    struct Renderer2DStorage {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init() {
        BN_PROFILE_FUNCTION();

        assert(s_Data == nullptr);
        s_Data = new Renderer2DStorage();

        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->setLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        s_Data->QuadVertexArray->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->setIndexBuffer(squareIB);

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

        s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data->TextureShader->setInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown() {
        BN_PROFILE_FUNCTION();

        delete s_Data;
        s_Data = nullptr;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        BN_PROFILE_FUNCTION();

        s_Data->TextureShader->bind();
        s_Data->TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {
        BN_PROFILE_FUNCTION();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        BN_PROFILE_FUNCTION();

        s_Data->TextureShader->setFloat4("u_Color", color);
        s_Data->TextureShader->setFloat("u_TilingFactor", 1.0f);
        s_Data->WhiteTexture->bind(0);
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
                                        glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->TextureShader->setMat4("u_Transform", transform);

        s_Data->QuadVertexArray->bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture,
                              const float tilingFactor, const glm::vec4& tintColor) {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture,
                              const float tilingFactor, const glm::vec4& tintColor) {
        BN_PROFILE_FUNCTION();

        s_Data->TextureShader->bind();
        s_Data->TextureShader->setFloat4("u_Color", tintColor);
        s_Data->TextureShader->setFloat("u_TilingFactor", tilingFactor);
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
                                        glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->TextureShader->setMat4("u_Transform", transform);

        texture->bind(0);

        s_Data->QuadVertexArray->bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation,
                                     const glm::vec4& color) {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color) {
        BN_PROFILE_FUNCTION();

        s_Data->TextureShader->setFloat4("u_Color", color);
        s_Data->TextureShader->setFloat("u_TilingFactor", 1.0f);
        s_Data->WhiteTexture->bind(0);
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                                    * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
                                    * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->TextureShader->setMat4("u_Transform", transform);

        s_Data->QuadVertexArray->bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation,
                                     const Ref<Texture>& texture, const float tilingFactor,
                                     const glm::vec4& tintColor) {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation,
                                     const Ref<Texture>& texture, const float tilingFactor,
                                     const glm::vec4& tintColor) {
        BN_PROFILE_FUNCTION();

        s_Data->TextureShader->bind();
        s_Data->TextureShader->setFloat4("u_Color", tintColor);
        s_Data->TextureShader->setFloat("u_TilingFactor", tilingFactor);
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                                    * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
                                    * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->TextureShader->setMat4("u_Transform", transform);

        texture->bind(0);

        s_Data->QuadVertexArray->bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}
