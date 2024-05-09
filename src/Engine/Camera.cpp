#include "Camera.hpp"

#include <shared_mutex>

Camera::Camera() {
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_MovementSpeed = SPEED;
    m_MouseSensitivity = SENSITIVITY;
    m_Zoom = ZOOM;
    updateCameraVectors();
    loop = new std::thread(Camera::test, this);
}

void Camera::SetCursorMode(uint32_t mode) { m_CursorMode = mode; }
uint32_t Camera::GetCursorMode() { return m_CursorMode; }

void Camera::init(unsigned int screenWidth, unsigned int screenHeight, glm::vec3 position, glm::vec3 up, float yaw,
                  float pitch) {
    if (!m_Inited) {
        m_Inited = true;
        m_Position = position;
        m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
        m_ScreenWidth = screenWidth;
        m_ScreenHeight = screenHeight;
        m_CursorMode = GLFW_CURSOR_NORMAL;
        lastX = m_ScreenWidth / 2.0f;
        lastY = m_ScreenHeight / 2.0f;
    } else {
        std::cout << "INITED ALREADY \n";
    }
}

void Camera::mouse_callback(GLFWwindow*, double xposIn, double yposIn) {
    if (GetCursorMode() != GLFW_CURSOR_DISABLED) {
        firstMouse = true;
        return;
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    ProcessMouseMovement(xoffset, yoffset);
}

void Camera::scroll_callback(GLFWwindow*, double, double) {}

void Camera::key_callback(GLFWwindow*, int key, int, int action, int) {
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        map[Camera_Movement::LEFT] = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        map[Camera_Movement::RIGHT] = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        map[Camera_Movement::FORWARD] = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        map[Camera_Movement::BACKWARD] = true;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        map[Camera_Movement::UP] = true;
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        map[Camera_Movement::DOWN] = true;
    }

    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        map[Camera_Movement::LEFT] = false;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        map[Camera_Movement::RIGHT] = false;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        map[Camera_Movement::FORWARD] = false;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        map[Camera_Movement::BACKWARD] = false;
    }
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
        map[Camera_Movement::UP] = false;
    }
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
        map[Camera_Movement::DOWN] = false;
    }
}

void Camera::mouse_button_callback(GLFWwindow* window, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
        if (GetCursorMode() != GLFW_CURSOR_NORMAL) {
            SetCursorMode(GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        if (GetCursorMode() != GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            SetCursorMode(GLFW_CURSOR_DISABLED);
        }
    }
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera
// defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = m_MovementSpeed * deltaTime;
    if (direction == FORWARD)
        m_Position += m_Front * velocity;
    if (direction == BACKWARD)
        m_Position -= m_Front * velocity;
    if (direction == LEFT)
        m_Position -= m_Right * velocity;
    if (direction == RIGHT)
        m_Position += m_Right * velocity;
    if (direction == UP)
        m_Position += m_Up * velocity;
    if (direction == DOWN)
        m_Position -= m_Up * velocity;
}

// // processes input received from a mouse input system. Expects the offset value in both the x and y
// direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    // update m_Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset) {
    m_Zoom -= (float)yoffset;
    if (m_Zoom < 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom > 45.0f)
        m_Zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_Right = glm::normalize(
        glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0
                                          // the more you look up or down which results in slower movement.
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::test() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        for (auto const& [key, val] : map) {
            if (val == true) {
                ProcessKeyboard(key, 2.0f);
            }
        }
    }
}