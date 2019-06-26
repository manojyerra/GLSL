#include "Macros.h"
#include "GameLoop.h"

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	int sw = 800;
	int sh = 600;

	GLFWwindow* window = glfwCreateWindow(sw, sh, "GLFW Window", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error" << std::endl;
	}

	glfwSwapInterval(1);

	GameLoop* gameLoop = new GameLoop(sw, sh);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		gameLoop->Update(1.0f/30.0f);
		gameLoop->Draw();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}