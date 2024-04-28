#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>

#include <chrono>
#include <cmath>
#include <iostream>

#include "Indexbuffer.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "glTools.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "timer.hpp"

#define IMGUI
int FPS = 60;
static Timer timer;
static long long int realFPS = 0;

void Time() {
    // Frame info
    static long long int lastTime;
    long long int currentTime = std::chrono::duration_cast<std::chrono::microseconds>(
                                    std::chrono::high_resolution_clock::now().time_since_epoch())
                                    .count();
    long long int deltaTime = currentTime - lastTime;
    realFPS = 1000000 / deltaTime;
    // std::cout << "FPS: " << 1000000 / deltaTime << " " << deltaTime << " " << lastTime << std::endl;
    lastTime = currentTime;
}

bool DrawTime(int fps) {
    if (fps == -1)
        return true;
    // Frame info
    timer.tick();

    auto deltaTime = timer.us();
    if (deltaTime > std::chrono::microseconds(1000000 / fps))
        return true;
    return false;
}

GLFWwindow* init() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "ERROR: glfwInit() \n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwSwapInterval(10);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: glewInit() \n";
    }
    return window;
}

int main(void) {
    {
        GLFWwindow* window = init();

        ////////////////////////////////
        float positions[] = {
            // positions 2    // texture2
            0.0f,   0.0f,   0.0f,   0.0f, 0.0f,  //
            100.0f, 0.0f,   0.0f,   1.0f, 0.0f,  //
            100.0f, 100.0f, 0.0f,   1.0f, 1.0f,  //
            0.0f,   100.0f, 0.0f,   0.0f, 1.0f,  //

            0.0f,   0.0f,   100.0f, 0.0f, 0.0f,  //
            100.0f, 0.0f,   100.0f, 1.0f, 0.0f,  //
            100.0f, 100.0f, 100.0f, 1.0f, 1.0f,  //
            0.0f,   100.0f, 100.0f, 0.0f, 1.0f,  //

            200.0f, 0.0f,   0.0f,   0.0f, 0.0f,  //
            300.0f, 0.0f,   0.0f,   1.0f, 0.0f,  //
            300.0f, 100.0f, 0.0f,   1.0f, 1.0f,  //
            200.0f, 100.0f, 0.0f,   0.0f, 1.0f,  //

            200.0f, 0.0f,   100.0f, 0.0f, 0.0f,  //
            300.0f, 0.0f,   100.0f, 1.0f, 0.0f,  //
            300.0f, 100.0f, 100.0f, 1.0f, 1.0f,  //
            200.0f, 100.0f, 100.0f, 0.0f, 1.0f,  //

        };
        unsigned int indices[] = {0, 1, 2, 2, 3, 0, 5, 6, 7, 7, 4, 5, 8, 9, 10, 10, 11, 8, 13, 14, 15, 15, 12, 13};
        // unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        Texture texture("C:\\Users\\Dom\\Desktop\\logo2.png");
        IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));
        Renderer renderer;

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        Shader shader("D:\\Project\\aerodynamics\\simple-aerodynamic\\files\\shaders\\Basic.shader");
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);
        ////////////////////////////////

        // texture.Bind();

        shader.Unbind();
        // texture.Unbind();

        va.UnBind();
        ib.Unbinde();
        vb.Unbinde();

#ifdef IMGUI
        {
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            (void)io;
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            const char* glsl_version = "#version 130";
            ImGui_ImplOpenGL3_Init(glsl_version);
        }
#endif

        glm::vec3 translation(100, 100, 0);
        glm::mat4 proj;
        glm::mat4 view;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp;

        shader.Bind();

        /* Loop until the user closes the window */
        timer.start();
        float ang[3] = {0.0f};
        while (!glfwWindowShouldClose(window)) {
            bool draw = true;
            draw = DrawTime(-1);
            {
                proj = glm::ortho(0.0f, float(width), 0.0f, float(height), -50.0f, 250.0f);
                view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
                auto rotX = glm::rotate(glm::mat4(1.0f), to_radian(ang[0]), glm::vec3(1, 0, 0));
                auto rotY = glm::rotate(glm::mat4(1.0f), to_radian(ang[1]), glm::vec3(0, 1, 0));
                auto rotZ = glm::rotate(glm::mat4(1.0f), to_radian(ang[2]), glm::vec3(0, 0, 1));

                model = glm::translate(glm::mat4(1.0f), translation) * (rotX * rotY * rotZ);
                mvp = proj * view * model;
            }

#ifdef IMGUI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
#endif
            texture.Bind();

            if (draw) {
                Time();
                renderer.Clear();

                // shader.Bind();
                // renderer.Draw(va, ib, shader);
                // mvp = proj * view * model;

                /*---------------------------- */
                /*---------------------------- */
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
                timer.start();
            }

#ifdef IMGUI
            ImGui::Text("This is some useful text.");  // Display some text (you can use a format strings too)
            ImGui::SliderFloat3("POS", &translation.x, 0, 300);
            ImGui::SliderFloat3("ROT", ang, 0, 360);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
            if (draw) {
                /* Swap front and back buffers */
                glfwSwapBuffers(window);
                /* Poll for and process events */
                glfwPollEvents();
            }
            usleep(1);
        }
    }
    std::cout << "LAST FPS: " << realFPS << "\n";
    glfwTerminate();
    return 0;
}