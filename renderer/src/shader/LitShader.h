#pragma once
#include "ShaderProgram.h"

#include <glad/glad.h>

#include "../light/DirectionalLight.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"


class LitShader final : public ShaderProgram {
    constexpr static int MAX_POINT_LIGHT_COUNT = 1;
    constexpr static int MAX_SPOT_LIGHT_COUNT = 1;

public:
    LitShader() = default;

    void setDirectionalLight(const DirectionalLight& dirLight) const;
    void setPointLights(const std::vector<PointLight>& pointLights) const;
    void setSpotLights(const std::vector<SpotLight>& spotLights) const;

    static LitShader createShaderProgram(const std::string& vertexShaderSourcePath,
                                         const std::string& fragmentShaderSourcePath);

private:
    explicit LitShader(GLuint ID);
};
