#pragma once
#include <ostream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class BOSSENGINE_API GraphicsManager
{	
private:
	static GLFWwindow* _window;
	static std::shared_ptr<Camera> _mainCamera;
	
	static float lastX;
	static float lastY;

	static bool firstMouse;

public:
	static void Start()
	{
		SetupWindow();
		SetupCamera();
	}

	static GLFWwindow* GetWindow()
	{
		return _window;
	}

	static std::shared_ptr<Camera> GetMainCamera()
	{
		return _mainCamera;
	}

private:
	
	static void SetupWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		glfwSetCursorPosCallback(_window, MouseMovedCallback);
		glfwSetScrollCallback(_window, ScrollCallback);

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	static void SetupCamera()
	{
		lastX = 0;
		lastY = 0;
		firstMouse = true;
	}
	
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	
	static void MouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (firstMouse) // initially set to true
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		float xOffset = xPos - lastX;
		float yOffset = lastY - yPos; // Y ranges from bottom to top
		lastX = xPos;
		lastY = yPos;

		_mainCamera->ProcessMouseMovement(xOffset, yOffset, true);
	}

	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		_mainCamera->ProcessMouseScroll(yOffset);
	}
};

GLFWwindow* GraphicsManager::_window = nullptr;
std::shared_ptr<Camera> GraphicsManager::_mainCamera = std::make_shared<Camera>(glm::vec3(0, 0, 10.f));
float GraphicsManager::lastX = 0.0f;
float GraphicsManager::lastY = 0.0f;
bool GraphicsManager::firstMouse = true;