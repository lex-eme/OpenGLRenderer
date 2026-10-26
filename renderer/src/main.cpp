#include <glad/glad.h>

#include <fstream>
#include <iostream>

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "scene/Scene.h"

int width = 1600, height = 1050;
Scene scene;
bool mouseIsInViewport = false;

void framebuffer_size_callback(GLFWwindow* window, const int newWidth, const int newHeight) {
}

void keyboard_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    if (action == GLFW_RELEASE) {
        scene.keyReleased(key);
    }

    if (!mouseIsInViewport) {
        return;
    }

    if (action == GLFW_PRESS) {
        scene.keyPressed(key);
    }
}

void mouseButton_callback(GLFWwindow* window, const int button, const int action, int mods) {
    if (action == GLFW_RELEASE) {
        scene.mouseButtonReleased(button);
    }

    if (!mouseIsInViewport) {
        return;
    }

    if (action == GLFW_PRESS) {
        scene.mouseButtonPressed(button);
    }
}

static void glfw_error_callback(const int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(width * main_scale, height * main_scale, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetMouseButtonCallback(window, mouseButton_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "Version OpenGL : " << GLVersion.major << "." << GLVersion.minor << std::endl;

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &nrAttributes);
    std::cout << "Maximum nr of vertex uniform supported: " << nrAttributes << std::endl;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &nrAttributes);
    std::cout << "Maximum nr of fragment uniform supported: " << nrAttributes << std::endl;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &nrAttributes);
    std::cout << "Maximum nr of uniform buffer bindings supported: " << nrAttributes << std::endl;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &nrAttributes);
    std::cout << "Maximum uniform block size supported: " << nrAttributes << std::endl;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;
    // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    io.ConfigDpiScaleFonts = true;
    // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true; // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
#endif

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 410";
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifdef __APPLE__
    width *= 2;
    height *= 2;
#endif
    scene.load(window, width, height);

    ImVec2 viewportSize(300, 300);
    double currentTime = glfwGetTime();
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        const double newTime = glfwGetTime();
        deltaTime = newTime - currentTime;
        currentTime = newTime;

        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        scene.update(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        ImGui::ShowDemoWindow();
        ImGui::Begin("Game", nullptr);
        mouseIsInViewport = ImGui::IsWindowHovered();

        if (ImVec2 contentRegion = ImGui::GetContentRegionAvail();
            viewportSize.x != contentRegion.x || viewportSize.y != contentRegion.y) {
            viewportSize = contentRegion;
            scene.windowResized(static_cast<int>(contentRegion.x), static_cast<int>(contentRegion.y));
        }

        scene.draw();
        ImGui::Image(scene.frameBuffer.texture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::Begin("Properties", nullptr);
        scene.drawDebug();
        ImGui::End();
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
