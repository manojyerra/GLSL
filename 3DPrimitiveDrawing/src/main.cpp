#include "Macros.h"
#include "GameLoop.h"
#include "Input.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GameLoop* gameLoop = NULL;

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_SAMPLES, 8);

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.

	if (gameLoop)
	{
		gameLoop->SetScreenSize(width, height);
	}
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


glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);


glfwSetWindowPos(window, 0, 0);

std::cout << glGetString(GL_VERSION) << std::endl;

*/