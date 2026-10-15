set(SOURCES
        src/main.cpp
        src/glad.c
        src/ShaderProgram.cpp
        src/ShaderProgram.h
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
        src/scene/SampleObjectScene.cpp
        src/scene/SampleObjectScene.h
        src/scene/LightScene.cpp
        src/scene/LightScene.h
        src/Sampler.cpp
        src/Sampler.h
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