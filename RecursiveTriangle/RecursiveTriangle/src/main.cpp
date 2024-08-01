#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void triangles(unsigned int& transLoc, glm::vec3 position, glm::mat4 matrix, int level);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Recursive Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD!\n";
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader shader("shaders/shader.vert", "shaders/shader.frag");

	float vertices[] =
	{
		// position				// colors
		-1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.use();
		glBindVertexArray(VAO);

		unsigned int transLoc = glGetUniformLocation(shader.getID(), "trans");
		glm::mat4 matrix = glm::mat4(1.0f);
		triangles(transLoc, glm::vec3(0.0f, -0.5f, 0.0f), matrix, 0);

		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader.getID());
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void triangles(unsigned int& transLoc, glm::vec3 position, glm::mat4 matrix, int level)
{
	if (level >= 9) {
		return;
	}

	matrix = glm::translate(matrix, position);
	matrix = glm::scale(matrix, glm::vec3(0.5f, 0.5f, 0.0f));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(matrix));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	triangles(transLoc, glm::vec3(-1.0f, -0.5f, 0.0f), matrix, level + 1);
	triangles(transLoc, glm::vec3(1.0f, -0.5f, 0.0f), matrix, level + 1);
	triangles(transLoc, glm::vec3(0.0f, 1.5f, 0.0f), matrix, level + 1);
}