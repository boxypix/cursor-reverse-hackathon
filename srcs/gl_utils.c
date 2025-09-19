#include "morphosis.h"

void 						framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	int 				i = 0;
	glViewport(0, 0, width, height);

	if (window)
		i++;
}

void 						processInput(GLFWwindow *window, t_gl *gl)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		gl->export = 1;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void 						init_gl(t_gl *gl)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	gl->window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Morphosis", NULL, NULL);
	glfwMakeContextCurrent(gl->window);
	glewExperimental = GL_TRUE;
	glewInit();
	if (!gl->window)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit (1);
	}
	glfwSetFramebufferSizeCallback(gl->window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
}

void 						terminate_gl(t_gl *gl)
{
	glDeleteVertexArrays(1, &gl->vao);
	glDeleteBuffers(1, &gl->vbo);
	glDeleteProgram(gl->shaderProgram);
	glfwTerminate();
}
