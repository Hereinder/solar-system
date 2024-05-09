
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

// Defines several possible options for camera movement.
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

// Default camera values
const float SPEED = 5.5f;
const float SENSITIVITY = 0.15f;
const float ZOOM = 45.0f;

class Camera {
public:
    // constructor with vectors
    static Camera& getInstance() {
        static Camera m_Instance;
        return m_Instance;
    }

    Camera(Camera const&) = delete;
    void operator=(Camera const&) = delete;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
    glm::vec3 GetViewPosition() { return m_Position; }

    void init(unsigned int screenWidth, unsigned int screenHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
              glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0, float pitch = 0.0);

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

private:
    Camera();
    // Camera(Camera const&);  // Don't Implement
    // void operator=(Camera const&);

    void SetCursorMode(uint32_t mode);
    uint32_t GetCursorMode();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    void test();

private:
    // camera Attributes
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    // euler Angles
    float m_Yaw;
    float m_Pitch;
    // camera options
    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;
    std::unordered_map<Camera_Movement, bool> map;

    float lastX = 0;
    float lastY = 0;
    uint32_t m_CursorMode;
    bool firstMouse = true;

    std::thread* loop;
    Camera* m_Instance;
    std::mutex m_Mutex;
    bool m_Inited = false;

    unsigned int m_ScreenWidth;
    unsigned int m_ScreenHeight;
};
