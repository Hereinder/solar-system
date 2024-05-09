#include "Engine.hpp"

#include <unistd.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

#include "Gui.hpp"
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera::getInstance().mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera::getInstance().scroll_callback(window, xoffset, yoffset);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Camera::getInstance().key_callback(window, key, scancode, action, mods);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Camera::getInstance().mouse_button_callback(window, button, action, mods);
}

Engine::Engine() {
    window = Init();
    if (!window) {
        std::cout << "[ERROR Engine]: init error";
    }

    glfwGetWindowSize(window, &m_Width, &m_Height);
    Camera::getInstance().init(m_Width, m_Height, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, 0.0);
    Gui::init(window);

    // Prepare();
    PrepareInstance();
    m_FrameTimer.reset();
    m_GlobalTimer.reset();
}

GLFWwindow* Engine::Init() {
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
    //  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: glewInit() \n";
    }
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    return window;
}

void Engine::Run() {
    {
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 20000.0f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            if (CheckDrawTime(-1)) {
                m_FrameTimer.reset();
                Gui::newFrame();

                // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_Renderer.Draw(proj, CameraModel::GetViewMatrix(), CameraModel::GetViewPosition());

                Gui::render(m_FPSValue);
                /* Swap front and back buffers */
                glfwSwapBuffers(window);
                /* Poll for and process events */
                glfwPollEvents();

                CalculateFPS();
                m_FrameCnt++;
            }
            usleep(1);
        }
    }
    glfwTerminate();
}

bool Engine::CheckDrawTime(const int fps) {
    if (fps == -1)
        return true;

    m_FrameTimer.tick();
    if (m_FrameTimer.us() > std::chrono::microseconds(1000000 / fps))
        return true;
    return false;
}

void Engine::CalculateFPS() {
    m_GlobalTimer.tick();
    // Frame info
    static long long int lastTime;
    long long int currentTime = m_GlobalTimer.us().count();
    long long int deltaTime = currentTime - lastTime;
    m_FPSValue = 1000000 / deltaTime;
    lastTime = currentTime;
}

void Engine::Prepare() {
    std::random_device dev;
    std::mt19937 rng(dev());

    // StoneObject* stone1 = new StoneObject(
    //     glm::vec3(0.0, 0.0, 0.0),
    //     "D:\\Project\\aerodynamics\\simple-aerodynamic\\files\\shaders\\Texture.shader",
    //     "C:\\Users\\Dom\\Desktop\\logo2.png");

    StoneObject* stone1 = new StoneObject(glm::vec3(6000, 1000.0, 6000), "BasicLightning.shader");

    // Define the parameters of the torus
    float majorRadius = 8000.0f;  // Radius of the torus from the center to the center of the tube
    float minorRadius = 5000.0f;  // Radius of the tube
    float height1 = 2000.0f;      // Height of the cylinder

    int cnt = 0;
    m_GlobalTimer.reset();
    while (cnt < 100000) {
        // Generate random coordinates within the cylinder
        float x = std::uniform_real_distribution<float>(-majorRadius - minorRadius, majorRadius + minorRadius)(rng);
        float z = std::uniform_real_distribution<float>(-majorRadius - minorRadius, majorRadius + minorRadius)(rng);
        float y = std::uniform_real_distribution<float>(-height1 / 2.0f, height1 / 2.0f)(rng);
        float ang = std::uniform_real_distribution<float>(-90, 90)(rng);

        // Check if the point is inside the torus
        float distanceToCenter = glm::length(glm::vec2(x, z));
        if (glm::abs(distanceToCenter) > minorRadius && glm::abs(distanceToCenter) < majorRadius) {
            stone1->Batch(x, y, z, ang);
            cnt++;
        }
    }
    std::cout << "DIFF:" << m_GlobalTimer.tick().us().count() << "\n";
    m_Renderer.PushBack(stone1);

    StaticLight* light = new StaticLight(glm::vec3(0.0, 0.0, 0.0), "Light.shader");

    m_Renderer.PushBack(light);
    m_Renderer.LoadMesh();
}