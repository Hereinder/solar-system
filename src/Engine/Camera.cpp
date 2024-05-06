#include "Camera.hpp"

static CameraModel::Camera* camera;

void CameraModel::init(unsigned int screenWidth, unsigned int screenHeight) {
    if (!camera)
        camera = new CameraModel::Camera(screenWidth, screenHeight);
}

glm::mat4 CameraModel::GetViewMatrix() {
    if (!camera)
        std::cout << "[ERROR CameraMdoel]: Did you inited camera? \n";
    return camera->GetViewMatrix();
}

glm::vec3 CameraModel::GetViewPosition() {
    if (!camera)
        std::cout << "[ERROR CameraMdoel]: Did you inited camera? \n";
    return camera->GetViewPosition();
}

void CameraModel::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    if (!camera)
        std::cout << "[ERROR CameraMdoel]: Did you inited camera? \n";
    camera->ProcessKeyboard(direction, deltaTime);
}

void CameraModel::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (camera->GetCursorMode() != GLFW_CURSOR_DISABLED) {
        return;
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera->firstMouse) {
        camera->lastX = xpos;
        camera->lastY = ypos;
        camera->firstMouse = false;
    }

    float xoffset = xpos - camera->lastX;
    float yoffset = camera->lastY - ypos;  // reversed since y-coordinates go from bottom to top

    camera->lastX = xpos;
    camera->lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void CameraModel::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

void CameraModel::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        camera->map[CameraModel::Camera_Movement::LEFT] = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        camera->map[CameraModel::Camera_Movement::RIGHT] = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        camera->map[CameraModel::Camera_Movement::FORWARD] = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        camera->map[CameraModel::Camera_Movement::BACKWARD] = true;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        camera->map[CameraModel::Camera_Movement::UP] = true;
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        camera->map[CameraModel::Camera_Movement::DOWN] = true;
    }

    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        camera->map[CameraModel::Camera_Movement::LEFT] = false;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        camera->map[CameraModel::Camera_Movement::RIGHT] = false;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        camera->map[CameraModel::Camera_Movement::FORWARD] = false;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        camera->map[CameraModel::Camera_Movement::BACKWARD] = false;
    }
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
        camera->map[CameraModel::Camera_Movement::UP] = false;
    }
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
        camera->map[CameraModel::Camera_Movement::DOWN] = false;
    }
}

void CameraModel::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
        if (camera->GetCursorMode() != GLFW_CURSOR_NORMAL) {
            camera->SetCursorMode(GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        if (camera->GetCursorMode() != GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera->SetCursorMode(GLFW_CURSOR_DISABLED);
        }
    }
}