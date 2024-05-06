#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

#include "Camera.hpp"
#include "Renderer.hpp"
#include "StaticLight.hpp"
#include "StoneObject.hpp"
#include "TreeObject.hpp"
#include "glTools.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "timer.hpp"
#define IMGUI

static Timer timer;

static long long int realFPS = 0;
static int FRAME_CNT = 0;
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 960;

void Time() {
    // Frame info
    static long long int lastTime;
    long long int currentTime = std::chrono::duration_cast<std::chrono::microseconds>(
                                    std::chrono::high_resolution_clock::now().time_since_epoch())
                                    .count();
    long long int deltaTime = currentTime - lastTime;
    if (FRAME_CNT % 1000 == 0) {
    realFPS = 1000000 / deltaTime;
    }
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
    // FULL SCREEN
    //  window = glfwCreateWindow(640, 480, "Hello World", glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

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
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, CameraModel::mouse_callback);
    glfwSetScrollCallback(window, CameraModel::scroll_callback);
    glfwSetKeyCallback(window, CameraModel::key_callback);
    glfwSetMouseButtonCallback(window, CameraModel::mouse_button_callback);
    return window;
}

int main(void) {
    {
        GLFWwindow* window = init();

        Renderer renderer;

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        CameraModel::init(width, height);

        std::random_device dev;
        std::mt19937 rng(dev());

        // Define the parameters of the torus
        float majorRadius = 8000.0f;  // Radius of the torus from the center to the center of the tube
        float minorRadius = 5000.0f;  // Radius of the tube
        float height1 = 2000.0f;      // Height of the cylinder

        int cnt = 0;
        while (cnt < 10000) {
            // Generate random coordinates within the cylinder
            float x = std::uniform_real_distribution<float>(-majorRadius - minorRadius, majorRadius + minorRadius)(rng);
            float z = std::uniform_real_distribution<float>(-majorRadius - minorRadius, majorRadius + minorRadius)(rng);
            float y = std::uniform_real_distribution<float>(-height1 / 2.0f, height1 / 2.0f)(rng);

            // Check if the point is inside the torus
            float distanceToCenter = glm::length(glm::vec2(x, z));
            if (glm::abs(distanceToCenter) > minorRadius && glm::abs(distanceToCenter) < majorRadius) {
                stone1->Batch(x, y, z);
                cnt++;
            }
        }
        renderer.PushBack(stone1);
        renderer.PushBack(light);
        renderer.LoadMesh();

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

        /* Loop until the user closes the window */
        timer.start();
        float ang[3] = {0.0f};
        while (!glfwWindowShouldClose(window)) {
            bool draw = true;
            draw = DrawTime(-1);

#ifdef IMGUI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
#endif
            // texture.Bind();

            if (draw) {
                timer.start();

                // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Time();

                glm::mat4 proj;
                glm::mat4 view;
                {
                    proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);
                    view = CameraModel::GetViewMatrix();
                    glm::vec3 campos = CameraModel::GetViewPosition();
                    renderer.Draw(proj, view, campos);
                }
            }

#ifdef IMGUI
            ImGui::Text("FPS: %llu", realFPS);  // Display some text (you can use a format strings too)
            // ImGui::SliderFloat3("POS", &translation.x, 0, 300);
            // ImGui::SliderFloat3("ROT", ang, 0, 360);

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
    glfwTerminate();
    return 0;
}