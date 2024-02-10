#include "Camera.h"

#include "imgui/imgui.h"

float Camera::yaw = 0.0f;
float Camera::pitch = 0.0f;

Camera::Camera(glm::mat4& View)
	:m_cameraPos(0.0f, 0.0f, 300.0f),
	m_cameraFront(0.0f, 0.0f, -1.0f),
	m_cameraUp(0.0f, 1.0f, 0.0f),
	m_View(View),
    m_IsCursorDisable(false)
{
    
}

Camera::~Camera()
{

}

void Camera::BindViewMatrix(glm::mat4& View)
{
	m_View = View;
}

void Camera::CameraUpdate(float deltaTime)
{
    GLFWwindow* window = glfwGetCurrentContext();

    float cameraMoveDistance = 80.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraMoveDistance * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraMoveDistance * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraMoveDistance;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraMoveDistance;

    glm::vec3 front;
    front.x = - cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = - cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    m_cameraFront = glm::normalize(front);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if (m_IsCursorDisable)
        {
            m_IsCursorDisable = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            m_IsCursorDisable = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    m_View = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        static float lastX = 400;
        static float lastY = 300;

        float xoffset = (float)xpos - lastX;
        float yoffset = (float)ypos - lastY;
        lastX = (float)xpos;
        lastY = (float)ypos;

        float sensitivity = 0.05f;

        yaw += xoffset * sensitivity;
        pitch += yoffset * sensitivity;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
}