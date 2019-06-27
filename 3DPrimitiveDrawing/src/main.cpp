#include "Macros.h"
#include "GameLoop.h"
#include "Input.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 16);

	int sw = 1500;
	int sh = 800;

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

	//std::cout << glGetString(GL_VERSION) << std::endl;
	

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		Input::Update(1.0f / 30.0f);
		gameLoop->Update(1.0f/30.0f);
		gameLoop->Draw();

		glfwSwapBuffers(window);
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


/*
The action is one of GLFW_PRESS or GLFW_RELEASE.

Mouse button states for named buttons are also saved in per - window state arrays that can be polled with glfwGetMouseButton.

int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
if (state == GLFW_PRESS)
{
	upgrade_cow();
}

*/