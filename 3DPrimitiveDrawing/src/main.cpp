#include "Platform.h"
#include "GLInclude.h"
#include "GameLoop.h"
#include "Input.h"
#include <string>
#include "vld.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GameLoop* gameLoop = NULL;

int main(int argc, char** argv)
{
	for(int i=0; i<argc; i++)
		Platform::debugPrint("\nargv = %s", argv[i]);

	Platform::debugPrint("\n\nBegin: Loading resources\n\n");
	long startTime = Platform::GetTimeInMillis();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //supported from 3.3 version

	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	//TODO: How to change number of samples runtime and disable/enable sampling runtime ( mainly for while drawing an object ).
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	int sw = (int)(mode->width*0.85f);
	int sh = (int)(mode->height*0.85f);
	int sx = (int)((mode->width - sw) / 2.0f);
	int sy = (int)((mode->height - sh) / 2.0f);

	GLFWwindow* window = glfwCreateWindow(sw, sh, "GLFW Window", NULL, NULL);
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

	gameLoop = new GameLoop((float)sw, (float)sh);
	Input::Init();

	double previousTime = glfwGetTime();
	unsigned int frameCount = 0;

	Platform::debugPrint("\nEnd: Loading resources, Load Time : %ld milliseconds\n\n", Platform::GetTimeInMillis() - startTime);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		for (int i = 0; i <Input::NUM_KEYS; i++)
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

		if(deltaTime > 0.5)
		{
			frameCount = (unsigned int)(frameCount*1.0 / deltaTime);
			char arr[128];
			sprintf(arr, "FPS : %d", frameCount);
			glfwSetWindowTitle(window, arr);

			frameCount = 0;
			previousTime = currentTime;
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



/*
glfwWindowHint(GLFW_RED_BITS, mode->redBits);
glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwMaximizeWindow(window);
std::cout << glGetString(GL_VERSION) << std::endl;
*/
