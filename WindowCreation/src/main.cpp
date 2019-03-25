#include <GLFW/glfw3.h>

int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Window", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0, 0.0, 1.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}