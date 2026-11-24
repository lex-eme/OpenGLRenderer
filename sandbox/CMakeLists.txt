add_executable(Sandbox
        Sandbox.cpp
        Sandbox.h
        TestLayer.cpp
        TestLayer.h
        Sandbox2D.cpp
        Sandbox2D.h)

target_link_libraries(Sandbox PRIVATE App spdlog::spdlog glm::glm imgui-glfw)
target_include_directories(Sandbox PRIVATE ../renderer/src)

if (APPLE)
    target_compile_definitions(Sandbox PRIVATE BN_PLATFORM_APPLE)
elseif (WIN32)
    target_compile_definitions(Sandbox PRIVATE BN_PLATFORM_WINDOWS)
endif ()