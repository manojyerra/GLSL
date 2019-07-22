#include "Macros.h"
#include "GameLoop.h"
#include "Input.h"
//#include "vld.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GameLoop* gameLoop = NULL;

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	//TODO: How to change number of samples runtime and disable/enable sampling runtime ( mainly for while drawing an object ).
	glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int sw = mode->width*0.75;
	int sh = mode->height*0.75;

	GLFWwindow* window = glfwCreateWindow(sw, sh, "GLFW Window", NULL, NULL);
	//glfwMaximizeWindow(window);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error" << std::endl;
	}

	glfwSwapInterval(1);

	gameLoop = new GameLoop(sw, sh);
	Input::Init();

	double previousTime = glfwGetTime();
	unsigned int frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		Input::Update(xpos, ypos, 1.0f / 30.0f);
		gameLoop->Update(1.0f / 30.0f);
		gameLoop->Draw();

		glfwSwapBuffers(window);

		frameCount++;
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		//Sleep(1);

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
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.

	char arr[1024];
	sprintf(arr, "\nOnSizeChange : sw = %d sh = %d", width, height);
	OutputDebugStringA(arr);

	if (gameLoop && width > 10 && height > 10)
	{
		gameLoop->SetScreenSize(width, height);
	}
	else
	{
		OutputDebugStringA("\n\n ***** Ignoring framebuffer size callback. **** \n\n");
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
The action is one of GLFW_PRESS or GLFW_RELEASE.

Mouse button states for named buttons are also saved in per - window state arrays that can be polled with glfwGetMouseButton.

int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
if (state == GLFW_PRESS)
{
	upgrade_cow();
}


glfwWindowHint(GLFW_RED_BITS, mode->redBits);
glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);


glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);


glfwSetWindowPos(window, 0, 0);

std::cout << glGetString(GL_VERSION) << std::endl;

*/