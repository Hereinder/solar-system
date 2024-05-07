
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>
#include <unordered_map>

namespace CameraModel {
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system
    // specific input methods
    enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };
    // Default camera values
    const float SPEED = 10.5f;
    const float SENSITIVITY = 0.15f;
    const float ZOOM = 45.0f;
    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices
    // for use in OpenGL
    class Camera {
    public:
        // constructor with vectors
        Camera(unsigned int screenWidth, unsigned int screenHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0, float pitch = 0.0)
            : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
              MovementSpeed(SPEED),
              MouseSensitivity(SENSITIVITY),
              Zoom(ZOOM),
              m_ScreenWidth(screenWidth),
              m_ScreenHeight(screenHeight) {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();

            lastX = m_ScreenWidth / 2.0f;
            lastY = m_ScreenHeight / 2.0f;
            loop = new std::thread(CameraModel::Camera::test, this);
        }
        // constructor with scalar values
        Camera(unsigned int screenWidth, unsigned int screenHeight, float posX, float posY, float posZ, float upX,
               float upY, float upZ, float yaw, float pitch)
            : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
              MovementSpeed(SPEED),
              MouseSensitivity(SENSITIVITY),
              Zoom(ZOOM),
              m_ScreenWidth(screenWidth),
              m_ScreenHeight(screenHeight) {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();

            lastX = m_ScreenWidth / 2.0f;
            lastY = m_ScreenHeight / 2.0f;
            loop = new std::thread(CameraModel::Camera::test, this);
        }

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

        glm::vec3 GetViewPosition() { return Position; }

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera
        // defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
            float velocity = MovementSpeed * deltaTime;
            if (direction == FORWARD)
                Position += Front * velocity;
            if (direction == BACKWARD)
                Position -= Front * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;
            if (direction == UP)
                Position += Up * velocity;
            if (direction == DOWN)
                Position -= Up * velocity;
        }

        // // processes input received from a mouse input system. Expects the offset value in both the x and y
        // direction.
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch) {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset) {
            Zoom -= (float)yoffset;
            if (Zoom < 1.0f)
                Zoom = 1.0f;
            if (Zoom > 45.0f)
                Zoom = 45.0f;
        }

        void SetCursorMode(uint32_t mode) { m_CursorMode = mode; }
        uint32_t GetCursorMode() { return m_CursorMode; }

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors() {
            // calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            Right = glm::normalize(
                glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more
                                              // you look up or down which results in slower movement.
            Up = glm::normalize(glm::cross(Right, Front));
        }

        void test() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                for (auto const& [key, val] : map) {
                    if (val == true) {
                        ProcessKeyboard(key, 2.0f);
                    }
                }
            }
        }

    public:
        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        // euler Angles
        float Yaw;
        float Pitch;
        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
        std::unordered_map<CameraModel::Camera_Movement, bool> map;

        float lastX;
        float lastY;
        bool firstMouse = true;

    private:
        std::thread* loop;
        unsigned int m_ScreenWidth;
        unsigned int m_ScreenHeight;
        uint32_t m_CursorMode = GLFW_CURSOR_NORMAL;
    };

    void init(unsigned int screenWidth, unsigned int screenHeight);
    glm::mat4 GetViewMatrix();
    glm::vec3 GetViewPosition();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

}