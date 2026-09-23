include(FetchContent)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY "https://github.com/SFML/SFML"
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

add_subdirectory(sfml)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.91.1
)

FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG v3.0
)
add_subdirectory(imgui-sfml)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.3
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        soil2
        GIT_REPOSITORY https://github.com/SpartanJ/SOIL2
        GIT_TAG 1.4.1
)

FetchContent_MakeAvailable(soil2)