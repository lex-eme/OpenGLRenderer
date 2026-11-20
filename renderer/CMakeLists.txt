set(SOURCES
        src/main.cpp
        src/glad.c
        src/shader/ShaderProgram.cpp
        src/shader/ShaderProgram.h
        src/Transform.cpp
        src/Transform.h
        src/mesh/Mesh.cpp
        src/mesh/Mesh.h
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
        src/FrameBuffer.cpp
        src/FrameBuffer.h
        src/mesh/Grid.cpp
        src/mesh/Grid.h
        src/material/GridMaterial.cpp
        src/material/GridMaterial.h
        src/pch.cpp
        src/pch.h
        src/scene/SceneTree.cpp
        src/scene/SceneTree.h
        src/Skybox.cpp
        src/Skybox.h
        src/event/Event.h
        src/event/ApplicationEvent.h
        src/event/KeyEvent.h
        src/core/KeyCodes.h
        src/core/MouseCodes.h
        src/event/MouseEvent.h
        src/core/Window.cpp
        src/core/Window.h
        src/platform/macOS/MacWindow.cpp
        src/platform/macOS/MacWindow.h
        src/core/Application.cpp
        src/core/Application.h
        src/core/Core.h
        src/Bottleneck.h
        src/core/EntryPoint.h
        src/core/Log.cpp
        src/core/Log.h
        src/core/Layer.cpp
        src/core/Layer.h
        src/core/LayerStack.cpp
        src/core/LayerStack.h
        src/imgui/ImGuiLayer.cpp
        src/imgui/ImGuiLayer.h
        src/core/Input.h
        src/platform/macOS/MacInput.cpp
        src/platform/macOS/MacInput.h
        src/Renderer/GraphicsContext.cpp
        src/Renderer/GraphicsContext.h
        src/platform/openGL/OpenGLContext.cpp
        src/platform/openGL/OpenGLContext.h
        src/Renderer/Shader.cpp
        src/Renderer/Shader.h
        src/Renderer/Buffer.cpp
        src/Renderer/Buffer.h
        src/platform/openGL/OpenGLBuffer.cpp
        src/platform/openGL/OpenGLBuffer.h
        src/Renderer/Renderer.cpp
        src/Renderer/Renderer.h
        src/platform/openGL/OpenGLShader.cpp
        src/platform/openGL/OpenGLShader.h
        src/Renderer/VertexArray.cpp
        src/Renderer/VertexArray.h
        src/platform/openGL/OpenGLVertexArray.cpp
        src/platform/openGL/OpenGLVertexArray.h
        src/Renderer/RendererAPI.cpp
        src/Renderer/RendererAPI.h
        src/Renderer/RenderCommand.cpp
        src/Renderer/RenderCommand.h
        src/platform/openGL/OpenGLRendererAPI.cpp
        src/platform/openGL/OpenGLRendererAPI.h
        src/Renderer/OrthographicCamera.cpp
        src/Renderer/OrthographicCamera.h
        src/core/TimeStep.cpp
        src/core/TimeStep.h
        src/Renderer/Texture.cpp
        src/Renderer/Texture.h
        src/platform/openGL/OpenGLTexture.cpp
        src/platform/openGL/OpenGLTexture.h
)

find_package(OpenGL REQUIRED)

if (WIN32 AND MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif ()

add_library(App STATIC
)
target_sources(App PRIVATE ${SOURCES})

target_include_directories(App PRIVATE ../dependencies ./src)
target_link_libraries(App PRIVATE glm::glm assimp::assimp glfw imgui-glfw spdlog::spdlog)

if (APPLE)
    target_link_libraries(App PRIVATE OpenGL::GL)
    target_compile_definitions(App PRIVATE BN_PLATFORM_APPLE)
elseif (WIN32)
    target_compile_definitions(App PRIVATE BN_PLATFORM_WINDOWS BN_BUILD_DLL)
    target_link_libraries(App PRIVATE opengl32)
    add_custom_command(TARGET App POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:App>
            $<TARGET_FILE_DIR:App>
            COMMAND_EXPAND_LISTS
    )
endif ()