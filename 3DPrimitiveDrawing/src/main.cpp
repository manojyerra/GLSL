#include "Platform.h"
#include "GLInclude.h"
#include "GameLoop.h"
#include "Input.h"
#include <string>
#include <chrono>
#include <thread>
#include "vld.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GameLoop* gameLoop = NULL;

int main(int argc, char** argv)
{
	Platform::debugPrint("\n\nBegin: Loading resources\n\n");
	long startTime = Platform::GetTimeInMillis();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	//TODO: Check how to change number of samples at runtime.
	//glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	int sw = (int)(mode->width * 0.85f);
	int sh = (int)(mode->height * 0.85f);
	int sx = (int)((mode->width - sw) / 2.0f);
	int sy = (int)((mode->height - sh) / 2.0f);

	GLFWwindow* window = glfwCreateWindow(sw, sh, "", NULL, NULL);
	glfwSetWindowPos(window, sx, sy);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw new std::exception("\nError: Failed to initialize OpenGL context.\n");
	}

	glfwSwapInterval(1);

	gameLoop = new GameLoop((unsigned int)sw, (unsigned int)sh, argc, argv);
	Input::Init();

	double previousTime = glfwGetTime();
	unsigned int frameCount = 0;
	float limitFPSTo = 70.0f;
	unsigned int timeForLimitFPS = (unsigned int)(1000.0f / limitFPSTo);

	Platform::debugPrint("\nEnd: Loading resources, Load Time : %ld milliseconds\n\n", Platform::GetTimeInMillis() - startTime);

	while (!glfwWindowShouldClose(window))
	{
		double startTime = glfwGetTime();

		glfwPollEvents();

		for (int i = 0; i < Input::NUM_KEYS; i++)
			Input::storeKeyStates[i] = glfwGetKey(window, i);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		Input::Update((int)xpos, (int)ypos, 1.0f / 30.0f);
		gameLoop->Update(1.0f / 30.0f);
		gameLoop->Draw();

		glfwSwapBuffers(window);

		frameCount++;
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		if (deltaTime > 0.5)
		{
			frameCount = (unsigned int)((double)frameCount / deltaTime);
			char arr[128];
			sprintf(arr, "%s [FPS : %d]", Platform::GetWindowTitle(), frameCount);
			glfwSetWindowTitle(window, arr);

			frameCount = 0;
			previousTime = currentTime;
		}

		int timeTakenInMillis = (int)((glfwGetTime() - startTime) * 1000.0f);
		int sleepTimeInMillis = timeForLimitFPS - timeTakenInMillis;

		if (sleepTimeInMillis > 1 && timeTakenInMillis >= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long> (sleepTimeInMillis)));
		}
	}

	if (gameLoop)
	{
		delete gameLoop;
		gameLoop = NULL;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Platform::debugPrint("\nOnSizeChange : sw = %d sh = %d", width, height);

	if (gameLoop && width > 10 && height > 10)
	{
		gameLoop->SetScreenSize((float)width, (float)height);
	}
	else
	{
		Platform::debugPrint("\n\n ***** Ignoring framebuffer size callback. **** \n\n");
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			Input::LEFT_BUTTON_DOWN = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Input::LEFT_BUTTON_DOWN = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS)
		{
			Input::MIDDLE_BUTTON_DOWN = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Input::MIDDLE_BUTTON_DOWN = false;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			Input::RIGHT_BUTTON_DOWN = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Input::RIGHT_BUTTON_DOWN = false;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::SCROLL_STATE_STORE = (yoffset < 0) ? Input::SCROLL_DOWN : Input::SCROLL_UP;
}
