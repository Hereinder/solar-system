#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// #define IMGUI
static bool showAboutPopup = false;

class Gui {
public:
    static void init(GLFWwindow* window) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        const char* glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    static void newFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    static void render(const long long int& FPSValue) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ShowMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                ShowMenuEdit();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ShowMenuHelp();
                ImGui::EndMenu();
            }

            ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetCursorPosX());
            ImGui::Text("FPS: %llu", FPSValue);  // Display some text (you can use a format strings too)
            ImGui::EndMainMenuBar();
        }

        handlePopup();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:
    static void ShowMenuFile() {
        if (ImGui::MenuItem("New")) {
        }
        if (ImGui::MenuItem("Open", "Ctrl+O")) {
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
        }
        if (ImGui::MenuItem("Save As..")) {
        }

        ImGui::Separator();
        if (ImGui::BeginMenu("Options")) {
            static bool enabled = true;
            ImGui::MenuItem("Enabled", "", &enabled);
            ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Border);
            for (int i = 0; i < 10; i++) ImGui::Text("Scrolling Text %d", i);
            ImGui::EndChild();
            static float f = 0.5f;
            static int n = 0;
            ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
            ImGui::InputFloat("Input", &f, 0.1f);
            ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");

            static bool b = true;
            ImGui::Checkbox("SomeOption", &b);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Colors")) {
            float sz = ImGui::GetTextLineHeight();
            for (int i = 0; i < ImGuiCol_COUNT; i++) {
                const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz),
                                                          ImGui::GetColorU32((ImGuiCol)i));
                ImGui::Dummy(ImVec2(sz, sz));
                ImGui::SameLine();
                ImGui::MenuItem(name);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Disabled", false))  // Disabled
        {
            IM_ASSERT(0);
        }
        if (ImGui::MenuItem("Checked", NULL, true)) {
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
        }
    }

    static void ShowMenuEdit() {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {
        }
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
        }  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {
            std::cout << "CUT \n";
        }
        if (ImGui::MenuItem("Copy", "CTRL+C")) {
        }
        if (ImGui::MenuItem("Paste", "CTRL+V")) {
        }
    }

    static void ShowMenuHelp() {
        if (ImGui::MenuItem("About", NULL, false, true)) {
            showAboutPopup = true;
        }
    }

    static void handlePopup() {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (showAboutPopup) {
            ImGui::OpenPopup("FilePopup");
            if (ImGui::BeginPopupModal("FilePopup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
                ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
                if (ImGui::Button("OK")) {
                    showAboutPopup = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }
};