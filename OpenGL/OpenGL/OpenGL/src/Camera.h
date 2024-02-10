#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"

class Camera {
public:
	Camera(glm::mat4& View);
	~Camera();

	void BindViewMatrix(glm::mat4& View);
	void CameraUpdate(float deltaTime);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	static float yaw;
	static float pitch;
private:

	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::mat4& m_View;

	bool m_IsCursorDisable;
};