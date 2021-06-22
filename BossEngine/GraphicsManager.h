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
	
	static float _lastX;
	static float _lastY;

	static bool _firstMouse;

	static bool _wireframeMode;

public:
	static void Start()
	{
		SetupWindow();
	}

	static GLFWwindow* GetWindow()
	{
		return _window;
	}

	static std::shared_ptr<Camera> GetMainCamera()
	{
		return _mainCamera;
	}

	static void ToggleWireframe()
	{
		_wireframeMode = !_wireframeMode;
		if (_wireframeMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

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
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	
	static void MouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (_firstMouse) // initially set to true
		{
			_lastX = xPos;
			_lastY = yPos;
			_firstMouse = false;
		}

		float xOffset = xPos - _lastX;
		float yOffset = _lastY - yPos; // Y ranges from bottom to top
		_lastX = xPos;
		_lastY = yPos;

		_mainCamera->ProcessMouseMovement(xOffset, yOffset, true);
	}

	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		_mainCamera->ProcessMouseScroll(yOffset);
	}
};

GLFWwindow* GraphicsManager::_window = nullptr;
std::shared_ptr<Camera> GraphicsManager::_mainCamera = std::make_shared<Camera>(glm::vec3(0, 0, 10.f));
float GraphicsManager::_lastX = 0.0f;
float GraphicsManager::_lastY = 0.0f;
bool GraphicsManager::_firstMouse = true;
bool GraphicsManager::_wireframeMode = false;