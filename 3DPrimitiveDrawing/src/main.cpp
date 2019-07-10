#include "Macros.h"
#include "GameLoop.h"
#include "Input.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int sw = mode->width;
	int sh = mode->height;

	GLFWwindow* window = glfwCreateWindow(sw, sh, "GLFW Window", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error" << std::endl;
	}

	glfwSwapInterval(1);

	GameLoop* gameLoop = new GameLoop(sw, sh);

	Input::Init();
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//std::cout << glGetString(GL_VERSION) << std::endl;

	double previousTime = glfwGetTime();
	unsigned int frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		Input::Update(1.0f / 30.0f);
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

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
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
	if(yoffset < 0)
	{
		Input::SCROLL_STATE_STORE = Input::SCROLL_DOWN;
	}
	else
	{
		Input::SCROLL_STATE_STORE = Input::SCROLL_UP;
	}
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


*/