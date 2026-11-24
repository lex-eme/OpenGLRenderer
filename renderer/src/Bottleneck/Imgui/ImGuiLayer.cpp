#include "pch.h"

#include "ImGuiLayer.h"

#include "Bottleneck/Core/Application.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


void Bottleneck::ImGuiLayer::onAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Application& application = Application::get();
    Window& window = application.getWindow();
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window.getNativeWindow()), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void Bottleneck::ImGuiLayer::onDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Bottleneck::ImGuiLayer::onEvent(Event& event) {
    if (m_blockEvents) {
        const ImGuiIO& io = ImGui::GetIO();
        event.handled |= event.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        event.handled |= event.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

void Bottleneck::ImGuiLayer::onImGuiRender() {
    ImGui::ShowDemoWindow();
}

void Bottleneck::ImGuiLayer::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Bottleneck::ImGuiLayer::end() {
    ImGuiIO& io = ImGui::GetIO();
    const Application& app = Application::get();
    io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()),
                            static_cast<float>(app.getWindow().getHeight()));

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
