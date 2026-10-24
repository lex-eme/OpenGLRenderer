set(SOURCES
        src/main.cpp
        src/glad.c
        src/shader/ShaderProgram.cpp
        src/shader/ShaderProgram.h
        src/Transform.cpp
        src/Transform.h
        src/mesh/Mesh.cpp
        src/mesh/Mesh.h
        src/mesh/Sphere.cpp
        src/mesh/Sphere.h
        src/mesh/Torus.cpp
        src/mesh/Torus.h
        src/Model.cpp
        src/Model.h
        src/Image.cpp
        src/Image.h
        src/Texture.cpp
        src/Texture.h
        src/Camera.cpp
        src/Camera.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/Sampler.cpp
        src/Sampler.h
        src/light/Light.cpp
        src/light/Light.h
        src/light/DirectionalLight.cpp
        src/light/DirectionalLight.h
        src/light/PointLight.cpp
        src/light/PointLight.h
        src/shader/LitShader.cpp
        src/shader/LitShader.h
        src/light/SpotLight.cpp
        src/light/SpotLight.h
        src/AssetManager.cpp
        src/AssetManager.h
        src/material/Material.cpp
        src/material/Material.h
        src/UniformBufferObject.cpp
        src/UniformBufferObject.h
        src/material/UnlitMaterial.cpp
        src/material/UnlitMaterial.h
        src/material/ColorMaterial.cpp
        src/material/ColorMaterial.h
        src/material/BatteryMaterial.cpp
        src/material/BatteryMaterial.h
        src/ImGuiUtils.cpp
        src/ImGuiUtils.h
)

find_package(OpenGL REQUIRED)

add_executable(App)
target_sources(App PRIVATE ${SOURCES})

target_include_directories(App PRIVATE ../dependencies)
target_link_libraries(App PRIVATE glm::glm assimp::assimp glfw imgui-glfw)

if (APPLE)
    target_link_libraries(App PRIVATE OpenGL::GL)
elseif (WIN32)
    target_link_libraries(App PRIVATE opengl32)
endif ()