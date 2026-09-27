set(SOURCES
        src/main.cpp
        src/glad.c
        src/ShaderProgram.cpp
        src/ShaderProgram.h
        src/Transform.cpp
        src/Transform.h
        src/Mesh.cpp
        src/Mesh.h
        src/Sphere.cpp
        src/Sphere.h
        src/Torus.cpp
        src/Torus.h
        src/Model.cpp
        src/Model.h
)

find_package(OpenGL REQUIRED)

add_executable(App)
target_sources(App PRIVATE ${SOURCES})

target_include_directories(App PRIVATE ../dependencies)
target_link_libraries(App PRIVATE glm::glm ImGui-SFML::ImGui-SFML SFML::Audio soil2)

if (APPLE)
    target_link_libraries(App PRIVATE OpenGL::GL)
elseif (WIN32)
    target_link_libraries(App PRIVATE opengl32)
endif ()