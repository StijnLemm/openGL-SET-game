#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "fpsCam.h"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLFWwindow* window;

void init();
void update();
void draw();

int windowWidth = 1400;
int windowHeight = 800;
bool ribbonMode = false;
bool flyMode = false;
FpsCam* camera;

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";


	window = glfwCreateWindow(windowWidth, windowHeight, "Set - Card game", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}


void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
			if (key == GLFW_KEY_M)
			{
				if (action == GLFW_PRESS)
				{
					ribbonMode = !ribbonMode;
					if (ribbonMode)
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						std::cout << "Ribbon mode enabled" << std::endl;
					}
					else
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						std::cout << "Fill mode enabled" << std::endl;
					}
				}
			}
			if (key == GLFW_KEY_N)
			{
				if (action == GLFW_PRESS)
				{
					flyMode = !flyMode;

					if (flyMode)
					{
						camera->enable(window);
						std::cout << "Fly mode enabled" << std::endl;
					}
					else
					{
						camera->disable(window);
						std::cout << "Fly mode disabled" << std::endl;
					}
				}
			}
		});

	camera = new FpsCam();


	tigl::shader->enableTexture(true);

	glEnable(GL_TEXTURE);
	glEnable(GL_DEPTH_TEST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Resources/texture_atlas.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


void update()
{
	if (flyMode)
	{
		camera->update(window);
	}
}

glm::vec3 calculatePosition(int index) {
	// start at position 0,0 with index 0
	int x = (index / 3);
	int y = index % 3 - 1;

	return glm::vec3(x, y * 2, 0);
}

void createCard(glm::mat4 modelMatrix, float textureX, float textureY) {

	glm::vec3 position(0, 0, 0);

	glm::vec3 a(-0.5, -1, -0.5);
	glm::vec3 b(0.5, -1, -0.5);
	glm::vec3 c(0.5, 1, -0.5);
	glm::vec3 d(-0.5, 1, -0.5);

	tigl::shader->setModelMatrix(modelMatrix);
	tigl::begin(GL_QUADS);

	float multiplier = 1.0f / 9.0f;

	tigl::addVertex(Vertex::PT(a, glm::vec2((textureX - 1.0f) * multiplier, (textureY - 1.0f) * multiplier)));
	tigl::addVertex(Vertex::PT(b, glm::vec2((textureX - 1.0f) * multiplier, textureY * multiplier)));
	tigl::addVertex(Vertex::PT(c, glm::vec2(textureX * multiplier, textureY * multiplier)));
	tigl::addVertex(Vertex::PT(d, glm::vec2(textureX * multiplier, (textureY - 1.0f) * multiplier)));
	tigl::end();
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (flyMode)
	{
		tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(90.0f), (float)(windowWidth / windowHeight), 0.1f, 200.0f));
		tigl::shader->setViewMatrix(camera->getMatrix());
	}
	else {

		tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(90.0f), (float)(windowWidth / windowHeight), 0.1f, 200.0f));
		tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(2, 0, 5), glm::vec3(2, 0.5, 0), glm::vec3(0, 1, 0)));
	}

	tigl::shader->enableColor(true);

	for (int i = 0; i < 13; i++)
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, calculatePosition(i));

		createCard(modelMatrix, (float)(i % 9), (float)(i % 9));
	}
}
